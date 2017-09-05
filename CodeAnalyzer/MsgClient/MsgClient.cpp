/////////////////////////////////////////////////////////////////////////
// MsgClient.cpp - Demonstrates simple one-way HTTP messaging          //
//                                                                     //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016           //
// Application: OOD Project #4                                         //
// Platform:    Visual Studio 2015, Dell XPS 8900, Windows 10 pro      //
/////////////////////////////////////////////////////////////////////////
/*
 * This package implements a client that sends HTTP style messages and
 * files to a server that simply displays messages and stores files.
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
  * - pull the sending parts into a new Sender class
  * - You should create a Sender like this:
  *     Sender sndr(endPoint);  // sender's EndPoint
  *     sndr.PostMessage(msg);
  *   HttpMessage msg has the sending adddress, e.g., localhost:8080.
  */
  //#include "../HttpMessage/HttpMessage.h"
  //#include "../Logger/Cpp11-BlockingQueue.h"
  //#include "../Sockets/Sockets.h"
  //#include "../FileSystem/FileSystem.h"
  //#include "../Logger/Logger.h"
  //#include "../Utilities/Utilities.h"
  //#include <string>
  //#include <iostream>
  //#include <thread>
#define _SECURE_SCL_DEPRECATE 0
#include "MsgClient.h"
#include <windows.h>
#include <ShellAPI.h>


using namespace Logging;
using Show = StaticLogger<1>;
using namespace Utilities;
using Utils = StringHelper;

static std::string files_list;

//----< factory for creating messages >------------------------------
/*
 * This function only creates one type of message for this demo.
 * - To do that the first argument is 1, e.g., index for the type of message to create.
 * - The body may be an empty string.
 * - EndPoints are strings of the form ip:port, e.g., localhost:8081. This argument
 *   expects the receiver EndPoint for the toAddr attribute.
 */
