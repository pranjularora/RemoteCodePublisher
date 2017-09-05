///////////////////////////////////////////////////////////////////////////
// Window.cpp - C++\CLI implementation of WPF Application                //
//          - Demo for CSE 687 Project #4                                //
// ver 3.0																 //
// Author: Pranjul Arora												 //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015     //
///////////////////////////////////////////////////////////////////////////
/*
*  To run as a Windows Application:
*  - Set Project Properties > Linker > System > Subsystem to Windows
*  - Comment out int main(...) at bottom
*  - Uncomment int _stdcall WinMain() at bottom
*  To run as a Console Application:
*  - Set Project Properties > Linker > System > Subsytem to Console
*  - Uncomment int main(...) at bottom
*  - Comment out int _stdcall WinMain() at bottom
*/
#include "Window.h"
using namespace CppCliWindows;
// Construtor initializing views
WPFCppCliDemo::WPFCppCliDemo() {
	displayRequirements();
	ObjectFactory* pObjFact = new ObjectFactory;// set up channel
	pSendr_ = pObjFact->createSendr();
	pRecvr_ = pObjFact->createRecvr();
	pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_);
	pChann_->start();
	delete pObjFact;

	recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage)); // client's receive thread
	recvThread->Start();
	// set event handlers
	this->Loaded += gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
	this->Closing += gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
	hPublishButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::PublishFiles);
	hClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
	hClearButton_DV->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
	hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
	hSendFilesInList->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getAllItemFromList);
	hDownloadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::dowloadFilesMessage);
	hGetFilesButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getAllFilesMessage);
	hGetFilesNoParentButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getAllFilesNoParentMessage);
	hDeleteFilesButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::deleteFilesMessage);
	hRadioButtonDel1->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clearListBox_DelView);
	hRadioButtonDel2->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clearListBox_DelView);
	hRadioButtonDel3->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clearListBox_DelView);
	hGetLazyFilesButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getAllFilesMessage);
	hDisplayWebPageButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::displayWebPage);
	hRadioButtonLazy1->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clearListBox_DelView);
	hRadioButtonLazy2->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clearListBox_DelView);
	hRadioButtonLazy3->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clearListBox_DelView);
	hLazyDownloadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getLazyFiles);
	hIISFilesButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getIISFiles);
	this->Title = "Client GUI"; // set Window properties
	this->Width = 800;
	this->Height = 600;
	this->Content = hDockPanel; // attach dock panel to Window
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);
	setUpTabControl();// setup Window controls and views
	setUpStatusBar();
	setUpPublishView();
	setUpSendFileListView();
	setUpDownloadView();
	setUpLazyDownloadView();
	setUpIISView();
	setUpDeleteView();
}
// destructor
WPFCppCliDemo::~WPFCppCliDemo()
{
	delete pChann_;
	delete pSendr_;
	delete pRecvr_;
}
// <-----------------SETTING UP STATUS BAR-------------------------------->
void WPFCppCliDemo::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	//hStatus->Text = "very important messages will appear here";
	//status->FontWeight = FontWeights::Bold;
	hStatusBarItem->Content = hStatus;
	hStatusBar->Padding = Thickness(10, 2, 10, 2);
}
// <----------------------SETTING UP THE TAB CONTROLS--------------------------->
void WPFCppCliDemo::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	hSendMessageTab->Header = "Publish Files";
	hFileListTab->Header = "Send Files";
	hDownloadTab->Header = "Download Files";

	hDeleteFilesTab->Header = "Delete/Get Files";
	hLazyDownloadTab->Header = "Lazy Download / Display Web Page";
	hIISTab->Header = "IIS Download";
	hTabControl->Items->Add(hFileListTab);
	hTabControl->Items->Add(hSendMessageTab);
	hTabControl->Items->Add(hDownloadTab);
	hTabControl->Items->Add(hDeleteFilesTab);
	hTabControl->Items->Add(hLazyDownloadTab);
	hTabControl->Items->Add(hIISTab);
}
// <--------------------SETTING UP TEXT BLOCK PROPERTY FOR PUBLISH VIEW----------------------------->
void WPFCppCliDemo::setTextBlockProperties()
{
	RowDefinition^ hRow0Def = gcnew RowDefinition();
	hRow0Def->Height = GridLength(30);
	hPublishGrid->RowDefinitions->Add(hRow0Def);

	hPublishTextLabel->Content = "Select a Category for which Publishing Needs to be done!";
	hPublishTextLabel->FontSize = 16;
	hPublishTextLabel->FontWeight = FontWeights::Bold;

	hStackPanel0->Children->Add(hPublishTextLabel);
	hPublishGrid->Children->Add(hStackPanel0);
	hPublishGrid->SetRow(hStackPanel0, 0);

	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hPublishGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextBlock1;
	hTextBlock1->Padding = Thickness(15);
	hTextBlock1->Text = "";
	//hTextBlock1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlock1->FontWeight = FontWeights::Bold;
	hTextBlock1->FontSize = 14;
	hScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer1->Content = hBorder1;
	hPublishGrid->SetRow(hScrollViewer1, 1);
	hPublishGrid->Children->Add(hScrollViewer1);
}

