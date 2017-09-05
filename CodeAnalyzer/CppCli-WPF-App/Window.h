#ifndef WINDOW_H
#define WINDOW_H
///////////////////////////////////////////////////////////////////////////
// Window.h - C++\CLI implementation of WPF Application                  //
//          - Demo for CSE 687 Project #4                                //
// ver 4.0																 //
// Author: Pranjul Arora												 //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015     //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package demonstrates how to build a C++\CLI WPF application.  It
*  provides one class, WPFCppCliDemo, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class MockChannel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*  The window class hosts, in its window, a tab control with three views, two
*  of which are provided with functionality that you may need for Project #4.
*  It loads the DLL holding MockChannel.  MockChannel hosts a send queue, a
*  receive queue, and a C++11 thread that reads from the send queue and writes
*  the deQ'd message to the receive queue.
*
*  The Client can post a message to the MockChannel's send queue.  It hosts
*  a receive thread that reads the receive queue and dispatches any message
*  read to a ListBox in the Client's FileList tab.  So this Demo simulates
*  operations you will need to execute for Project #4.
*
*  Required Files:
*  ---------------
*  Window.h, Window.cpp, MochChannel.h, MochChannel.cpp,
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*  - this builds C++\CLI client application and native mock channel DLL
*
* * Public Interface:
*  -----------------
* void displayRequirement1();
* - This function helps defining Requirement
* void displayRequirement2(); 
* - This function helps defining Requirement
* void displayRequirement3(); 
* - This function helps defining Requirement
* void displayRequirement4(); 
* - This function helps defining Requirement
* void displayRequirement5();
* - This function helps defining Requirement
* void displayRequirement6();
* - This function helps defining Requirement
* void displayRequirement7();
* - This function helps defining Requirement
* void displayRequirement8();
* - This function helps defining Requirement
* void displayRequirement9();
* - This function helps defining Requirement
* void displayRequirement10();
* - This function helps defining Requirement
* void displayRequirement11();
* - This function helps defining Requirement
*
* void displayRequirements(); // displays all the requirements
* - This function helps defining Requirement
*
* void displayPattern(int);
* - This function helps defining Requirement
*
* void setUpStatusBar();
* - This function set up status bar
* 
* void setUpTabControl();
* - This function set up status bar
* void setUpPublishView();
* This function set up Publish
* void setUpSendFileListViewProperties();
* - This function set up Publish view property
* void setUpSendFileListView();
* - This function set up send view property
* void setUpDownloadViewProperties();
* - This function set up download view property
* void setUpDownloadView();
* - This function set up download  property
* void setUpDeleteViewProperties();
* - This function set up delete view property
* void setUpDeleteView();
* - This function set up delete view 
* void setUpLazyDownloadViewProperties();
* - This function set up Lazy download view property
* void setUpLazyDownloadView();
* - This function set up Lazy download view 
* void setUpIISView();
* - This function set up IIS download view
* void setUpIISViewProperties();
* - This function set up IIS download view property
* void PublishFiles(Object^ obj, RoutedEventArgs^ args);
* - This function gets called when publish button is clicked
* void addText(String^ msg);
* 
* void addText_ListOfFiles(String^ msg);
* void addText_DownloadViewMessage(String^ msg);
*
* void getMessage();
*
* std::vector<std::string> splitAndReplace(std::string, std::string pattern);
*
* void clear(Object^ sender, RoutedEventArgs^ args);
* void getItemsFromList(Object^ sender, RoutedEventArgs^ args);
* - This function gets called when Get files button is clicked
* void getAllItemFromList(Object^ sender, RoutedEventArgs^ args);
* - This function gets called when publish button is clicked
* void dowloadFilesMessage(Object^ sender, RoutedEventArgs^ args);
* - This function gets called when download button is clicked
*
* void getAllFilesMessage(Object^ sender, RoutedEventArgs^ args);
* - This function gets called when getFiles button is clicked
* void getAllFilesNoParentMessage(Object^ sender, RoutedEventArgs^ args);
* - This function gets called when no parent button is clicked
* void deleteFilesMessage(Object^ sender, RoutedEventArgs^ args);
* - This function gets called when delete button is clicked
* void clearListBox_DelView(Object^ sender, RoutedEventArgs^ args);
* - This function gets called when cl;ear button is clicked
*
* void getLazyFiles(Object^ sender, RoutedEventArgs^ args);
* - This function gets called when lazy button is clicked
* void displayWebPage(Object^ sender, RoutedEventArgs^ args);
* - This function gets called when display button is clicked
*
* void getIISFiles(Object^ sender, RoutedEventArgs^ args);
* - This function gets called when iis button is clicked
*
* void browseForFolder(Object^ sender, RoutedEventArgs^ args);
* - This function gets called when browse button is clicked
* void OnLoaded(Object^ sender, RoutedEventArgs^ args);
* - This function defines startup of WPF
* void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
* - This function defines unloading of WPF
*
*
*
*  Maintenance History:
*  --------------------
*  ver 4.0 : 3rd May 2016
*  changes done for project #4
*  ver 3.0 : 22 Apr 2016
*  - added support for multi selection of Listbox items.  Implemented by
*    Saurabh Patel.  Thanks Saurabh.
*  ver 2.0 : 15 Apr 2015
*  - completed message passing demo with moch channel
*  - added FileBrowserDialog to show files in a selected directory
*  ver 1.0 : 13 Apr 2015
*  - incomplete demo with GUI but not connected to mock channel
*/
/*
* Create C++/CLI Console Application
* Right-click project > Properties > Common Language Runtime Support > /clr
* Right-click project > Add > References
*   add references to :
*     System
*     System.Windows.Presentation
*     WindowsBase
*     PresentatioCore
*     PresentationFramework
*/
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::Text;
using namespace System::Windows;