HttpMessage MsgClient::makeMessage(size_t n, const std::string& body, const EndPoint& ep)
{
	HttpMessage msg;
	HttpMessage::Attribute attrib;
	EndPoint myEndPoint = "localhost:8081";  // ToDo: make this a member of the sender
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

void MsgClient::sendMessage(HttpMessage& msg, Socket& socket)
{
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
bool MsgClient::sendFile(const std::string& filename, Socket& socket, std::string category)
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

	HttpMessage msg = makeMessage(1, "", "localhost::8080");
	msg.addAttribute(HttpMessage::Attribute("file", filename));
	msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
	msg.addAttribute(HttpMessage::Attribute("Category", category));
	sendMessage(msg, socket);
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
//----< this defines the behavior of the client >--------------------
// change execute1 to execute after doing all required

//----< Create message for download files button >------------------------------
void MsgClient::downloadFilesMessage(HttpMessage& msg, std::string msgType, std::string category) {
	std::cout << "\n\n====>>>Client received Message to:  " << msgType << "\n\n";
	msg = makeMessage(1, msgType, "toAddr:localhost:8080");
	msg.addAttribute(HttpMessage::Attribute("Category", category));
	std::cout << "\n\n====>>>  sending Message to Server to Download Files from category: " << category << "\n";
}

//----< Create message for Get files button >------------------------------
void MsgClient::getFilesMessage(HttpMessage& msg, std::string msgType, std::string category) {
	std::cout << "\n\n====>>>Client received Message to:  " << msgType << "\n\n";
	msg = makeMessage(1, msgType, "toAddr:localhost:8080");
	msg.addAttribute(HttpMessage::Attribute("Category", category));
	std::cout << "\n\n====>>>  sending Message to Server to Get List of Files from category: " << category << "\n";
}

//----< Create message for Publish files button >------------------------------
void MsgClient::publishFilesMessage(HttpMessage& msg, std::string msgType, std::string category) {
	std::cout << "\n\n====>>>Client received Message to:  " << msgType << "\n\n";
	msg = makeMessage(1, msgType, "toAddr:localhost:8080");
	msg.addAttribute(HttpMessage::Attribute("Category", category));
	std::cout << "\n\n====>>>  sending Message to Server to Publish Files of category: " << category << "\n";

}

//----< Create message for Delete files button >------------------------------
void MsgClient::deleteSelectedFilesMessage(HttpMessage& msg, std::string msgType, std::string category, std::string filesList) {
	std::cout << "\n\n====>>>Client received Message to:  " << msgType << "\n\n";
	msg = makeMessage(1, msgType, "toAddr:localhost:8080");
	msg.addAttribute(HttpMessage::Attribute("Category", category));
	msg.addAttribute(HttpMessage::Attribute("FilesList", filesList));
	std::cout << "\n\n====>>>  sending Message to Server to Deleted Selected Files of category: " << category << "\n";
}

//----< Create message for Lazy Download files button >------------------------------
void MsgClient::lazyDownloadMessage(HttpMessage& msg, std::string msgType, std::string category, std::string filesList) {
	std::cout << "\n\n====>>>Client received Message to:  " << msgType << "\n\n";
	removeAllFiles("../LazyDownload/" + category);
	std::cout << "\n\n====>>>  All the Lazy files will be downloaded on Client side at location: " << FileSystem::Path::getFullFileSpec("../LazyDownload/" + category) << "\n";
	std::cout << "\n================================================================================================================================================\n";
	msg = makeMessage(1, msgType, "toAddr:localhost:8080");
	msg.addAttribute(HttpMessage::Attribute("Category", category));
	msg.addAttribute(HttpMessage::Attribute("FilesList", filesList));
	std::cout << "\n\n====>>>  sending Message to Server to Lazy Download the Selected file of category: " << category << "\n";
}

//----< Create message for IIS button >------------------------------
void MsgClient::iisMessage(HttpMessage& msg, std::string msgType, std::string category, std::string path) {
	std::cout << "\n\n====>>>Client received Message to:  " << msgType << "\n\n";
	removeAllFiles("../LazyDownload/" + category);
	msg = makeMessage(1, msgType, "toAddr:localhost:8080");
	msg.addAttribute(HttpMessage::Attribute("Category", category));
	msg.addAttribute(HttpMessage::Attribute("path", path));
	std::cout << "\n\n====>>>  sending Message to Server for IIS Download from the Selected file of category: " << category << "\n";
}

//----< Display a particular file >------------------------------
void MsgClient::displayFileMessage(std::string category, std::string path) {
	std::cout << "\n\n====>>>Client received Message to Display Files " << "\n";
	std::cout << "\n================================================================================================================================================\n";
	std::string launchPath = "../DownloadedFiles/" + category + "/" + path;
	std::cout << "\n\n====>>>  Local Directory will be traversed to fetch files: Directory==> " << FileSystem::Path::getFullFileSpec("../DownloadedFiles/" + category) << "\n";
	launchPath = FileSystem::Path::getFullFileSpec(launchPath);
	if (FileSystem::File::exists(launchPath)) {
		std::cout << "\n====>>>Launching the File\n";
		LPCSTR helpFile = launchPath.c_str();
		ShellExecuteA(NULL, "open", helpFile, NULL, NULL, SW_SHOWNORMAL); // shell execute
	}
	else
		std::cout << "\n==============>>>>  File Does Not Exist, Please Download the File First\n";

}

//----< get Messages Type And Category >------------------------------
std::vector<std::string> MsgClient::getMessageTypeAndCategory(std::string& msgType, std::string& category, std::string msgFromGUI) {
	std::vector<std::string> msgVectorFromGUI = splitAndReplace(msgFromGUI, ",");
	msgType = msgVectorFromGUI[msgVectorFromGUI.size() - 1];
	category = msgVectorFromGUI[msgVectorFromGUI.size() - 2];
	return msgVectorFromGUI;
}

//----< Function that Client uses to communicate to server >------------------------------
std::string MsgClient::execute1(std::string msgFromGUI) {
	std::string result = "", msgType, category;
	try {
		SocketSystem ss; SocketConnecter si;
		while (!si.connect("localhost", 8080)) { std::cout << "\n Client Waiting to connect..."; ::Sleep(100); }
		std::vector<std::string> msgVectorFromGUI = getMessageTypeAndCategory(msgType, category, msgFromGUI); HttpMessage msg;
		if (msgType == "SendFiles") {
			std::cout << "\n ======>>> Client received Message to: " << msgType << "\n\n";
			std::vector<std::string> files; files.assign(msgVectorFromGUI.begin(), msgVectorFromGUI.end() - 1);
			for (size_t i = 0; i < files.size() - 1; ++i)
				if (FileSystem::Path::getExt(files[i]) == "h" || FileSystem::Path::getExt(files[i]) == "cpp") {
					std::cout << "\n =====>>>>>  Sending File: " + files[i]; sendFile(files[i], si, category);
				}
		}
		else if (msgType == "DownloadFiles") {
			downloadFilesMessage(msg, msgType, category);
			sendMessage(msg, si); MsgClient::startReceiver(); result = files_list; publishDownloadFiles(result);
		}
		else if (msgType == "GetFilesList") {
			getFilesMessage(msg, msgType, category); sendMessage(msg, si); MsgClient::startReceiver(); result = files_list;
		}
		else if (msgType == "GetFilesNoParentList") {
			removeAllFiles("../NoParentFiles/" + category); msg = makeMessage(1, msgType, "toAddr:localhost:8080");
			msg.addAttribute(HttpMessage::Attribute("Category", category)); sendMessage(msg, si); MsgClient::startReceiver();
			result = files_list; callIndexForLazyDownload("../NoParentFiles/" + msg.findValue("Category"));
		}
		else if (msgType == "PublishFiles") {
			publishFilesMessage(msg, msgType, category); sendMessage(msg, si); MsgClient::startReceiver(); result = files_list;
		}
		else if (msgType == "deleteSelectedFiles") {
			deleteSelectedFilesMessage(msg, msgType, category, msgFromGUI); sendMessage(msg, si);
		}
		else if (msgType == "LazyDownload") {
			lazyDownloadMessage(msg, msgType, category, msgFromGUI); sendMessage(msg, si);
			MsgClient::startReceiver(); callIndexForLazyDownload("../LazyDownload/" + msg.findValue("Category"));
		}
		else if (msgType == "IISFiles") {
			iisMessage(msg, msgType, category, msgVectorFromGUI[0]); sendMessage(msg, si); 
			MsgClient::startReceiver(); callIndexForIISDownload(msgVectorFromGUI[0] + "/" + category);
		}
		else if (msgType == "DisplayFile")
			displayFileMessage(category, msgVectorFromGUI[0]);
		files_list = ""; msgVectorFromGUI.clear();
		return result;
	}
	catch (std::exception& exc) { std::cout << "n  Exeception caught: \n" << std::string(exc.what()); }
	return result;
}

//----< creates an index page for IIS download feature >------------------------------
void MsgClient::callIndexForIISDownload(std::string path) {
	if (FileSystem::Directory::exists(path)) {
		std::vector<std::string> fileNamesVec = FileSystem::Directory::getFiles(path);
		std::vector<std::string> files;
		for (size_t i = 0; i < fileNamesVec.size(); i++) {
			std::string val = path + "/" + fileNamesVec[i];
			val = FileSystem::Path::getFullFileSpec(val);
			files.push_back(val);
		}
		createIndexPage(files, path + "/Index.htm");
	}
}

//----< creates an index page for lazy download feature >------------------------------
void MsgClient::callIndexForLazyDownload(std::string path) {
	std::cout << "\n\n====>>>  The requested Files will be Downloaded on the client at Location: " << FileSystem::Path::getFullFileSpec(path) << "\n";
	std::cout << "\n================================================================================================================================================\n";
	if (FileSystem::Directory::exists(path)) {
		std::vector<std::string> fileNamesVec = FileSystem::Directory::getFiles(path);
		std::vector<std::string> files;
		for (size_t i = 0; i < fileNamesVec.size(); i++) {
			std::string val = path + "/" + fileNamesVec[i];
			val = FileSystem::Path::getFullFileSpec(val);
			files.push_back(val);
		}
		createIndexPage(files, "../../TableofContent.htm");
	}
}

//----< creates an index page>------------------------------
void MsgClient::createIndexPage(std::vector<std::string> files, std::string path) {
	std::string tableOfContentHeaderFile = FileSystem::Path::getFullFileSpec("../../TableofContentHeader.htm");
	std::string tableOfContentFile = FileSystem::Path::getFullFileSpec(path);
	std::ofstream out(tableOfContentFile);
	std::ifstream in(tableOfContentHeaderFile);
	std::string line;
	// copying the common header and filling dependencies
	while (std::getline(in, line)) {
		if (!in)
		{
			std::cout << "Could not open file for reading: " << tableOfContentHeaderFile << "\n";
			return;
		}
		if (!out)
		{
			std::cout << "Could not open tableOfContentFile: " << tableOfContentFile + ".htm" << "\n";
			return;
		}
		size_t tableTag = line.find("</table>");
		if (tableTag != std::string::npos) {
			for (auto file : files) {
				out << "<tr>\n";
				out << "<td><a href= \"" << file << "\">" << FileSystem::Path::getName(file) << "</a></td>\n";
				out << "</tr>\n";
			}
		}
		out << line;
	}

	out.close();
	in.close();
	std::string launchPath = path; // launch table of content page
	launchPath = FileSystem::Path::getFullFileSpec(launchPath);
	std::cout << " Launching File: " << FileSystem::Path::getFullFileSpec(launchPath);
	LPCSTR helpFile = launchPath.c_str();
	ShellExecuteA(NULL, "open", helpFile, NULL, NULL, SW_SHOWNORMAL); // shell execute
}

//----< Publish Downloaded Files >------------------------------
void MsgClient::publishDownloadFiles(std::string msg) {
	std::vector<std::string> pathVec = splitAndReplace(msg, ",");
	std::string path = pathVec[0];
	if (FileSystem::Directory::exists(path)) {
		std::vector<std::string> fileNamesVec = FileSystem::Directory::getFiles(path);
		std::vector<std::string> files;
		for (size_t i = 0; i < fileNamesVec.size(); i++) {
			std::string val = path + "/" + fileNamesVec[i];
			files.push_back(val);
		}
		createIndexPage(files, "../../TableofContent.htm");
	}
}

//----< Remove all files from a directory >------------------------------
void MsgClient::removeAllFiles(std::string path) {
	std::vector<std::string> files = FileSystem::Directory::getFiles(path);
	for (size_t i = 0; i < files.size(); i++) {
		std::string file = FileSystem::Path::getFullFileSpec(path + "/" + files[i]);
		FileSystem::File::remove(file);
	}
}

// mini tokenizer written to tokenize a string based on delimeters
std::vector<std::string> MsgClient::splitAndReplace(std::string split, std::string pattern) {
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


////// CLient as receiver  
class ClientHandler_Client
{
public:

	ClientHandler_Client(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void operator()(Socket socket);

private:

	bool connectionClosed_;
	HttpMessage readMessage_Client(Socket& socket);
	bool readFile_Client(const std::string& filename, size_t fileSize, Socket& socket, std::string category, std::string msgTypeServer);
	BlockingQueue<HttpMessage>& msgQ_;
};

//----< Read messages sent by the server >------------------------------
HttpMessage ClientHandler_Client::readMessage_Client(Socket& socket) {
	connectionClosed_ = false; HttpMessage msg;// read message attributes
	while (true) {
		std::string attribString = socket.recvString('\n');
		if (attribString.size() > 1) {
			HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);
			msg.addAttribute(attrib);
		}
		else
			break;
	}
	if (msg.attributes().size() == 0) {	// If client is done, connection breaks and recvString returns empty string
		connectionClosed_ = true; return msg;
	}
	if (msg.attributes()[0].first == "POST") {// read body if POST - all messages in this demo are POSTs
		std::string filename = msg.findValue("file");// is this a file message?
		if (filename != "") {
			size_t contentSize;
			std::string sizeString = msg.findValue("content-length");
			std::string category = msg.findValue("Category");
			std::string msgTypeServer = msg.findValue("msgTypeServer");
			if (sizeString != "")
				contentSize = Converter<size_t>::toValue(sizeString);
			else
				return msg;
			readFile_Client(filename, contentSize, socket, category, msgTypeServer);
		}

		if (filename != "") {// construct message body
			msg.removeAttribute("content-length");
			std::string bodyString = "<file>" + filename + "</file>"; std::string sizeString = Converter<size_t>::toString(bodyString.size());
			msg.addAttribute(HttpMessage::Attribute("content-length", sizeString)); msg.addBody(bodyString);
		}
		else {	// read message body
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
//----< save files sent by the server >------------------------------
bool ClientHandler_Client::readFile_Client(const std::string& filename, size_t fileSize, Socket& socket, std::string category, std::string msgTypeServer) {
	std::string dirName;
	std::cout << "\n ========>> Client Reading File:\" " << filename << "\", sent by Server\n";
	if (msgTypeServer == "DownloadFiles_post") {
		dirName = "../DownloadedFiles/";
	}
	else if (msgTypeServer == "LazyDownload_post") {
		dirName = "../LazyDownload/";
	}
	else if (msgTypeServer == "GetFilesNoParentList_post") {
		dirName = "../NoParentFiles/";
	}
	if (!FileSystem::Directory::exists(dirName)) {
		FileSystem::Directory::create(dirName);
	}
	files_list = FileSystem::Path::getFullFileSpec(dirName + category) + "," + msgTypeServer;
	std::string fqname = dirName + category + "/" + FileSystem::Path::getName(filename);
	if (!FileSystem::Directory::exists(dirName + category + "/")) {
		FileSystem::Directory::create(dirName + category + "/");
	}
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

void ClientHandler_Client::operator()(Socket socket) {
	while (true) {
		HttpMessage msg = readMessage_Client(socket);
		msgQ_.enQ(msg);
		if (connectionClosed_ || msg.bodyString() == "quit") {
			std::cout << "\n\n ========>> (Client as a Server) handler thread is terminating";
			break;
		}
		std::string msgType = msg.findValue("msgType");
		std::string msgTypeServer = msg.findValue("msgTypeServer");
		std::string category = msg.findValue("Category");
		std::cout << "\n ========>> Client as Server received msg of type: " << msgType << "\n";
		std::cout << "\n ========>> Client as Server received msg of category: " << category << "\n";
		if (msgType == "GetFilesList_post") {
			files_list = msg.bodyString();
		}
		else if (msgTypeServer == "DownloadFiles_post") {
			files_list += msg.bodyString() + "," + msgTypeServer;
		}
		else if (msgTypeServer == "PublishFiles_post") {
			files_list += msg.bodyString() + "," + msgTypeServer;
		}
	}
}
////////////////////////////------ Receive End
////////////////////////////------ Start Receiver
//----< Making Client as server >------------------------------
void MsgClient::startReceiver() {
	std::cout << "\n\n\n\n\t\t==================CLIENT BECOMING SERVER===============================\n\n";
	BlockingQueue<HttpMessage> msgQ_clientReceiver;
	try
	{
		SocketSystem ss;
		SocketListener sl(8084, Socket::IP6);
		ClientHandler_Client cp(msgQ_clientReceiver);
		sl.start(cp);
		while (true)
		{
			HttpMessage msg = msgQ_clientReceiver.deQ();
			Show::write("\n\n ========>> Client recieved message from Server with BODY contents:\n" + msg.bodyString());

			if (msg.bodyString() == "quit") {
				std::cout << "\n\n ========>> Client received message to quit its server functionality\n";
				sl.close();
				break;
			}
		}
	}
	catch (std::exception& exc) {
		std::cout << "\n  Exeception caught: \n " << std::string(exc.what()) + "\n\n";
	}
}
/////// End of start server functiono


// Test Stub
#ifdef ClientFunctionality
int main()
{
	::SetConsoleTitle(L"Clients Running on Threads");
	std::cout << "Demonstrating two HttpMessage Clients each running on a child thread\n";
	Show::title("Demonstrating two HttpMessage Clients each running on a child thread");
	{

		std::thread t1(
			[&]() { c1.execute1("SendFiles", "../TestFiles"); }
		);

		t1.join();

	}


	std::cin.get();
}
#endif // ClientFunctionality