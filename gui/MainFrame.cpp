#include "MainFrame.h"

#include <portaudio.h>
#include <wx/msgdlg.h>
#include <wx/mstream.h>
#include <wx/sound.h>

#include "gui/MainMenuBar.h"
#include "gui/Panels/FileSection.h"

MainFrame::MainFrame(wxWindow* parent)
	: wxFrame(NULL, wxID_ANY, "Hello World") {
	auto* sizer = new wxBoxSizer(wxHORIZONTAL);

	_fileSection = new FileSection(this);

	sizer->Add(_fileSection);

	sizer->AddStretchSpacer(1);

	this->SetSizer(sizer);

	this->CreateStatusBar();
	this->SetMenuBar(new MainMenuBar(this));

	// Finalise layout and then change the contents dir
	// _fileSection->ChangeContentsDirectory("/home/lucas/desktop/2dx_fake");
}

/*
MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Hello World") {
	this->contentsTree = nullptr;

	wxMenu* menuFile = new wxMenu;
	menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
					 "Help string shown in status bar for this menu item");
	menuFile->AppendSeparator();
	menuFile->Append(ID_NewContentsFolder, "&Open Game Folder",
					 "Do the thing?????");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);

	// Add file menu and help menu to menu bar
	wxMenuBar* mainBar = new wxMenuBar;
	mainBar->Append(menuFile, "&File");
	mainBar->Append(menuHelp, "&Help");
	SetMenuBar(mainBar);

	//
ChangeContentsDirectory("/home/luboise/iidx-skin-maker/test/contents/data");
	// ChangeContentsDirectory("C:/LDJ-003-2022101900/contents/data");

	CreateStatusBar();
	SetStatusText("Welcome to wxWidgets!");

	Bind(wxEVT_MENU, &MainFrame::OnHello, this, ID_Hello);
	Bind(wxEVT_MENU, &MainFrame::OnOpenNewContentsFolder, this,
		 ID_NewContentsFolder);
	Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
	Bind(wxEVT_TREE_ITEM_ACTIVATED, &MainFrame::OnClickContentsFile, this,
		 wxID_ANY);
}
*/

void MainFrame::OnExit(wxCommandEvent& event) {
	Pa_Terminate();
	Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event) {
	wxMessageBox("This is a wxWidgets Hello World example", "About Hello World",
				 wxOK | wxICON_INFORMATION);
}

void MainFrame::OnHello(wxCommandEvent& event) {
	wxLogMessage("Hello world from wxWidgets!");
}