// <-----------------------SETTING UP BUTTON PROPERTY FOR PUBLISH VIEW-------------------------->
void WPFCppCliDemo::setButtonsProperties() { // SETTING UP BUTTON PROPERTY FOR PUBLISH VIEW
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hPublishGrid->RowDefinitions->Add(hRow2Def);
	hRadioButtonP1->Content = "Category A";
	hRadioButtonP2->Content = "Category B";
	hRadioButtonP3->Content = "Category C";
	hRadioButtonP1->IsChecked = true;
	hStackPanel2->Orientation = Orientation::Horizontal;
	hStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer2->Width = 20;
	hSpacer3->Width = 20;
	hStackPanel2->Children->Add(hRadioButtonP1);
	hStackPanel2->Children->Add(hSpacer2);
	hStackPanel2->Children->Add(hRadioButtonP2);
	hStackPanel2->Children->Add(hSpacer3);
	hStackPanel2->Children->Add(hRadioButtonP3);
	hPublishGrid->Children->Add(hStackPanel2);
	hPublishGrid->SetRow(hStackPanel2, 2);
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(75);
	hPublishGrid->RowDefinitions->Add(hRow3Def);
	hPublishButton->Content = "Publish Files";
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hClearButton->Content = "Clear";
	hBorder3->Child = hPublishButton;
	Border^ hBorder4 = gcnew Border();
	hBorder4->Width = 120;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hClearButton;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 20;
	hStackPanel1->Children->Add(hBorder3);
	hStackPanel1->Children->Add(hSpacer);
	hStackPanel1->Children->Add(hBorder4);
	hStackPanel1->Orientation = Orientation::Horizontal;
	hStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hPublishGrid->SetRow(hStackPanel1, 3);
	hPublishGrid->Children->Add(hStackPanel1);
}
// <-----------------SETTING UP PUBLISH VIEW-------------------------------->
void WPFCppCliDemo::setUpPublishView() { //SETTING UP PUBLISH VIEW
	Console::Write("\n  Setting up Publish Files view");
	hPublishGrid->Margin = Thickness(20);
	hSendMessageTab->Content = hPublishGrid;

	setTextBlockProperties();
	setButtonsProperties();
}


// <-----------------------SETTING UP IIS TAB VIEW-------------------------->
void WPFCppCliDemo::setUpIISView() { //SETTING UP IIS TAB VIEW
	Console::Write("\n  setting up IIS Download view");
	hIISGrid->Margin = Thickness(20);
	hIISTab->Content = hIISGrid;

	RowDefinition^ hRow0Def = gcnew RowDefinition();
	hRow0Def->Height = GridLength(30);
	hIISGrid->RowDefinitions->Add(hRow0Def);

	hIISTextLabel->Content = "Specify a valid path in the text area below!\nFiles will be download at the specified path!\nA category folder will be created based on the chosen category!";
	hIISTextLabel->FontSize = 16;
	hIISTextLabel->FontWeight = FontWeights::Bold;


	hStackPanel0_IIS->Children->Add(hIISTextLabel);
	hIISGrid->Children->Add(hStackPanel0_IIS);
	hIISGrid->SetRow(hStackPanel0_IIS, 0);

	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hIISGrid->RowDefinitions->Add(hRow1Def);
	hRow0Def->Height = GridLength(150);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextBoxIIS;
	hIISGrid->SetRow(hBorder1, 1);
	hIISGrid->Children->Add(hBorder1);
	setUpIISViewProperties();
}
// <----------------------SETTING UP Properties of IIS Tab View--------------------------->
void WPFCppCliDemo::setUpIISViewProperties() { // -SETTING UP Properties of IIS Tab View-
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hIISGrid->RowDefinitions->Add(hRow2Def);

	hRadioButtonIIS1->Content = "Category A";
	hRadioButtonIIS2->Content = "Category B";
	hRadioButtonIIS3->Content = "Category C";
	hRadioButtonIIS1->IsChecked = true;
	hStackPanel2_IIS->Orientation = Orientation::Horizontal;
	hStackPanel2_IIS->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer2->Width = 20;
	hSpacer3->Width = 20;
	hStackPanel2_IIS->Children->Add(hRadioButtonIIS1);
	hStackPanel2_IIS->Children->Add(hSpacer2);
	hStackPanel2_IIS->Children->Add(hRadioButtonIIS2);
	hStackPanel2_IIS->Children->Add(hSpacer3);
	hStackPanel2_IIS->Children->Add(hRadioButtonIIS3);
	hIISGrid->Children->Add(hStackPanel2_IIS);
	hIISGrid->SetRow(hStackPanel2_IIS, 2);

	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(75);
	hIISGrid->RowDefinitions->Add(hRow3Def);
	TextBlock^ hSpacer4 = gcnew TextBlock();
	hSpacer4->Width = 20;
	hStackPanel3_IIS->Orientation = Orientation::Horizontal;
	hStackPanel3_IIS->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;

	hIISFilesButton->Content = "IIS";
	hIISFilesButton->Height = 30;
	hIISFilesButton->Width = 120;
	hIISFilesButton->BorderThickness = Thickness(2);
	hIISFilesButton->BorderBrush = Brushes::Black;

	hStackPanel3_IIS->Children->Add(hIISFilesButton);

	hIISGrid->Children->Add(hStackPanel3_IIS);
	hIISGrid->SetRow(hStackPanel3_IIS, 3);
}
// <----------------------SETTING UP PROPERTIES FOR LAZY DOWNLOAD AND DISPLAY WEB PAGE VIEW--------------------------->
void WPFCppCliDemo::setUpLazyDownloadViewProperties() { // SETTING UP PROPERTIES FOR LAZY DOWNLOAD AND DISPLAY WEB PAGE VIEW
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(75);
	hLazyGrid->RowDefinitions->Add(hRow3Def);
	TextBlock^ hSpacer4 = gcnew TextBlock();
	hSpacer4->Width = 20;
	TextBlock^ hSpacer5 = gcnew TextBlock();
	hSpacer5->Width = 20;
	hStackPanel3_Lazy->Orientation = Orientation::Horizontal;
	hStackPanel3_Lazy->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;

	hGetLazyFilesButton->Content = "Get All Files";
	hGetLazyFilesButton->Height = 30;
	hGetLazyFilesButton->Width = 120;
	hGetLazyFilesButton->BorderThickness = Thickness(2);
	hGetLazyFilesButton->BorderBrush = Brushes::Black;

	hLazyDownloadButton->Content = "Lazy Download";
	hLazyDownloadButton->Height = 30;
	hLazyDownloadButton->Width = 120;
	hLazyDownloadButton->BorderThickness = Thickness(2);
	hLazyDownloadButton->BorderBrush = Brushes::Black;

	hDisplayWebPageButton->Content = "Display Web Page";
	hDisplayWebPageButton->Height = 30;
	hDisplayWebPageButton->Width = 120;
	hDisplayWebPageButton->BorderThickness = Thickness(2);
	hDisplayWebPageButton->BorderBrush = Brushes::Black;

	hStackPanel3_Lazy->Children->Add(hGetLazyFilesButton);
	hStackPanel3_Lazy->Children->Add(hSpacer4);
	hStackPanel3_Lazy->Children->Add(hDisplayWebPageButton);
	hStackPanel3_Lazy->Children->Add(hSpacer5);
	hStackPanel3_Lazy->Children->Add(hLazyDownloadButton);

	hLazyGrid->Children->Add(hStackPanel3_Lazy);
	hLazyGrid->SetRow(hStackPanel3_Lazy, 3);
}

