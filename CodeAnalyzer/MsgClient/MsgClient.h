#pragma once
/////////////////////////////////////////////////////////////////////////////
// MsgClient.h - Represents a Client                                       //
//																		   //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Pranjul Arora, 2017                                         //
// All rights granted provided that this notice is retained                //	
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2015                             //
// Platform:    Mac Book Pro, Core i5, MAC OS                              //
// Application: Project 2, CSE687 - Object Oriented Design                 //
// Author:      Pranjul Arora, Syracuse University                         //
//              (315) 949 9061, parora01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* -------------------
* This package acts a client
*
*
* Public Interface:
* -----------------
* std::string execute1(std::string msgFromGUI);
* - This function makes a call to server and send messages and files
* static void startReceiver();
* - This function makes client as server
*
*	void publishDownloadFiles(std::string msg);
* - This function helps publishing downloaded files
* 	void createIndexPage(std::vector<std::string> files);
* - This function creates a index page to display
*
*
* Required Files:
* ---------------
* HttpMessage.h
* Cpp11-BlockingQueue.h
* Sockets.h
* FileSystem.h
* Logger.h
* Utilities.h"
*
* Build Process:
* --------------
* devenv CodeAnalyzer.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 3rd May 2017
* - first release
*
*/


#include "../HttpMessage/HttpMessage.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include <string>
#include <iostream>
#include <thread>

using namespace Logging;
using Show = StaticLogger<1>;
using namespace Utilities;
using Utils = StringHelper;

/////////////////////////////////////////////////////////////////////
// ClientCounter creates a sequential number for each client
//

/////////////////////////////////////////////////////////////////////
// MsgClient class
// - was created as a class so more than one instance could be 
//   run on child thread
//
class MsgClient
{
public:
	using EndPoint = std::string;
	//void execute(const size_t TimeBetweenMessages, const size_t NumMessages);
	std::string execute1(std::string msgFromGUI);
	static void startReceiver();
	void publishDownloadFiles(std::string msg);
	void createIndexPage(std::vector<std::string> files, std::string path);
private:
	///// My Code /////
	std::vector<std::string> getMessageTypeAndCategory(std::string& msgType, std::string& category, std::string msgFromGUI);
	void downloadFilesMessage(HttpMessage& msg, std::string msgType, std::string category);
	void getFilesMessage(HttpMessage& msg, std::string msgType, std::string category);
	void publishFilesMessage(HttpMessage& msg, std::string msgType, std::string category);
	void deleteSelectedFilesMessage(HttpMessage& msg, std::string msgType, std::string category, std::string filesList);
	void lazyDownloadMessage(HttpMessage& msg, std::string msgType, std::string category, std::string filesList);
	void iisMessage(HttpMessage& msg, std::string msgType, std::string category, std::string path);
	void displayFileMessage(std::string category, std::string path);


	// mini tokenizer written to tokenize a string based on delimeters
	void removeAllFiles(std::string path);
	void callIndexForLazyDownload(std::string path);
	void callIndexForIISDownload(std::string path);
	std::vector<std::string> splitAndReplace(std::string, std::string pattern);

	///// My Code /////
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
	void sendMessage(HttpMessage& msg, Socket& socket);
	bool sendFile(const std::string& fqname, Socket& socket, std::string category);
};