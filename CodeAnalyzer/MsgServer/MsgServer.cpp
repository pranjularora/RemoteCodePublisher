/////////////////////////////////////////////////////////////////////////
// MsgServer.cpp - Demonstrates simple one-way HTTP style messaging    //
//                 and file transfer                                   //
//																	   //
// Author: Pranjul Arora											   //
// Source:															   //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016           //
// Application: OOD Project #4                                         //
// Platform:    Visual Studio 2015, Dell XPS 8900, Windows 10 pro      //
/////////////////////////////////////////////////////////////////////////
/*
* This package implements a server that receives HTTP style messages and
* files from multiple concurrent clients and simply displays the messages
* and stores files.
*
* It's purpose is to provide a very simple illustration of how to use
* the Socket Package provided for Project #4.
*/
/*
* Required Files:
*   MsgClient.cpp, MsgServer.cpp
*   HttpMessage.h, HttpMessage.cpp
*   Cpp11-BlockingQueue.h
*   Sockets.h, Sockets.cpp
*   FileSystem.h, FileSystem.cpp
*   Logger.h, Logger.cpp
*   Utilities.h, Utilities.cpp
*/
/*
 * ToDo:
 * - pull the receiving code into a Receiver class
 * - Receiver should own a BlockingQueue, exposed through a
 *   public method:
 *     HttpMessage msg = Receiver.GetMessage()
 * - You will start the Receiver instance like this:
 *     Receiver rcvr("localhost:8080");
 *     ClientHandler ch;
 *     rcvr.start(ch);
 */
#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../CodePublisher/CodePublisher.h"
#include "../ScopeStack/ScopeStack.h"

#include <string>
#include <iostream>
#include <sstream>



using namespace Logging;
//using namespace Async;
using Show = StaticLogger<1>;
using namespace Utilities;
using namespace CodeAnalysis;
static size_t listen_Port = 8080;
static size_t sendTo_Port = 8084;
/////////////////////////////////////////////////////////////////////
// ClientHandler class
/////////////////////////////////////////////////////////////////////
// - instances of this class are passed by reference to a SocketListener
// - when the listener returns from Accept with a socket it creates an
//   instance of this class to manage communication with the client.
// - You no longer need to be careful using data members of this class
//   because each client handler thread gets its own copy of this 
//   instance so you won't get unwanted sharing.
// - I changed the SocketListener semantics to pass
//   instances of this class by value for version 5.2.
// - that means that all ClientHandlers need copy semantics.
//
class ClientHandler
{
public:
	ClientHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void operator()(Socket socket);
	void PublishFiles(std::string category);
	static std::vector<std::string> splitAndReplace(std::string, std::string pattern);
	static void processCommandLineServerArgs(int argc, char* argv[]);
private:
	void removeAllFiles(std::string path);
	void deleteSelectedFiles(std::string list, std::string category);
	void lazyDownload(std::string list, std::string category);

	void sendLazyFiles(std::string fileName, std::string category);

	void noParent(std::string list, std::string category);

	void sendNoParentFiles(std::string fileName, std::string category);


	bool connectionClosed_;
	HttpMessage readMessage(Socket& socket);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket, std::string category);
	BlockingQueue<HttpMessage>& msgQ_;
	DependencyTable dep;
};

///////////// My code ///////////////////
class MsgClient_server
{
public:
	using EndPoint = std::string;
	void execute_server(std::string msgType, std::string category);
	void makeMessageForDownloadFiles(HttpMessage& msg, std::string message, std::string category, std::string msgType);

	void makeMessageForGetFiles(HttpMessage& msg, std::string list_of_files, std::string category, std::string msgType);
	void makeMessageForPublishfiles(HttpMessage& msg, std::string list_of_files, std::string category, std::string msgCheck);
	//void execute1(std::string, std::string);

	static void startSender(std::string msgType, std::string category);
private:
	HttpMessage makeMessage_server(size_t n, const std::string& msgBody, const EndPoint& ep);
	void sendMessage_server(HttpMessage& msg, Socket& socket);
	bool sendFile_server(const std::string& fqname, Socket& socket, std::string category, std::string msgType);
};




//----< this defines processing to frame messages >------------------