// <----------------------SETTING UP LAZY DOWNLOAD AND DISPLAY WEB PAGE VIEW--------------------------->
void WPFCppCliDemo::setUpLazyDownloadView() { // SETTING UP LAZY DOWNLOAD AND DISPLAY WEB PAGE VIEW
	Console::Write("\n  setting up Lazy Download view");
	hLazyGrid->Margin = Thickness(20);
	hLazyDownloadTab->Content = hLazyGrid;
	RowDefinition^ hRow0Def = gcnew RowDefinition();
	hRow0Def->Height = GridLength(130);
	hLazyGrid->RowDefinitions->Add(hRow0Def);
	hLazyTextLabel->Content += "\t\t\tNote: Only Published files (i.e. \".htm\") files will appear here!\n\t\t\tSo, Files Needs to be Published before retrieving the list";
	hLazyTextLabel->Content += "\nNote: For Demonstration: Lazy Download Cleans local Repository first and then downloads Lazy and dependent files\n\n";
	hLazyTextLabel->Content += "\"Get All Files\"          ==>> Display all files from the selected category  \n";
	hLazyTextLabel->Content += "\"Display Web Page\"==>>  Select a file from List Box to view its Web Page \n";
	hLazyTextLabel->Content += "\"Lazy Download\"     ==>>  Select a file from List Box to make Lazy Download";
	hLazyTextLabel->FontSize = 13;
	hLazyTextLabel->FontWeight = FontWeights::Bold;
	hStackPanel0_Lazy->Children->Add(hLazyTextLabel);
	hLazyGrid->Children->Add(hStackPanel0_Lazy);
	hLazyGrid->SetRow(hStackPanel0_Lazy, 0);
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hLazyGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hListBox_LazyTab->SelectionMode = SelectionMode::Single;
	hBorder1->Child = hListBox_LazyTab;
	hLazyGrid->SetRow(hBorder1, 1);
	hLazyGrid->Children->Add(hBorder1);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hLazyGrid->RowDefinitions->Add(hRow2Def);
	hRadioButtonLazy1->Content = "Category A";
	hRadioButtonLazy2->Content = "Category B";
	hRadioButtonLazy3->Content = "Category C";
	hRadioButtonLazy1->IsChecked = true;
	hStackPanel2_Lazy->Orientation = Orientation::Horizontal;
	hStackPanel2_Lazy->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer2->Width = 20;
	hSpacer3->Width = 20;
	hStackPanel2_Lazy->Children->Add(hRadioButtonLazy1);
	hStackPanel2_Lazy->Children->Add(hSpacer2);
	hStackPanel2_Lazy->Children->Add(hRadioButtonLazy2);
	hStackPanel2_Lazy->Children->Add(hSpacer3);
	hStackPanel2_Lazy->Children->Add(hRadioButtonLazy3);
	hLazyGrid->Children->Add(hStackPanel2_Lazy);
	hLazyGrid->SetRow(hStackPanel2_Lazy, 2);
	setUpLazyDownloadViewProperties();
}

