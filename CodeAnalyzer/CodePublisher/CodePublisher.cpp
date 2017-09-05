/////////////////////////////////////////////////////////////////////////////
// CodePublisher.cpp - Executive for Code Publisher                        //
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
#define _SECURE_SCL_DEPRECATE 0
#include "CodePublisher.h"
#include "HtmlHelper.h"
#include "windows.h"


// returns current date-time in "yyyy-mm-dd hh-mm-ss"
const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%Y-%m-%d %H-%M-%S", &tstruct);

	return buf;
}


// process command line arguments
std::string CodePublisher::processCommandLineArgs(int argc, char* argv[], std::string pattern) {
	try {
		for (int i = 2; i < argc; ++i)
		{
			std::string argument = std::string(argv[i]);
			if (argument.compare(pattern) == 0)
			{
				std::string argument = std::string(argv[i + 1]);
				return argument;
			}

		}
		return "";
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  command line argument parsing error:" << "\n  " << ex.what() << "\n\n";
		return "invalid";
	}

}

// create body and header tag for web pages to be published
void CodePublisher::createBody(std::ofstream& out, std::string fileName, std::string htmlLoc,
	DependencyTable depTable, std::ifstream& htmlFile) {

	std::string line;
	out << "<!-" << "\n";
	out << "		" << fileName << " - HTML file for " << fileName << "\"";
	while (std::getline(htmlFile, line))
		out << line << "\n";
	using Key = std::string; // TypeName is the key
	using Keys = std::vector<Key>;
	out << "<h1>" << fileName << "</h1>" << "\n" << "<hr />\n";
	out << "<body>\n";
	out << "<div class=\"indent\">" << "\n";
	out << "<h4>Dependencies:</h4>" << "\n";
	// adding dependencies
	Keys depTablKey = depTable.keys();
	for (Key key : depTablKey)
	{
		std::string name = FileSystem::Path::getName(key);
		if (name == fileName) {
			std::cout << "\nFile Name: " << "\t\t\t" << std::setw(8) << key;
			ElementDep dep = depTable.value(key);
			for (auto dependentFile : dep.dependentFiles) {
				//out << "\"<a href =" << "\"" << htmlLoc + (FileSystem::Path::getName(dependentFile) + ".htm") << "\">" << FileSystem::Path::getName(dependentFile) << "</a>\"";
				out << "\"<a href =" << "\"" << (FileSystem::Path::getName(dependentFile) + ".htm") << "\">" << FileSystem::Path::getName(dependentFile) << "</a>\"";
				out << "\n";
			}

		}
	}
	//out << "<a href = \"Cpp11-BlockingQueue.cpp.htm\">Cpp11 - BlockingQueue.cpp </a>" << "\n";
	out << "</div>" << "\n";
	out << "<hr / > " << "\n";
	out << "<pre>" << "\n";
}
// do analysis--> use project#2 for analysis
void CodePublisher::doAnalysis(int argc, char* argv[], DependencyTable& depTable, TypeTable& typeTable
	, CodeAnalysis::CodeAnalysisExecutive& exec) {

	try {

		int iniStatus = exec.ProfessorCodeInitialization(exec, argc, argv);
		if (iniStatus == 1)
			return;

		CodeAnalysis::TypeAnal ta(typeTable, depTable);
		ta.doTypeAnal();

		ta.doDepAnal();

	}
	catch (std::exception& except)
	{
		exec.flushLogger();
		std::cout << "\n\n  caught exception in Executive::main: " + std::string(except.what()) + "\n\n";
		exec.stopLogger();
		std::cin.get();
		return;
	}

}