// mini tokenizer written to tokenize a string based on delimeters
std::vector<std::string> ClientHandler::splitAndReplace(std::string split, std::string pattern) {
	std::vector<std::string> splitVec;

	std::stringstream stringStream(split);
	std::string line;
	while (std::getline(stringStream, line))
	{
		std::size_t prev = 0, pos;
		while ((pos = line.find_first_of(pattern, prev)) != std::string::npos)
		{
			if (pos > prev)
				splitVec.push_back(line.substr(prev, pos - prev));
			prev = pos + 1;
		}
		if (prev < line.length())
			splitVec.push_back(line.substr(prev, std::string::npos));
	}
	return splitVec;
}

//----< Read messages sent by the Client >------------------------------
HttpMessage ClientHandler::readMessage(Socket& socket) {
	connectionClosed_ = false;
	HttpMessage msg;
	while (true) {// read message attributes
		std::string attribString = socket.recvString('\n');
		if (attribString.size() > 1) {
			HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);
			msg.addAttribute(attrib);
		}
		else
			break;
	}	// If client is done, connection breaks and recvString returns empty string
	if (msg.attributes().size() == 0) {
		connectionClosed_ = true;
		return msg;
	}
	if (msg.attributes()[0].first == "POST") {	// read body if POST - all messages in this demo are POSTs
		std::string filename = msg.findValue("file");// is this a file message?
		if (filename != "") {
			size_t contentSize; std::string sizeString = msg.findValue("content-length"); std::string category = msg.findValue("Category");
			if (sizeString != "")
				contentSize = Converter<size_t>::toValue(sizeString);
			else
				return msg;
			readFile(filename, contentSize, socket, category);
		}
		if (filename != "") {// construct message body
			msg.removeAttribute("content-length");
			std::string bodyString = "<file>" + filename + "</file>";
			std::string sizeString = Converter<size_t>::toString(bodyString.size());
			msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
			msg.addBody(bodyString);
		}
		else {// read message body
			size_t numBytes = 0;
			size_t pos = msg.findAttribute("content-length");
			if (pos < msg.attributes().size()) {
				numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
				Socket::byte* buffer = new Socket::byte[numBytes + 1];
				socket.recv(numBytes, buffer);
				buffer[numBytes] = '\0';
				std::string msgBody(buffer);
				msg.addBody(msgBody);
				delete[] buffer;
			}
		}
	}
	return msg;
}
//----< read a binary file from socket and save >--------------------
/*
 * This function expects the sender to have already send a file message,
 * and when this function is running, continuosly send bytes until
 * fileSize bytes have been sent.
 */

 // Create RepoDirectory if it does not exist.
bool ClientHandler::readFile(const std::string& filename, size_t fileSize, Socket& socket, std::string category) {
	std::cout << "\n ========>> Server Reading File:\" " << filename << "\", sent by Client\n";
	std::string fqname = "../ServerRepo/" + category + "/" + FileSystem::Path::getName(filename);
	if (!FileSystem::Directory::exists("../ServerRepo/" + category + "/")) {
		FileSystem::Directory::create("../ServerRepo/" + category + "/");
	}
	std::cout << "\n ========>> Server will Store File at location:\" " << FileSystem::Path::getFullFileSpec("../ServerRepo/" + category + "/") << "\n";
	FileSystem::File file(fqname);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood()) {
		std::cout << "\n\n  can't open file " + fqname;
		return false;
	}
	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];
	size_t bytesToRead;
	while (true) {
		if (fileSize > BlockSize)
			bytesToRead = BlockSize;
		else
			bytesToRead = fileSize;

		socket.recv(bytesToRead, buffer);
		FileSystem::Block blk;
		for (size_t i = 0; i < bytesToRead; ++i)
			blk.push_back(buffer[i]);
		file.putBlock(blk);
		if (fileSize < BlockSize)
			break;
		fileSize -= BlockSize;
	}
	file.close();
	return true;
}
//----< receiver functionality is defined by this function >---------
void ClientHandler::operator()(Socket socket) {
	while (true) {
		HttpMessage msg = readMessage(socket);
		msgQ_.enQ(msg);
		if (connectionClosed_ || msg.bodyString() == "quit") {
			std::cout << "\n\n ========>> Clienthandler thread is terminating";
			break;
		}
		std::string msgType = msg.bodyString();
		std::string category = msg.findValue("Category");
		std::cout << "\n ========>> Server received msg of type: " << msgType << "\n" << "\n ========>> Server received msg of category: " << category << "\n";
		if (msgType == "PublishFiles")
			PublishFiles(category);
		else if (msgType == "DownloadFiles")
			MsgClient_server::startSender(msgType, category);
		else if (msgType == "GetFilesList")
			MsgClient_server::startSender(msgType, category);
		else if (msgType == "GetFilesNoParentList")
			noParent(msgType, category);
		else if (msgType == "deleteSelectedFiles") {
			std::string list_of_Files = msg.findValue("FilesList");
			deleteSelectedFiles(list_of_Files, category);
		}
		else if (msgType == "LazyDownload") {
			std::string list_of_Files = msg.findValue("FilesList");
			lazyDownload(list_of_Files, category);
		}
		else if (msgType == "IISFiles") {
			std::string path = msg.findValue("path");
			std::string htmlPath = path;
			if (FileSystem::Directory::exists(path)) {
				path += "/" + category;
				if (!FileSystem::Directory::exists(path)) FileSystem::Directory::create(path);
				std::string localPath = "../PublishedFiles/" + category + "/"; std::vector<std::string> allFiles = FileSystem::Directory::getFiles(localPath);
				for (size_t i = 0; i < allFiles.size(); i++)
					FileSystem::File::copy(localPath + allFiles[i], path + "/" + allFiles[i]);
			}
			htmlPath += "/../DesignHelper";
			if (!FileSystem::Directory::exists(htmlPath)) {
				FileSystem::Directory::create(htmlPath);
				std::vector<std::string> helperFiles = FileSystem::Directory::getFiles("../DesignHelper/");
				for (size_t i = 0; i < helperFiles.size(); i++)
					FileSystem::File::copy("../DesignHelper/" + helperFiles[i], htmlPath + "/" + helperFiles[i]);
			}
			MsgClient_server::startSender(msgType, category);
		}
	}
}