// <----------------------SETTING UP PROPERTIES FOR DELETE VIEW--------------------------->
void WPFCppCliDemo::setUpDeleteViewProperties() { // SETTING UP PROPERTIES FOR DELETE VIEW
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(55);
	hDeleteGrid->RowDefinitions->Add(hRow2Def);
	hRadioButtonDel1->Content = "Category A";
	hRadioButtonDel2->Content = "Category B";
	hRadioButtonDel3->Content = "Category C";
	hRadioButtonDel1->IsChecked = true;
	hStackPanel2_Del->Orientation = Orientation::Horizontal;
	hStackPanel2_Del->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	TextBlock^ hSpacer2 = gcnew TextBlock(); hSpacer2->Width = 20;
	TextBlock^ hSpacer3 = gcnew TextBlock(); hSpacer3->Width = 20;
	hStackPanel2_Del->Children->Add(hRadioButtonDel1);
	hStackPanel2_Del->Children->Add(hSpacer2);
	hStackPanel2_Del->Children->Add(hRadioButtonDel2);
	hStackPanel2_Del->Children->Add(hSpacer3);
	hStackPanel2_Del->Children->Add(hRadioButtonDel3);
	hDeleteGrid->Children->Add(hStackPanel2_Del);
	hDeleteGrid->SetRow(hStackPanel2_Del, 2);
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(55);
	hDeleteGrid->RowDefinitions->Add(hRow3Def);
	TextBlock^ hSpacer4 = gcnew TextBlock(); hSpacer4->Width = 20;
	TextBlock^ hSpacer5 = gcnew TextBlock(); hSpacer5->Width = 20;
	TextBlock^ hSpacer6 = gcnew TextBlock(); hSpacer6->Width = 20;
	hStackPanel3_Del->Orientation = Orientation::Horizontal;
	hStackPanel3_Del->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hGetFilesButton->Content = "Get All Files";
	hGetFilesButton->Height = 30; hGetFilesButton->Width = 120;
	hGetFilesButton->BorderThickness = Thickness(2);
	hGetFilesButton->BorderBrush = Brushes::Black;
	hGetFilesNoParentButton->Content = "Get No Parent Files";
	hGetFilesNoParentButton->Height = 30; hGetFilesNoParentButton->Width = 120;
	hGetFilesNoParentButton->BorderThickness = Thickness(2);
	hGetFilesNoParentButton->BorderBrush = Brushes::Black;
	hDeleteFilesButton->Content = "Delete Files";
	hDeleteFilesButton->Height = 30; hDeleteFilesButton->Width = 120;
	hDeleteFilesButton->BorderThickness = Thickness(2);
	hDeleteFilesButton->BorderBrush = Brushes::Black;
	hStackPanel3_Del->Children->Add(hGetFilesButton);
	hStackPanel3_Del->Children->Add(hSpacer4);
	hStackPanel3_Del->Children->Add(hGetFilesNoParentButton);
	hStackPanel3_Del->Children->Add(hSpacer5);
	hStackPanel3_Del->Children->Add(hDeleteFilesButton);
	hStackPanel3_Del->Children->Add(hSpacer6);
	hDeleteGrid->Children->Add(hStackPanel3_Del);
	hDeleteGrid->SetRow(hStackPanel3_Del, 3);
}
// <----------------------SETTING UP DELETE--------------------------->
void WPFCppCliDemo::setUpDeleteView() { // SETTING UP DELETE
	Console::Write("\n  setting up Delete view");
	hDeleteGrid->Margin = Thickness(20);
	hDeleteFilesTab->Content = hDeleteGrid;
	RowDefinition^ hRow0Def = gcnew RowDefinition();
	hRow0Def->Height = GridLength(120);
	hDeleteGrid->RowDefinitions->Add(hRow0Def);
	hDeleteTextLabel->Content += "\t\t\tNote: Only Published files (i.e. \".htm\") files will appear here!\n\t\t\tSo, Files Needs to be Published before retrieving the list\n\n";
	hDeleteTextLabel->Content += "\"Get All Files\"==>> Display all files from the selected category  \n\"Delete\"         ==>> Delete selected files from the Repository";
	hDeleteTextLabel->Content += "\t\n Note: It only deletes .htm files, not the sent source files";
	hDeleteTextLabel->FontSize = 13;
	hDeleteTextLabel->FontWeight = FontWeights::Bold;
	hStackPanel0_Del->Children->Add(hDeleteTextLabel);
	hDeleteGrid->Children->Add(hStackPanel0_Del);
	hDeleteGrid->SetRow(hStackPanel0_Del, 0);
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(250);
	hDeleteGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hListBox_DelTab->SelectionMode = SelectionMode::Multiple;
	hBorder1->Child = hListBox_DelTab;
	hDeleteGrid->SetRow(hBorder1, 1);
	hDeleteGrid->Children->Add(hBorder1);
	setUpDeleteViewProperties();
}
// <----------------------SETTING UP PROPERTIES DOWNLOAD VIEW--------------------------->
void WPFCppCliDemo::setUpDownloadViewProperties() { // SETTING UP PROPERTIES DOWNLOAD VIEW
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(75);
	RowDefinition^ hRow3Def2 = gcnew RowDefinition();
	hRow3Def2->Height = GridLength(75);
	hDownloadMessageGrid->RowDefinitions->Add(hRow3Def);
	hDownloadMessageGrid->RowDefinitions->Add(hRow3Def2);

	hStackPanel2_DV->Orientation = Orientation::Horizontal;
	hStackPanel2_DV->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;

	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 20;

	hDownloadButton->Content = "Download Files";
	hDownloadButton->Height = 30;
	hDownloadButton->Width = 120;
	hDownloadButton->BorderThickness = Thickness(2);
	hDownloadButton->BorderBrush = Brushes::Black;

	hClearButton_DV->Content = "Clear";
	hClearButton_DV->Height = 30;
	hClearButton_DV->Width = 120;
	hClearButton_DV->BorderThickness = Thickness(2);
	hClearButton_DV->BorderBrush = Brushes::Black;

	hStackPanel2_DV->Children->Add(hDownloadButton);
	hStackPanel2_DV->Children->Add(hSpacer1);
	hStackPanel2_DV->Children->Add(hClearButton_DV);

	hDownloadMessageGrid->SetRow(hStackPanel2_DV, 3);
	hDownloadMessageGrid->Children->Add(hStackPanel2_DV);
}

// <----------------------SETTING UP DOWNLOAD VIEW--------------------------->
void WPFCppCliDemo::setUpDownloadView() { // SETTING UP DOWNLOAD VIEW
	Console::Write("\n  setting up Connection view");
	hDownloadMessageGrid->Margin = Thickness(20);
	hDownloadTab->Content = hDownloadMessageGrid;
	RowDefinition^ hRow0Def = gcnew RowDefinition();
	hRow0Def->Height = GridLength(30);
	hDownloadMessageGrid->RowDefinitions->Add(hRow0Def);
	hDownloadTextLabel->Content = "Download Category Specific files by selecting a category!";
	hDownloadTextLabel->FontSize = 16;
	hDownloadTextLabel->FontWeight = FontWeights::Bold;
	hStackPanel0_DV->Children->Add(hDownloadTextLabel);
	hDownloadMessageGrid->Children->Add(hStackPanel0_DV);
	hDownloadMessageGrid->SetRow(hStackPanel0_DV, 0);
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hDownloadMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextBlock_DownTab;
	hDownloadMessageGrid->SetRow(hBorder1, 1);
	hDownloadMessageGrid->Children->Add(hBorder1);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hDownloadMessageGrid->RowDefinitions->Add(hRow2Def);
	hRadioButtonD1->Content = "Category A";
	hRadioButtonD2->Content = "Category B";
	hRadioButtonD3->Content = "Category C";
	hRadioButtonD1->IsChecked = true;
	hStackPanel1_DV->Orientation = Orientation::Horizontal;
	hStackPanel1_DV->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer2->Width = 20;
	hSpacer3->Width = 20;
	hStackPanel1_DV->Children->Add(hRadioButtonD1);
	hStackPanel1_DV->Children->Add(hSpacer2);
	hStackPanel1_DV->Children->Add(hRadioButtonD2);
	hStackPanel1_DV->Children->Add(hSpacer3);
	hStackPanel1_DV->Children->Add(hRadioButtonD3);
	hDownloadMessageGrid->Children->Add(hStackPanel1_DV);
	hDownloadMessageGrid->SetRow(hStackPanel1_DV, 2);
	setUpDownloadViewProperties();
}
// <----------------------SETTING UP PROPERTIES SEND FILES LIST VIEW--------------------------->
void WPFCppCliDemo::setUpSendFileListViewProperties() { // SETTING UP PROPERTIES SEND FILES LIST VIEW
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(55);
	hFileListGrid->RowDefinitions->Add(hRow3Def);
	hRadioButtonS1->Content = "Category A";
	hRadioButtonS2->Content = "Category B";
	hRadioButtonS3->Content = "Category C";
	hRadioButtonS1->IsChecked = true;
	hStackPanel1_SV->Orientation = Orientation::Horizontal;
	hStackPanel1_SV->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer2->Width = 20;
	hSpacer3->Width = 20;
	hStackPanel1_SV->Children->Add(hRadioButtonS1);
	hStackPanel1_SV->Children->Add(hSpacer2);
	hStackPanel1_SV->Children->Add(hRadioButtonS2);
	hStackPanel1_SV->Children->Add(hSpacer3);
	hStackPanel1_SV->Children->Add(hRadioButtonS3);
	hFileListGrid->Children->Add(hStackPanel1_SV);
	hFileListGrid->SetRow(hStackPanel1_SV, 2);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(55);
	RowDefinition^ hRow2Def2 = gcnew RowDefinition();
	hRow2Def2->Height = GridLength(55);
	hFileListGrid->RowDefinitions->Add(hRow2Def);
	hFileListGrid->RowDefinitions->Add(hRow2Def2);
	hFolderBrowseButton->Content = "Select Directory";
	hFolderBrowseButton->Height = 30;
	hFolderBrowseButton->Width = 120;
	hFolderBrowseButton->BorderThickness = Thickness(2);
	hFolderBrowseButton->BorderBrush = Brushes::Black;
	hFileListGrid->SetRow(hFolderBrowseButton, 3);
	hFileListGrid->Children->Add(hFolderBrowseButton);
	hSendFilesInList->Content = "Send All Files";
	hSendFilesInList->Height = 30;
	hSendFilesInList->Width = 120;
	hSendFilesInList->BorderThickness = Thickness(2);
	hSendFilesInList->BorderBrush = Brushes::Black;
	hFileListGrid->SetRow(hSendFilesInList, 4);
	hFileListGrid->Children->Add(hSendFilesInList);

	hFolderBrowserDialog->ShowNewFolderButton = false;
	hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}