// mini tokenizer written to tokenize a string based on delimeters
std::vector<std::string> CodePublisher::splitAndReplace(std::string split) {

	std::vector<std::string> splitVec;

	std::stringstream stringStream(split);
	std::string line;
	while (std::getline(stringStream, line))
	{
		std::size_t prev = 0, pos;
		while ((pos = line.find_first_of(" :(", prev)) != std::string::npos)
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

// helps displaying Publisher and execute shell command to launch index page or specified file
void CodePublisher::displayPublisher(std::vector<std::string> files, size_t argc, char* argv[], std::string WebPageRepositoryLoc) {
	std::string tableOfContentHeaderFile = FileSystem::Path::getFullFileSpec("../../TableofContentHeader.htm");
	std::string tableOfContentFile = FileSystem::Path::getFullFileSpec("../../TableofContent.htm");
	std::ofstream out(tableOfContentFile);
	std::ifstream in(tableOfContentHeaderFile);
	std::string line;

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
	std::string launchPath = "..\\..\\TableofContent.htm"; // launch table of content page
	displayPattern(8);
	//std::cout << "\n Launch Arguments: --->>> \n";
	if (processCommandLineArgs(argc, argv, "/launch").compare("") != 0) {
		launchPath = WebPageRepositoryLoc + processCommandLineArgs(argc, argv, "/launch"); // launch specific file
	}
	else {
		//std::cout << "\n No Launch Arguments are specified, Considering the default Index Page: --->>> ";
	}
	std::cout << " Launching File: " << FileSystem::Path::getFullFileSpec(launchPath);
//	LPCTSTR helpFile = launchPath.c_str();
//	ShellExecute(NULL, "open", helpFile, NULL, NULL, SW_SHOWNORMAL); // shell execute
}

// process command line argument for getting web page
std::string CodePublisher::getWebPageRepo(int argc, char* argv[], std::string category) {
	Path WebPageRepositoryLoc = "../WebPagesRepository/";
	std::string checkRepoPath = processCommandLineArgs(argc, argv, "/wf");
	if (checkRepoPath.compare("") != 0) {
		WebPageRepositoryLoc = "../" + checkRepoPath + "/";
	}
	else if (checkRepoPath.compare("invalid") == 0) {
		return "";
	}
	else {

		std::cout << "\n No Arguments are specied for WebPage Repository, Considering the default Path \n";
	}
	//WebPageRepositoryLoc = FileSystem::Path::getFullFileSpec(WebPageRepositoryLoc);

	if (!FileSystem::Directory::exists(WebPageRepositoryLoc)) {
		FileSystem::Directory::create(WebPageRepositoryLoc);
	}
	WebPageRepositoryLoc += category + "/";
	std::cout << "\n All the WebPages will be Published at location: ==>>> " << WebPageRepositoryLoc << "\n";
	if (!FileSystem::Directory::exists(WebPageRepositoryLoc)) {
		FileSystem::Directory::create(WebPageRepositoryLoc);
	}
	return WebPageRepositoryLoc;
}




// check file states i.e. they can be opened or not
int CodePublisher::checkFilesState(std::ifstream& file1, std::ifstream& file2, std::ofstream& outputFile) {
	if (!file1)
	{
		std::cout << "Could not open input files" << "\n";
		return 1;
	}
	if (!file2) {
		std::cout << "Could not open input files" << "\n";
		return 1;
	}
	if (!outputFile)
	{
		std::cout << "Could not open output files" << "\n";
		return 1;
	}
	return 0;
}

// replace < and > with the required tags
void CodePublisher::replaceGTLTchars(std::string& line) {
	std::string openBraceString = "{";
	std::string closeBraceString = "}";
	std::string wordToReplace1 = "<";
	std::string wordToReplace2 = ">";
	std::string smallerThan = "&lt;";
	std::string greaterThan = "&gt;";
	while (true) {
		size_t sml = line.find("<");
		if (sml != std::string::npos)
			line.replace(sml, wordToReplace1.length(), smallerThan);
		size_t grt = line.find(">");
		if (grt != std::string::npos)
			line.replace(grt, wordToReplace2.length(), greaterThan);

		sml = line.find("<");
		grt = line.find(">");
		if (sml == std::string::npos && grt == std::string::npos)
			break;
	}
}

// check if div tag needs to be inserted for implementing hiding and unhiding functionality
void CodePublisher::checkDiv(bool& divOpen, std::string& line, TypeTable& typeTable) {
	if (line.size() != 0) {
		if (line.find("if ") != std::string::npos) return;
		if (line.find("while ") != std::string::npos) return;
		if(line.find("for ") != std::string::npos) return;
		if (line.find("if(") != std::string::npos) return;
		if (line.find("while(") != std::string::npos) return;
		if (line.find("for(") != std::string::npos) return;
		for (auto tok : splitAndReplace(line)) {
			if (typeTable.containsKey(tok))
			{
				for (auto elem : typeTable.value(tok))
				{
					std::string checkClass = elem.type.getValue();
					if (checkClass.compare("class") == 0 || checkClass.compare("function") == 0 || checkClass.compare("struct") == 0)
						divOpen = true;
				}
			}
		}

	}
}

// insert div tag for implementing hiding and unhiding functionality
void CodePublisher::insertDivClassTag(bool& divOpen, std::string& line, int& count, std::ofstream& out, int& divOpenCount) {
	HtmlHelper htmlHelper;
	std::string openBraceString = "{";
	size_t openBracePos = line.find(openBraceString);
	if (openBracePos != std::string::npos) {
		if (line.find("if ") != std::string::npos) return;
		if (line.find("while ") != std::string::npos) return;
		if (line.find("for ") != std::string::npos) return;
		if (line.find("if(") != std::string::npos) return;
		if (line.find("while(") != std::string::npos) return;
		if (line.find("for(") != std::string::npos) return;
		if (line.find("stack_Brace.push") != std::string::npos) return;
		std::string divId = "div" + std::to_string(++count);
		std::string btnId = "button" + std::to_string(count);
		htmlHelper.createButton(btnId, divId, out);
		std::string replaceToString = "{ <div id=\"" + divId + "\">";
		line.replace(openBracePos, openBraceString.length(), replaceToString);
		divOpenCount++;
		divOpen = false;
	}
}

// helps publishing file.. from this function 
int CodePublisher::publishFiles(Files filesCPP, Files& list_of_HtmlFiles, File WebPageRepositoryLoc, DependencyTable& depTable, TypeTable& typeTable) {
	std::stack<std::string> stack_Brace;
	//	HtmlHelper htmlHelper;
	int count = 0;
	int divOpenCount = 0;
	std::string openBraceString = "{", closeBraceString = "}";
	bool divOpen = false, divInserted = false;
	for (std::string inputFile : filesCPP) {
		std::string htmlF = FileSystem::Path::getFullFileSpec("../../") + "htmlContent.htm", output = WebPageRepositoryLoc + FileSystem::Path::getName(inputFile) + ".htm";
		std::ifstream htmlFile(htmlF), in(inputFile);
		std::ofstream out(output);
		std::string line;
		{
			if (checkFilesState(in, htmlFile, out) == 1)
				return 1;
			list_of_HtmlFiles.push_back(output);
			createBody(out, FileSystem::Path::getName(inputFile), WebPageRepositoryLoc, depTable, htmlFile);
			while (std::getline(in, line)) {
				replaceGTLTchars(line); // replace greater than and smaller than symbols to make them html compatible
				checkDiv(divOpen, line, typeTable); // check if divTag needs to be inserted
				if (divOpen)
					insertDivClassTag(divOpen, line, count, out, divOpenCount); // insert div class tag to implement hiding and unhiding
				if (divOpenCount > 0) {
					size_t openBracePos = line.find(openBraceString), closeBracePos = line.find(closeBraceString), openPos = line.find("openBraceString"), closePos = line.find("closeBraceString"), replacePos = line.find("replaceToString"), stackPos = line.find("stack_Brace");
					if (openBracePos != std::string::npos) {
						if (!(openPos != std::string::npos || replacePos != std::string::npos || stackPos != std::string::npos))
							stack_Brace.push("{");
					}
					if (stack_Brace.size() == divOpenCount) {
						std::string replaceToString = "</div> }";
						if (closeBracePos != std::string::npos) {
							if (!(closePos != std::string::npos || replacePos != std::string::npos || stackPos != std::string::npos)) {
								line.replace(closeBracePos, closeBraceString.length(), replaceToString);
								divOpenCount--;
							}
						}
					}
					if (closeBracePos != std::string::npos)
						if (!(closePos != std::string::npos || replacePos != std::string::npos || stackPos != std::string::npos))
							stack_Brace.pop();
				}
				out << line << "\n";
			}
			out << "</pre>" << "\n" << "</body>" << "\n" << "</html>";
		}
	}
	return 0;
}

void CodePublisher::displayPattern(int reqNumber) {
	std::cout << "\n\n\n================================================================================================================================================";
	std::cout << "\n  Displaying Requirement " << reqNumber << ": =============>";
	std::cout << "\n================================================================================================================================================\n";
}

// displays all the requirements
void CodePublisher::displayRequirements() {
	displayPattern(1);
	std::cout << "\n  Used Visual Studio 2015 and its C++ Windows console projects, as provided in the ECS computer labs--> check code for the same\n";

	displayPattern(2);
	std::cout << "\n  use the C++ standard library's streams for all console I/O and new and delete for all heap-based memory management--> check code for the same\n";

	displayPattern(3);
	std::cout << "\n Provided a Publisher--> that create web Pages of all *.h and *.cpp files.. \n";
	std::cout << " Dont give \"launch\" argument on the command line to view a index page containing all requested web pages! \n";

	displayPattern(4);
	std::cout << "\n Provided the facility to expand or collapse class bodies, methods, and global functions using JavaScript and CSS properties \n";
	std::cout << " Launch an index page web page(by not giving any command line parameter) OR give specific file name on the command line (using /launch \"Filename.htm\") to verify the same \n";

	displayPattern(5);
	std::cout << "\n Provide a CSS style sheet that the Publisher uses to style its generated pages and a JavaScript file that provides functionality to hide and unhide sections of code for outlining, using mouse clicks. \n";
	std::cout << "\n For Verification-- check File: " << FileSystem::Path::getFullFileSpec("../DesignHelper/HideUnhide.js") << "\n";
	std::cout << "\n                                " << FileSystem::Path::getFullFileSpec("../DesignHelper/cssStyle.js") << "\n";

	displayPattern(6);
	std::cout << "\n On each web page head section there is a link to the css style sheet and javscript file, this can be verified by viewing source on the browser of any web page \n";
	std::cout << "\n Links are given of Files: " << FileSystem::Path::getFullFileSpec("../DesignHelper/HideUnhide.js") << "\n";
	std::cout << "\n                                " << FileSystem::Path::getFullFileSpec("../DesignHelper/cssStyle.js") << "\n";

	displayPattern(7);
	std::cout << "\n Project #2 has been used to find dependencies and HTML5 links are embedded on the web Pages \n";
	std::cout << "\n Check Code to verify --> that Project #2 has been used " << "\n";
	std::cout << "\n Check Web pages to verify the embedded links to the dependecies " << "\n";

	displayPattern(9);
	std::cout << "\n Code Publisher functionality has been displayed for important packages in the Test Files \n";
	std::cout << "\n All the cpp and .h files in Packages- CodePublisher, TypeTable, DependencyTable and Abstract Syntax tree will be published  " << "\n";
	std::cout << "\n Launch an index page web page (by not giving any command line parameter) to view test files and important files " << "\n";

	displayPattern(10);
	std::cout << "\n An automated test unit test suite has been included to meet all requirements \n";
	std::cout << "\n For Verification-- check CodePublisher.cpp (Executive for Project#3) in the CodePublisher Project " << "\n";
}

DependencyTable CodePublisher::RemotePublisher(std::string commandLineString, std::string category) {
	std::vector<std::string> list_of_HtmlFiles;
	
	CodeAnalysis::CodeAnalysisExecutive exec;

	Scanner::Toker toker_;
	DependencyTable depTable;
	TypeTable typeTable;
	
	std::vector<std::string> commandLineVector = splitAndReplace(commandLineString);
	int size = commandLineVector.size();
	char** commandLineArray = new char*[size]; // preparing command line args
	for (int i = 0; i < size; i++) {
		char * writable = new char[commandLineVector.at(i).size() + 1];
		std::copy(commandLineVector.at(i).begin(), commandLineVector.at(i).end(), writable);
		writable[commandLineVector.at(i).size()] = '\0'; // 

		commandLineArray[i+1] = writable;
		std::cout << "\nArguments: " << commandLineArray[i + 1];
	}

	std::cout << "\n\n\n================================================================================================================================================";
	//displayPattern(8);
	Path WebPageRepositoryLoc = getWebPageRepo(commandLineVector.size() + 1, commandLineArray, category);
	if (WebPageRepositoryLoc.compare("invalid") == 0) {
		std::cout << "Invalid Repository to Store Files\n";
		/*return 1;*/
	}

	doAnalysis(commandLineVector.size()+1, commandLineArray, depTable, typeTable, exec); // Type Table creation + Dependency Analysis
	Files filesHeader = exec.cppHeaderFiles();
	Files filesCPP = exec.cppImplemFiles();
	filesCPP.insert(filesCPP.end(), filesHeader.begin(), filesHeader.end());
	std::cout << "\n\n================================     Files To Be Published     ================================" << "\n";
	publishFiles(filesCPP, list_of_HtmlFiles, WebPageRepositoryLoc, depTable, typeTable);
	//displayRequirements();
    displayPublisher(list_of_HtmlFiles, commandLineVector.size()+1, commandLineArray, WebPageRepositoryLoc);
	//delete[] commandLineArray;
	return depTable;
}


#ifdef CodePublisher
// Executive for Code Publisher
int main(int argc, char* argv[]) {
	std::cout << FileSystem::Path::getFullFileSpec(argv[1]) << "\n";
	std::cout << "\n\n\n================================================================================================================================================";
	std::cout << "\n======>>>> NOTE: For Command Line Argumets-->";
	std::cout << "\n--Files to test-- Give Path, include a space, and type either or all [*.cpp, *.h], as Analysis can be done on these \n";
	std::cout << "\n--Use /wf \"Repository Name\" else Default Repository will be used \n--USe /launch \"Specific Web Page Name, else Index Page will open\n";

	CodePublisher cp;
	cp.RemotePublisher("..\\TA-TEST-Project-2 *.h *.cpp");
	std::cin.get();
}
#endif // CodePublisher