using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;


#include "../MockChannel/MockChannel.h"
#include <iostream>
#include <vector>
#include <sstream>

namespace CppCliWindows
{
	ref class WPFCppCliDemo : Window
	{
		// MockChannel references

		ISendr* pSendr_;
		IRecvr* pRecvr_;
		IMockChannel* pChann_;

		// Controls for Window

		DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
		Grid^ hGrid = gcnew Grid();
		TabControl^ hTabControl = gcnew TabControl();
		TabItem^ hSendMessageTab = gcnew TabItem();
		TabItem^ hFileListTab = gcnew TabItem();
		TabItem^ hDownloadTab = gcnew TabItem();

		///////////////////////////
		TabItem^ hDeleteFilesTab = gcnew TabItem();
		TabItem^ hLazyDownloadTab = gcnew TabItem();
		TabItem^ hIISTab = gcnew TabItem();
		///////////////////////////

		StatusBar^ hStatusBar = gcnew StatusBar();
		StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
		TextBlock^ hStatus = gcnew TextBlock();

		// <----------------------------Code Changes Done For Project#4----------------------------------------------->

		//TextBox^ textBlock_SMP = gcnew TextBox();



		// Controls for DownloadFiles View
		Grid^ hDownloadMessageGrid = gcnew Grid();
		Button^ hDownloadButton = gcnew Button();
		Button^ hClearButton_DV = gcnew Button();

		TextBlock^ hTextBlock_DownTab = gcnew TextBlock();
		Grid^ hGrid3 = gcnew Grid();

		Label^ hDownloadTextLabel = gcnew Label();
		StackPanel^ hStackPanel0_DV = gcnew StackPanel();
		StackPanel^ hStackPanel1_DV = gcnew StackPanel(); // DV--> Send File View
		StackPanel^ hStackPanel2_DV = gcnew StackPanel();
		RadioButton^ hRadioButtonD1 = gcnew RadioButton(); // D--> Send File View
		RadioButton^ hRadioButtonD2 = gcnew RadioButton();
		RadioButton^ hRadioButtonD3 = gcnew RadioButton();

		// Controls for DeleteView 
		Grid^ hDeleteGrid = gcnew Grid();
		Button^ hGetFilesButton = gcnew Button();
		Button^ hDeleteFilesButton = gcnew Button();
		Button^ hGetFilesNoParentButton = gcnew Button();
		ListBox^ hListBox_DelTab = gcnew ListBox();
		Grid^ hGrid4 = gcnew Grid();
		TextBlock^ hTextBlock4 = gcnew TextBlock();
		ScrollViewer^ hScrollViewer4 = gcnew ScrollViewer();
		Label^ hDeleteTextLabel = gcnew Label();
		StackPanel^ hStackPanel0_Del = gcnew StackPanel();
		StackPanel^ hStackPanel1_Del = gcnew StackPanel();

		StackPanel^ hStackPanel2_Del = gcnew StackPanel(); // for radio buttons
		RadioButton^ hRadioButtonDel1 = gcnew RadioButton(); // P--> Publish View
		RadioButton^ hRadioButtonDel2 = gcnew RadioButton();
		RadioButton^ hRadioButtonDel3 = gcnew RadioButton();
		
		StackPanel^ hStackPanel3_Del = gcnew StackPanel();

 
		// Controls for LazyDownload View
		Grid^ hLazyGrid = gcnew Grid();
		Button^ hGetLazyFilesButton = gcnew Button();
		Button^ hLazyDownloadButton = gcnew Button();
		Button^ hDisplayWebPageButton = gcnew Button();
		ListBox^ hListBox_LazyTab = gcnew ListBox();
		Grid^ hGrid5 = gcnew Grid();
		TextBlock^ hTextBlock5 = gcnew TextBlock();
		ScrollViewer^ hScrollViewer5 = gcnew ScrollViewer();
		Label^ hLazyTextLabel = gcnew Label();
		StackPanel^ hStackPanel0_Lazy = gcnew StackPanel();
		StackPanel^ hStackPanel1_Lazy = gcnew StackPanel();

		StackPanel^ hStackPanel2_Lazy = gcnew StackPanel(); // for radio buttons
		RadioButton^ hRadioButtonLazy1 = gcnew RadioButton(); // P--> Publish View
		RadioButton^ hRadioButtonLazy2 = gcnew RadioButton();
		RadioButton^ hRadioButtonLazy3 = gcnew RadioButton();

		StackPanel^ hStackPanel3_Lazy = gcnew StackPanel();


		// Controls for IIS View
		Grid^ hIISGrid = gcnew Grid();
		Button^ hIISFilesButton = gcnew Button();
	
		ListBox^ hListBox_IISTab = gcnew ListBox();
		Grid^ hGrid6 = gcnew Grid();
		TextBlock^ hTextBlock6 = gcnew TextBlock();
		TextBox^ hTextBoxIIS = gcnew TextBox();
		ScrollViewer^ hScrollViewer6 = gcnew ScrollViewer();
		
		Label^ hIISTextLabel = gcnew Label();
		StackPanel^ hStackPanel0_IIS = gcnew StackPanel();


		StackPanel^ hStackPanel1_IIS = gcnew StackPanel();

		StackPanel^ hStackPanel2_IIS = gcnew StackPanel(); // for radio buttons
		RadioButton^ hRadioButtonIIS1 = gcnew RadioButton(); // P--> Publish View
		RadioButton^ hRadioButtonIIS2 = gcnew RadioButton();
		RadioButton^ hRadioButtonIIS3 = gcnew RadioButton();

		StackPanel^ hStackPanel3_IIS = gcnew StackPanel();
		

		// <----------------------------Code Changes Done For Project#4 END----------------------------------------------->

		// Controls for Publish Files View
		Grid^ hPublishGrid = gcnew Grid();
		Button^ hPublishButton = gcnew Button();
		Button^ hClearButton = gcnew Button();
		TextBlock^ hTextBlock1 = gcnew TextBlock();
		ScrollViewer^ hScrollViewer1 = gcnew ScrollViewer();
		StackPanel^ hStackPanel0 = gcnew StackPanel();
		StackPanel^ hStackPanel1 = gcnew StackPanel();
		Label^ hPublishTextLabel = gcnew Label();

		StackPanel^ hStackPanel2 = gcnew StackPanel(); // for radio buttons
		RadioButton^ hRadioButtonP1 = gcnew RadioButton(); // P--> Publish View
		RadioButton^ hRadioButtonP2 = gcnew RadioButton();
		RadioButton^ hRadioButtonP3 = gcnew RadioButton();



		  // Controls for Send Files View ///////////////////
		Grid^ hFileListGrid = gcnew Grid();
		Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
		ListBox^ hListBox = gcnew ListBox();
		Button^ hFolderBrowseButton = gcnew Button();
		Button^ hSendFilesInList = gcnew Button();
		Grid^ hGrid2 = gcnew Grid();
		Label^ hSendTextLabel = gcnew Label();
		StackPanel^ hStackPanel0_SV = gcnew StackPanel(); // SV--> Send File View
		StackPanel^ hStackPanel1_SV = gcnew StackPanel(); // SV--> Send File View
		RadioButton^ hRadioButtonS1 = gcnew RadioButton(); // S--> Send File View
		RadioButton^ hRadioButtonS2 = gcnew RadioButton();
		RadioButton^ hRadioButtonS3 = gcnew RadioButton();

		// receive thread

		Thread^ recvThread;

	public:
		WPFCppCliDemo();
		~WPFCppCliDemo();
		void displayRequirement1();
		void displayRequirement2(); 
		void displayRequirement3(); 
		void displayRequirement4(); 
		void displayRequirement5();
		void displayRequirement6();
		void displayRequirement7();
		void displayRequirement8();
		void displayRequirement9();
		void displayRequirement10(); 
		void displayRequirement11();

		void displayRequirements(); // displays all the requirements
		void displayPattern(int);

		void setUpStatusBar();
		void setUpTabControl();
		void setUpPublishView();
		void setUpSendFileListViewProperties();
		void setUpSendFileListView();
		void setUpDownloadViewProperties();
		void setUpDownloadView();
		void setUpDeleteViewProperties();
		void setUpDeleteView();
		void setUpLazyDownloadViewProperties();
		void setUpLazyDownloadView();
		void setUpIISView();
		void setUpIISViewProperties();
		void PublishFiles(Object^ obj, RoutedEventArgs^ args);
		void addText(String^ msg);

		void addText_ListOfFiles(String^ msg);
		void addText_DownloadViewMessage(String^ msg);

		void getMessage();

		std::vector<std::string> splitAndReplace(std::string, std::string pattern);

		void clear(Object^ sender, RoutedEventArgs^ args);
		void getItemsFromList(Object^ sender, RoutedEventArgs^ args);
		///// My Code /////

		// Tab2
		void getAllItemFromList(Object^ sender, RoutedEventArgs^ args);

		// Tab3
		void dowloadFilesMessage(Object^ sender, RoutedEventArgs^ args);

		// Tab 4
		void getAllFilesMessage(Object^ sender, RoutedEventArgs^ args);
		void getAllFilesNoParentMessage(Object^ sender, RoutedEventArgs^ args);
		void deleteFilesMessage(Object^ sender, RoutedEventArgs^ args);
		void clearListBox_DelView(Object^ sender, RoutedEventArgs^ args);

		// Tab 5
		void getLazyFiles(Object^ sender, RoutedEventArgs^ args);
		void displayWebPage(Object^ sender, RoutedEventArgs^ args);
		
		//Tab 6
		void getIISFiles(Object^ sender, RoutedEventArgs^ args);

		///// My Code /////
		void browseForFolder(Object^ sender, RoutedEventArgs^ args);
		void OnLoaded(Object^ sender, RoutedEventArgs^ args);
		void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
	private:
		

		std::string toStdString(String^ pStr);
		String^ toSystemString(std::string& str);
		void setTextBlockProperties();
		void setButtonsProperties();
	};
}


#endif