// <----------------------SETTING UP SEND FILES LIST VIEW--------------------------->
void WPFCppCliDemo::setUpSendFileListView() { // SETTING UP SEND FILES LIST VIEW
	Console::Write("\n  setting up FileList view");
	hFileListGrid->Margin = Thickness(20);
	hFileListTab->Content = hFileListGrid;

	RowDefinition^ hRow0Def = gcnew RowDefinition();
	hRow0Def->Height = GridLength(50);
	hFileListGrid->RowDefinitions->Add(hRow0Def);

	hSendTextLabel->Content = "Messages for this Tab will appear on the Status Bar";
	hSendTextLabel->FontSize = 16;
	hSendTextLabel->FontWeight = FontWeights::Bold;


	hStackPanel0_SV->Children->Add(hSendTextLabel);
	hFileListGrid->Children->Add(hStackPanel0_SV);
	hFileListGrid->SetRow(hStackPanel0_SV, 0);

	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(250);
	hFileListGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hListBox->SelectionMode = SelectionMode::Extended;
	hBorder1->Child = hListBox;
	hFileListGrid->SetRow(hBorder1, 1);
	hFileListGrid->Children->Add(hBorder1);
	setUpSendFileListViewProperties();



}


// <------------------CONVERT C++ CLI STRING TO C++ STRING-------------------------------->
std::string WPFCppCliDemo::toStdString(String^ pStr) { // CONVERT C++ CLI STRING TO C++ STRING
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}


// <------------------CONVERT C++ STRING TO C++ CLI STRING-------------------------------->
String^ WPFCppCliDemo::toSystemString(std::string& str) { // CONVERT C++ STRING TO C++ CLI STRING
	StringBuilder^ pStr = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
}

// <------------------GETS CALLED WHEN PUBLISH BUTTON IS CLICKED-------------------------------->
void WPFCppCliDemo::PublishFiles(Object^ obj, RoutedEventArgs^ args)
{
	String^ category;
	if (hRadioButtonP1->IsChecked) {
		category = "categoryA";
	}
	else if (hRadioButtonP2->IsChecked) {
		category = "categoryB";
	}
	else {
		category = "categoryC";
	}
	String^ message = category + ",PublishFiles";

	pSendr_->postMessage(toStdString(message));
	Console::Write("\n  Sent message to Publish Files");
	hStatus->Text = "Sent message to Publish " + category + " Files";
}
// <------------------IT ADD PUBLISH BUTTON RESPONSE TO GUI-------------------------------->
void WPFCppCliDemo::addText(String^ msg)
{
	std::vector<std::string> msgVectorFromGUI = splitAndReplace(toStdString(msg), ",");
	std::string messageToPost = "Publishing Files Status [Location/Message]: ====>>>  \n";
	messageToPost += msgVectorFromGUI[0];
	hTextBlock1->Text += toSystemString(messageToPost) + "\n\n";

}
// <------------------IT ADD GET LIST OF FILES BUTTON RESPONSE TO GUI-------------------------------->
void WPFCppCliDemo::addText_ListOfFiles(String^ msg) {
	std::vector<std::string> msgVectorFromGUI = splitAndReplace(toStdString(msg), ",");

	for (size_t i = 0; i < msgVectorFromGUI.size() - 2; i++)
	{
		if (hTabControl->SelectedIndex == 3) {
			if (msgVectorFromGUI[i].find("No Files Present in category") != std::string::npos)
				hStatus->Text = toSystemString(msgVectorFromGUI[i]);
			else
				hListBox_DelTab->Items->Add(toSystemString(msgVectorFromGUI[i]));

		}
		else if (hTabControl->SelectedIndex == 4) {
			if (msgVectorFromGUI[i].find("No Files Present in category") != std::string::npos)
				hStatus->Text = toSystemString(msgVectorFromGUI[i]);
			else
				hListBox_LazyTab->Items->Add(toSystemString(msgVectorFromGUI[i]));
		}
	}
}
// <------------------IT ADD DOWNLOAD BUTTON RESPONSE TO GUI-------------------------------->
void WPFCppCliDemo::addText_DownloadViewMessage(String^ msg) {
	std::vector<std::string> msgVectorFromGUI = splitAndReplace(toStdString(msg), ",");
	std::string messageToPost = "Downloading Status [Location/Message]:  ";
	messageToPost += msgVectorFromGUI[0];
	hTextBlock_DownTab->Text += toSystemString(messageToPost) + "\n\n";

}