//----< Selected files will be deleted as requested by user >------------------------------
void ClientHandler::deleteSelectedFiles(std::string list_of_Files, std::string category) {
	std::string path = "../PublishedFiles/" + category + "/";
	std::cout << "\n\n====>>>  Published Files on server will be deleted from location: " << FileSystem::Path::getFullFileSpec("../PublishedFiles/" + category) << "\n";
	std::cout << "\n================================================================================================================================================\n";
	std::vector<std::string> listVec = Utilities::StringHelper::split(list_of_Files);
	for (size_t i = 0; i < listVec.size() - 2; i++) {
		FileSystem::File::remove(path + listVec[i]);
	}
}

//----< Remove all files from a directory >------------------------------
void ClientHandler::removeAllFiles(std::string path) {
	std::vector<std::string> files = FileSystem::Directory::getFiles(path);
	for (size_t i = 0; i < files.size(); i++) {
		std::string file = FileSystem::Path::getFullFileSpec(path + "/" + files[i]);
		FileSystem::File::remove(file);
	}
}

//----< Function implementing lazy download feature >------------------------------
void ClientHandler::lazyDownload(std::string list_of_Files, std::string category) {

	std::vector<std::string> listVec = Utilities::StringHelper::split(list_of_Files);
	CodePublisher cp;
	size_t lastindex = listVec[0].find_last_of(".");
	std::string fileName = listVec[0].substr(0, lastindex);
	//std::string first_arg = "..\\ServerRepo\\" + category + " " + fileName +" /wf PublishedFiles_Lazy";
	removeAllFiles("../PublishedFiles_Lazy/" + category);
	std::cout << "\n\n====>>>  All the Lazy files on Server side will be at location: " << FileSystem::Path::getFullFileSpec("../PublishedFiles_Lazy/" + category) << "\n";
	std::cout << "\n================================================================================================================================================\n";
	std::string first_arg = "..\\ServerRepo\\" + category + " *.h *.cpp /wf PublishedFiles_Lazy";
	//std::cout << "\n\n\n  Arg formed==================================> " << first_arg << "\n\n";
	//cp.RemotePublisher("..\\ServerRepo *.h *.cpp /wf PublishedFiles", category);
	Show::stop();
	dep = cp.RemotePublisher(first_arg, category);
	sendLazyFiles(fileName, category);
}

//----< Function implementing NoPArent download feature >------------------------------
void ClientHandler::noParent(std::string list_of_Files, std::string category) {
	CodePublisher cp;

	std::string first_arg = "..\\ServerRepo\\" + category + " *.h *.cpp /wf PublishedFiles_Lazy";
	//std::cout << "\n\n\n  Arg formed==================================> " << first_arg << "\n\n";
	dep = cp.RemotePublisher(first_arg, category);
	sendNoParentFiles(list_of_Files, category);
}