// mini tokenizer written to tokenize a string based on delimeters
std::vector<std::string> WPFCppCliDemo::splitAndReplace(std::string split, std::string pattern) {
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

// <------------------RECEIVED MESSAGE TO PUT ON GUI-------------------------------->
void WPFCppCliDemo::getMessage()
{
	// recvThread runs this function

	while (true)
	{
		std::string msgType;
		std::cout << "\n ========>> Receive Thread calling getMessage()";
		std::string msg = pRecvr_->getMessage();
		//	std::cout << "\n  Message recived to post on GUI ----=====---==>> " << msg << "\n\n";
		String^ sMsg = toSystemString(msg);
		array<String^>^ args = gcnew array<String^>(1);
		args[0] = sMsg;
		std::vector<std::string> msgVectorFromGUI = splitAndReplace(msg, ",");
		msgType = msgVectorFromGUI[msgVectorFromGUI.size() - 1];
		/*std::string category = msgVectorFromGUI[msgVectorFromGUI.size() - 2];
		for (size_t i = 0; i < msgVectorFromGUI.size() - 1; ++i)
		{

		}*/

		if (msgType == "GetFilesList_post" && sMsg->Length > 2) {
			std::cout << "\n ========>> Posting MSG TO GUI \n";
			Action<String^>^ act_FilesList = gcnew Action<String^>(this, &WPFCppCliDemo::addText_ListOfFiles);
			Dispatcher->Invoke(act_FilesList, args);
		}
		else if (msgType == "DownloadFiles_post") {
			std::cout << "\n\n ========>> Posting MSG TO GUI: Files Downloaded\n";
			Action<String^>^ act_DownloadMsg = gcnew Action<String^>(this, &WPFCppCliDemo::addText_DownloadViewMessage);
			Dispatcher->Invoke(act_DownloadMsg, args);
		}
		else if (msgType == "PublishFiles_post") {
			std::cout << "\n\n ========>> Posting MSG TO GUI: Files Published\n";
			Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addText);
			Dispatcher->Invoke(act, args);  // must call addText on main UI thread
		}
	}
}
// <------------------GETS CALLED WHEN CLEAR BUTTON IS CLICKED-------------------------------->
void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared message text");
	if (hTabControl->SelectedIndex == 1) {
		hStatus->Text = "Cleared message";
		hTextBlock1->Text = "";
	}
	else if (hTabControl->SelectedIndex == 2) {
		hTextBlock_DownTab->Text = "";
	}
}

//////////////////  My code /////////////////
// start from here

// <------------------GETS CALLED WHEN GET FILES BUTTON IS CLICKED-------------------------------->
void WPFCppCliDemo::getAllItemFromList(Object^ sender, RoutedEventArgs^ args) {
	String^ category;
	if (hRadioButtonS1->IsChecked) {
		category = "categoryA";
	}
	else if (hRadioButtonS2->IsChecked) {
		category = "categoryB";
	}
	else {
		category = "categoryC";
	}
	String^ message = category + ",SendFiles";

	String^ listOfFiles;
	int counter = 0;
	int count = hListBox->Items->Count;
	if (count > 0) {
		for each(String^ item in hListBox->Items) {

			listOfFiles += item + ",";
		}

		listOfFiles += message;
		hStatus->Text = "Sending all .cpp and .h Files From the list";
		pSendr_->postMessage(toStdString(listOfFiles));
	}
	else {
		hStatus->Text = "No Files to Send";
	}

	// Now we have all files from the Listbox in a comma seperated string
	// send it to client, as send message button does
	// and check where split function needs to be called
}

// <------------------GETS CALLED WHEN DOWNLOAD BUTTON IS CLICKED-------------------------------->
void WPFCppCliDemo::dowloadFilesMessage(Object^ sender, RoutedEventArgs^ args) {
	String^ category;
	if (hRadioButtonD1->IsChecked) {
		category = "categoryA";
	}
	else if (hRadioButtonD2->IsChecked) {
		category = "categoryB";
	}
	else {
		category = "categoryC";
	}
	String^ message = category + ",DownloadFiles";

	pSendr_->postMessage(toStdString(message));

	Console::Write("\n======>>>>>>  sent message to download all Files from category: " + category + "\n\n");
	hStatus->Text = "Download Files";
}
// <------------------GETS CALLED WHEN DISPLAY BUTTON IS CLICKED-------------------------------->
void WPFCppCliDemo::displayWebPage(Object^ sender, RoutedEventArgs^ args) {
	String^ category;
	if (hListBox_LazyTab->SelectedItems->Count == 0) {
		hStatus->Text = "Please Select A File To Display ";
		return;
	}
	if (hRadioButtonLazy1->IsChecked) {
		category = "categoryA";
	}
	else if (hRadioButtonLazy2->IsChecked) {
		category = "categoryB";
	}
	else {
		category = "categoryC";
	}
	String^ message = hListBox_LazyTab->SelectedItem + ",";
	message += category + ",DisplayFile";
	pSendr_->postMessage(toStdString(message));

	Console::Write("\n======>>>>>>  sent message to display Web Page from category: " + category + "\n\n");
	hStatus->Text = "Displaying Web Page";
}

// <------------------GETS CALLED WHEN NO PARENT BUTTON IS CLICKED-------------------------------->
void WPFCppCliDemo::getAllFilesNoParentMessage(Object^ sender, RoutedEventArgs^ args) {
	String^ category;
	if (hRadioButtonDel1->IsChecked) {
		category = "categoryA";
	}
	else if (hRadioButtonDel2->IsChecked) {
		category = "categoryB";
	}
	else {
		category = "categoryC";
	}
	String^ message = category + ",GetFilesNoParentList";

	pSendr_->postMessage(toStdString(message));

	Console::Write("\n======>>>>>>  sent message to Get List of all Files with No Parent from category: " + category + "\n\n");
	hStatus->Text = "Get All with No Parent Files";


}

// <------------------GETS CALLED WHEN GET FILES BUTTON IS CLICKED-------------------------------->
void WPFCppCliDemo::getAllFilesMessage(Object^ sender, RoutedEventArgs^ args) {

	hListBox_DelTab->Items->Clear();
	hListBox_LazyTab->Items->Clear();
	String^ category;
	if (hTabControl->SelectedIndex == 3) {
		if (hRadioButtonDel1->IsChecked) {
			category = "categoryA";
		}
		else if (hRadioButtonDel2->IsChecked) {
			category = "categoryB";
		}
		else {
			category = "categoryC";
		}
	}

	else if (hTabControl->SelectedIndex == 4) {
		if (hRadioButtonLazy1->IsChecked) {
			category = "categoryA";
		}
		else if (hRadioButtonLazy2->IsChecked) {
			category = "categoryB";
		}
		else {
			category = "categoryC";
		}
	}
	String^ message = category + ",GetFilesList";

	pSendr_->postMessage(toStdString(message));

	Console::Write("\n======>>>>>>  sent message to Get List of all from category: " + category + "\n\n");
	hStatus->Text = "Get All Files";

}
// <------------------GETS CALLED WHEN IIS BUTTON IS CLICKED-------------------------------->
void WPFCppCliDemo::getIISFiles(Object^ sender, RoutedEventArgs^ args) {
	String^ pathFromTextBox = hTextBoxIIS->Text;
	if (pathFromTextBox->Length == 0) {
		hStatus->Text = "Please Enter a Path in the Text Box";
		return;
	}
	String^ category;
	if (hRadioButtonIIS1->IsChecked) {
		category = "categoryA";
	}
	else if (hRadioButtonIIS2->IsChecked) {
		category = "categoryB";
	}
	else {
		category = "categoryC";
	}
	String^ message = pathFromTextBox + "," + category + ",IISFiles";

	pSendr_->postMessage(toStdString(message));

	Console::Write("\n======>>>>>>  sent message to download[IIS] all Files from category: " + category + "\n\n");
	hStatus->Text = "Please check directory: \"" + pathFromTextBox + "/" + category + "\" to get the downloaded file";
}

// <------------------GETS CALLED WHEN LAZY BUTTON IS CLICKED-------------------------------->
void WPFCppCliDemo::getLazyFiles(Object^ sender, RoutedEventArgs^ args) {
	if (hListBox_LazyTab->SelectedItems->Count == 0) {
		hStatus->Text = "Please Select A File For Lazy Download ";
		return;
	}
	String^ message;
	message += hListBox_LazyTab->SelectedItem + ",";
	String^ category;
	if (hRadioButtonLazy1->IsChecked) {
		category = "categoryA";
	}
	else if (hRadioButtonLazy2->IsChecked) {
		category = "categoryB";
	}
	else {
		category = "categoryC";
	}
	message += category + ",LazyDownload";

	pSendr_->postMessage(toStdString(message));

	Console::Write("\n======>>>>>>  sent message to Lazy Download from category: " + category + "\n\n");
	hStatus->Text = "Starting Lazy Downloading";
	hListBox_LazyTab->Items->Clear();

}

// <------------------GETS CALLED WHEN DELETE BUTTON IS CLICKED-------------------------------->
void WPFCppCliDemo::deleteFilesMessage(Object^ sender, RoutedEventArgs^ args) {
	String^ message;
	int count = hListBox_DelTab->SelectedItems->Count;
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
		for each (String^ item in hListBox_DelTab->SelectedItems)
		{
			message += item + ",";
		}
	}

	else {
		hStatus->Text = "Please Select Some Files To Delete";
		return;
	}

	String^ category;
	if (hRadioButtonDel1->IsChecked) {
		category = "categoryA";
	}
	else if (hRadioButtonDel2->IsChecked) {
		category = "categoryB";
	}
	else {
		category = "categoryC";
	}
	message += category + ",deleteSelectedFiles";

	pSendr_->postMessage(toStdString(message));

	Console::Write("\n======>>>>>>  sent message to Delete Files from category: " + category + "\n\n");
	hStatus->Text = "Deleting Selected Files";
	hListBox_DelTab->Items->Clear();
	getAllFilesMessage(sender, args);
}
// <------------------GETS CALLED WHEN CLEAR BUTTON IS CLICKED-------------------------------->
void WPFCppCliDemo::clearListBox_DelView(Object^ sender, RoutedEventArgs^ args) {
	if (hTabControl->SelectedIndex == 3)
		hListBox_DelTab->Items->Clear();
	else if (hTabControl->SelectedIndex == 4)
		hListBox_LazyTab->Items->Clear();
}




//////////////////  My code /////////////////
// Send File List View
// Add this when we need to send only selected files
void WPFCppCliDemo::getItemsFromList(Object^ sender, RoutedEventArgs^ args)
{

	int index = 0;
	int count = hListBox->SelectedItems->Count;
	hStatus->Text = "Show Selected Items";
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
		for each (String^ item in hListBox->SelectedItems)
		{
			items[index++] = item;
			Console::Write("\n  File is :" + item);

		}
	}

	hListBox->Items->Clear();
	if (count > 0) {
		for each (String^ item in items)
		{
			hListBox->Items->Add(item);
		}
	}
}



// <------------------GETS CALLED WHEN SELECT DIRECTORY BUTTON IS CLICKED-------------------------------->
void WPFCppCliDemo::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	hListBox->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hFolderBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hFolderBrowserDialog->SelectedPath;
		std::cout << "\n  Opening Folder: \"" << toStdString(path) << "\"";
		array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
		for (int i = 0; i < files->Length; ++i)
			hListBox->Items->Add(files[i]);
		array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
		for (int i = 0; i < dirs->Length; ++i)
			hListBox->Items->Add(L"<> " + dirs[i]);
	}
	hStatus->Text = "Only \".cpp\" and \".h\" Files will be sent from the list";
}