//----< Function implementing NoPArent download feature >------------------------------
void ClientHandler::sendNoParentFiles(std::string fileName, std::string category) {
	std::string path = "../ServerRepo/" + category + "/";
	//std::string path_ToSend = "../PublishedFiles_Lazy/" + category + "/";

	path = FileSystem::Path::getFullFileSpec(path);
	std::vector<std::string> noParentFiles = FileSystem::Directory::getFiles(path);
	std::vector<std::string> files = FileSystem::Directory::getFiles(path);



	for (std::string file : files) {
		ElementDep elem = dep.value(FileSystem::Path::getFullFileSpec(path + file));
		for (auto dependentFile : elem.dependentFiles) {
			if (std::find(noParentFiles.begin(), noParentFiles.end(), FileSystem::Path::getName(dependentFile)) != noParentFiles.end()) {
				noParentFiles.erase(std::remove(noParentFiles.begin(), noParentFiles.end(), FileSystem::Path::getName(dependentFile)), noParentFiles.end());
			}

		}
	}


	std::string list_of_files;
	for (auto f : noParentFiles) {
		list_of_files += (f + ".htm") + ",";
	}
	if (noParentFiles.size() == 0) {
		list_of_files += "No Such File Exist";
	}
	list_of_files += category + "," + "GetFilesNoParentList_post";

	MsgClient_server::startSender(list_of_files, category);

}

//----< Function implementing lazy download feature >------------------------------
void ClientHandler::sendLazyFiles(std::string fileName, std::string category) {
	std::string path = "../ServerRepo/" + category + "/" + fileName;
	std::string path_ToSend = "../PublishedFiles_Lazy/" + category + "/";
	path = FileSystem::Path::getFullFileSpec(path);
	//std::vector<std::string> files = FileSystem::Directory::getFiles(path);

	std::unordered_set<std::string> lazyFiles;
	ScopeStack<std::string> s;
	s.push(path);
	while (s.size() > 0) {
		std::string file = FileSystem::Path::getName(s.pop());
		lazyFiles.insert(file);
		Keys depTablKey = dep.keys();
		for (Key key : depTablKey)
		{
			std::string name = FileSystem::Path::getName(key);
			if (name == FileSystem::Path::getName(file)) {
				std::cout << "\n ========>> Sending Lazy File Name: " << key << "\n";
				ElementDep elem = dep.value(key);
				for (auto dependentFile : elem.dependentFiles) {
					const bool is_in = lazyFiles.find(FileSystem::Path::getName(dependentFile)) != lazyFiles.end();
					if (!is_in)
						s.push(dependentFile);
				}

			}
		}
	}

	std::string list_of_files;
	for (auto f : lazyFiles) {
		list_of_files += (f + ".htm") + ",";
	}
	list_of_files += category + "," + "LazyDownload_post";

	MsgClient_server::startSender(list_of_files, category);

}
//----< Publish Files msg processing >------------------------------
void ClientHandler::PublishFiles(std::string category) {
	std::cout << "\n ========>> Message Received to Publish Files";
	std::cout << "\n ========>> Category Received: " << category << "\n\n";
	CodePublisher cp;
	std::string first_arg = "..\\ServerRepo\\" + category + " *.h *.cpp /wf PublishedFiles";
	//std::cout << "\n\n\n  Arg formed==================================> " << first_arg << "\n\n";
	//cp.RemotePublisher("..\\ServerRepo *.h *.cpp /wf PublishedFiles", category);
	std::vector<std::string> files = FileSystem::Directory::getFiles("..\\ServerRepo\\" + category);
	std::string message;
	if (files.size() == 0) {
		message = "No Files To Publish," + category + ",PublishFiles_post";
	}
	else {
		cp.RemotePublisher(first_arg, category);
		std::string path = FileSystem::Path::getFullFileSpec("..\\PublishedFiles\\" + category);
		message = path + "," + category + ",PublishFiles_post";
	}
	MsgClient_server::startSender(message, category);

}



//----< Making Server as a Client >--------------------------------------------------

class ClientCounter_server
{
public:
	ClientCounter_server() { ++clientCount; }
	size_t count() { return clientCount; }
private:
	static size_t clientCount;
};

size_t ClientCounter_server::clientCount = 0;