// <------------------ONLOAD-------------------------------->
void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Window loaded");
}
// <------------------UNLOAD-------------------------------->
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
	Console::Write("\n  Window closing");
}
// <-----------------DISPLAY REQUIREMENT-------------------------------->
void WPFCppCliDemo::displayRequirement1() {
	std::cout << "\n  Used Visual Studio 2015 and its C++ Windows console projects, as provided in the ECS computer labs--> check code for the same\n";
	std::cout << "\n  Used WPF to provide User interface-->> As appeared on the screen\n";
}
// <-----------------DISPLAY REQUIREMENT-------------------------------->
void WPFCppCliDemo::displayRequirement2() {
	std::cout << "\n  use the C++ standard library's streams for all console I/O and new and delete for all heap-based memory management--> check code for the same\n";
}
// <-----------------DISPLAY REQUIREMENT-------------------------------->
void WPFCppCliDemo::displayRequirement3() {
	std::cout << "\n Provided a Repository program that has functionality to publish[C++ source code files], as linked web pages--> check code for the same. \n";
}
// <-----------------DISPLAY REQUIREMENT-------------------------------->
void WPFCppCliDemo::displayRequirement4() {
	std::cout << "\n Provided a publishing process to satisfy the requirements of CodePublisher developed in Project #3.. \n";
	std::cout << "\n This can be verified by following the below steps: \n";
	std::cout << "\nFrom the GUI:\n";
	std::cout << "\n1)--> Select the Send Files Tab, Choose files using \"Select Directory Button\" and send files.\n";
	std::cout << "\n2)--> Now Select the Publish Files Tab and choose the same category and press \"Publish Button\" to publish the sent files.\n";
	std::cout << "\n3)--> Now Select the Download Files Tab and choose the same category and press \"Downlaod Button\".\n";
	std::cout << "\n4)--> Now Files will be downloaded on the \"Client\" at the specified path (can be check from Text Box on GUI)-> and then a Index page will appear displaying all the downloaded files.\n";
}
// <-----------------DISPLAY REQUIREMENT-------------------------------->
void WPFCppCliDemo::displayRequirement5() {
	std::cout << "\n Provided a Client program that can upload files and view Repository contents.\n";
	std::cout << "\n This can be verified by following the below steps: \n";
	std::cout << "\nFrom the GUI:\n";
	std::cout << "\n1)--> [Upload Files Functionality]===>> Select the Send Files Tab, Choose files using \"Select Directory Button\" and send files.\n";
	std::cout << "\n2)--> To View the Repository contents as described on the purpose section--> Different tabs are provided on the GUI.\n";
}
// <-----------------DISPLAY REQUIREMENT-------------------------------->
void WPFCppCliDemo::displayRequirement6() {
	std::cout << "\n Provided a message-passing communication system, based on Sockets, used to access the Repository's functionality from another process or machine--> check code for the same\n";
}
// <-----------------DISPLAY REQUIREMENT-------------------------------->
void WPFCppCliDemo::displayRequirement7() {
	std::cout << "\n The support for passing HTTP style messages using one-way message passing is provided --> check code for the same. \n";
}
// <-----------------DISPLAY REQUIREMENT-------------------------------->
void WPFCppCliDemo::displayRequirement8() {
	std::cout << "\n Provided a communication system that support sending and receiving streams of bytes and Streams are established with an initial exchange of messages.\n";
	std::cout << "\n This can be verified by sending Files using Send Files Tab on GUI or can be verified by looking at code.\n";
}
// <-----------------DISPLAY REQUIREMENT-------------------------------->
void WPFCppCliDemo::displayRequirement9() {
	std::cout << "\n An automated test unit test suite has been included to meet all requirements \n";
	std::cout << "\n For Verification-- check Windows.cpp (Executive for Project#4) in the CppCli-WPF-App Project " << "\n";
}
// <-----------------DISPLAY REQUIREMENT-------------------------------->
void WPFCppCliDemo::displayRequirement10() {
	std::cout << "\nProvided a Lazy Download strategy as stated in the Requirement\n";
	std::cout << "\nThis can be verified from GUI:\n";
	std::cout << "\n1)--> Before checking this functionality--> Firstly, Send files and Publish Files operation needs to be performed \n";
	std::cout << "\n2)--> Now, Open Lazy Download Tab and select a category and press get all files. \n";
	std::cout << "\n3)--> Choose any one file from the List Box and press Lazy Download Button==> Index page will open displaying all downloaded files\n";
}
// <-----------------DISPLAY REQUIREMENT-------------------------------->
void WPFCppCliDemo::displayRequirement11() {
	std::cout << "\nProvided a IIS feature as stated in the Requirement\n";
	std::cout << "\nThis can be verified from GUI:\n";
	std::cout << "\n1)--> Open IIS Download Tab and select a category and write path of virtual directory in the Text Box. \n";
	std::cout << "\n2)--> Now, press IIS Button and it will open a browser(This works for port 8080) --> For any other port user needs to manually write the path in the browser. \n";
}

// displays all the requirements
void WPFCppCliDemo::displayRequirements() {
	displayPattern(1);
	displayRequirement1();
	displayPattern(2);
	displayRequirement2();
	displayPattern(3);
	displayRequirement3();

	displayPattern(4);
	displayRequirement4();

	displayPattern(5);
	displayRequirement5();

	displayPattern(6);
	displayRequirement6();

	displayPattern(7); //write after confirming
	displayRequirement7();
	displayPattern(8);
	displayRequirement8();
	displayPattern(9); // write after confirming
	displayRequirement9();
	displayPattern(10);
	displayRequirement10();
	displayPattern(11);
	displayRequirement11();
	std::cout << "\n\n\n================================================================================================================================================\n\n";
}
// <-----------------DISPLAY PATTERN-------------------------------->
void WPFCppCliDemo::displayPattern(int reqNumber) {
	std::cout << "\n\n\n================================================================================================================================================";
	std::cout << "\n  Displaying Requirement " << reqNumber << ": =============>";
	std::cout << "\n================================================================================================================================================\n";
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
	Console::WriteLine(L"\n Starting WPFCppCliDemo");

	Application^ app = gcnew Application();
	app->Run(gcnew WPFCppCliDemo());
	Console::WriteLine(L"\n\n");
}