//----< Making Server as a Client to make message >--------------------------------------------------
HttpMessage MsgClient_server::makeMessage_server(size_t n, const std::string& body, const EndPoint& ep)
{
	HttpMessage msg;
	HttpMessage::Attribute attrib;
	EndPoint myEndPoint = "localhost:8080";  // ToDo: make this a member of the sender
											 // given to its constructor.
	switch (n)
	{
	case 1:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("POST", "Message"));
		msg.addAttribute(HttpMessage::Attribute("mode", "oneway"));
		msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
		msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));

		msg.addBody(body);
		if (body.size() > 0)
		{
			attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
			msg.addAttribute(attrib);
		}
		break;
	default:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("Error", "unknown message type"));
	}
	return msg;
}
//----< send message using socket >----------------------------------

void MsgClient_server::sendMessage_server(HttpMessage& msg, Socket& socket)
{
	std::cout << "\n\n ========>> Sending message back to Client: " << msg.bodyString() << "\n";
	std::string msgString = msg.toString();
	socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
}
//----< send file using socket >-------------------------------------
/*
* - Sends a message to tell receiver a file is coming.
* - Then sends a stream of bytes until the entire file
*   has been sent.
* - Sends in binary mode which works for either text or binary.
*/
//----< Making Server as a Client to send files >--------------------------------------------------
bool MsgClient_server::sendFile_server(const std::string& filename, Socket& socket, std::string category, std::string msgType)
{
	// assumes that socket is connected

	std::string fqname = filename;
	FileSystem::FileInfo fi(fqname);
	size_t fileSize = fi.size();
	std::string sizeString = Converter<size_t>::toString(fileSize);
	FileSystem::File file(fqname);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood())
		return false;

	HttpMessage msg = makeMessage_server(1, "", "localhost::8084");
	msg.addAttribute(HttpMessage::Attribute("file", filename));
	msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
	msg.addAttribute(HttpMessage::Attribute("Category", category));
	msg.addAttribute(HttpMessage::Attribute("msgTypeServer", msgType));
	sendMessage_server(msg, socket);
	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];
	while (true)
	{
		FileSystem::Block blk = file.getBlock(BlockSize);
		if (blk.size() == 0)
			break;
		for (size_t i = 0; i < blk.size(); ++i)
			buffer[i] = blk[i];
		socket.send(blk.size(), buffer);
		if (!file.isGood())
			break;
	}
	file.close();
	return true;
}
//----< Make a msg to send to client in response to download request >--------------------------------------------------
void MsgClient_server::makeMessageForDownloadFiles(HttpMessage& msg, std::string message, std::string category, std::string msgType) {
	msg = makeMessage_server(1, "No Files To Download", "toAddr:localhost:8084");
	msg.addAttribute(HttpMessage::Attribute("Category", category));
	msg.addAttribute(HttpMessage::Attribute("msgTypeServer", msgType));
}

//----< Make a msg to send to client in response to get files request >--------------------------------------------------
void MsgClient_server::makeMessageForGetFiles(HttpMessage& msg, std::string list_of_files, std::string category, std::string msgType) {
	msg = makeMessage_server(1, list_of_files, "toAddr:localhost:8084");
	msg.addAttribute(HttpMessage::Attribute("Category", category));
	msg.addAttribute(HttpMessage::Attribute("msgType", msgType));
}

//----< Make a msg to send to client in response to publish request >--------------------------------------------------
void MsgClient_server::makeMessageForPublishfiles(HttpMessage& msg, std::string list_of_files, std::string category, std::string msgCheck) {
	msg = makeMessage_server(1, list_of_files, "toAddr:localhost:8084");
	msg.addAttribute(HttpMessage::Attribute("Category", category));
	msg.addAttribute(HttpMessage::Attribute("msgTypeServer", msgCheck));
}


//----< this defines the behavior of the client >--------------------

void MsgClient_server::execute_server(std::string msgType, std::string category) {
	std::cout << "\n\n\n\n\n\t\t==================SERVER STARTING AS CLIENT===============================\n\n";
	try {
		SocketSystem ss; SocketConnecter si;
		while (!si.connect("localhost", sendTo_Port)) {
			std::cout << "\n ========>> Server as client waiting to connect\n"; ::Sleep(100);
		}
		HttpMessage msg; std::string path = "../PublishedFiles/" + category + "/";
		std::vector<std::string> files = FileSystem::Directory::getFiles(path);
		if (msgType == "DownloadFiles") {
			msgType += "_post";
			for (size_t i = 0; i < files.size(); ++i) {
				std::cout << "\n\n ========>> Sending File: " + files[i]; sendFile_server(path + files[i], si, category, msgType);
			}
			if (files.size() <= 0) {
				makeMessageForDownloadFiles(msg, "No Files To Download", category, msgType); sendMessage_server(msg, si);
			}
		}
		else if (msgType == "GetFilesList") {
			std::string list_of_files;
			for (size_t i = 0; i < files.size(); ++i) 
				list_of_files += files[i] + ",";
			if (list_of_files.size() == 0)
				list_of_files += "No Files Present in " + category + ",";
			msgType += "_post"; list_of_files += category + "," + msgType;
			makeMessageForGetFiles(msg, list_of_files, category, msgType);
			sendMessage_server(msg, si);
		}
		else {
			std::vector<std::string> list_of_files = ClientHandler::splitAndReplace(msgType, ",");
			std::string msgCheck = list_of_files[list_of_files.size() - 1]; std::string path = "../PublishedFiles_Lazy/" + category + "/";
			if (msgCheck == "LazyDownload_post")  // in this case msgType contains the list of files 
				for (size_t i = 0; i < list_of_files.size() - 2; i++)
					sendFile_server(path + list_of_files[i], si, category, msgCheck);
			else if (msgCheck == "PublishFiles_post") { // PublishFiles_post
				makeMessageForPublishfiles(msg, list_of_files[0], category, msgCheck); sendMessage_server(msg, si);
			}
			else if (msgCheck == "GetFilesNoParentList_post") // GetFilesNoParentList_post
				for (size_t i = 0; i < list_of_files.size() - 2; i++)
					sendFile_server(path + list_of_files[i], si, category, msgCheck);
		}
		msg = makeMessage_server(1, "quit", "toAddr:localhost:8084"); // server will always send a quit message to stop client as server
		sendMessage_server(msg, si);
	}
	catch (std::exception& exc) {
		std::cout << "\n  Exeception caught: " << "\n  " << std::string(exc.what()) + "\n\n";
	}
}

//----< make server as a client>--------------------------------------------------
void MsgClient_server::startSender(std::string msgType, std::string category) {

	//std::cout << "\n\n ========>> Sever received msg for category: " << category << "\n\n";
	MsgClient_server c1;
	std::thread thread1(
		[&]() { c1.execute_server(msgType, category); } // calling client part of the server
	);

	thread1.join();


}


//----< Making Server as a Client ---- END  >--------------------------------------------------



//----< test stub >--------------------------------------------------
// command line processing added for server
void ClientHandler::processCommandLineServerArgs(int argc, char* argv[]) {
	try {
		for (int i = 1; i < argc; ++i)
		{
			std::string argument = std::string(argv[i]);
			if (argument.compare("/listen") == 0)
			{
				std::string argument = std::string(argv[i + 1]);
				std::stringstream strListen_Port;
				strListen_Port << argument;
				strListen_Port >> listen_Port;
			}
			else if (argument.compare("/sendTo") == 0) {
				std::string argument = std::string(argv[i + 1]);
				std::stringstream strSendTo_Port;
				strSendTo_Port << argument;
				strSendTo_Port >> sendTo_Port;
			}

		}
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  command line argument parsing error:" << "\n  " << ex.what() << "\n\n";
	}

}

int main(int argc, std::string argv[])
{
	::SetConsoleTitle(L"HttpMessage Server - Runs Forever");
	std::cout << "\n\n\n\t\t<<<============================  HttpMessage Server started  ============================>>>\n\n";
	BlockingQueue<HttpMessage> msgQ;
	ClientHandler::processCommandLineServerArgs(argc, argv);

	try
	{
		{
			SocketSystem ss;
			SocketListener sl(listen_Port, Socket::IP6);
			ClientHandler cp(msgQ);
			sl.start(cp);
			/*
			* Since this is a server the loop below never terminates.
			* We could easily change that by sending a distinguished
			* message for shutdown.
			*/
			while (true)
			{
				HttpMessage msg = msgQ.deQ();
				std::cout << "\n\n\n ========>> Server Received message with body contents:\n" + msg.bodyString() << "\n";
				if (msg.bodyString() == "quit") {
					std::cout << "\n\n ========>> Server Received a quit message to start acting as a Client\n";

				}
			}

		}

	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}