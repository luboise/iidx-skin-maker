// wxWidgets "Hello World" Program

#include <string>
using std::string;

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

class MainFrame : public wxFrame
{
public:
	MainFrame();

private:
	void OnHello(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnOpenNewContentsFolder(wxCommandEvent& event);

	string contentsDirPath;
};

enum
{
	ID_Hello = 1,
	ID_NewContentsFolder = 2
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	MainFrame* frame = new MainFrame();
	frame->Show(true);
	return true;
}



MainFrame::MainFrame()
	: wxFrame(NULL, wxID_ANY, "Hello World")
{
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
	wxMenuBar* mainbar = new wxMenuBar;
	mainbar->Append(menuFile, "&File");
	mainbar->Append(menuHelp, "&Help");
	SetMenuBar(mainbar);

	CreateStatusBar();
	SetStatusText("Welcome to wxWidgets!");

	Bind(wxEVT_MENU, &MainFrame::OnHello, this, ID_Hello);
	Bind(wxEVT_MENU, &MainFrame::OnOpenNewContentsFolder, this, ID_NewContentsFolder);
	Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}

void MainFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("This is a wxWidgets Hello World example",
		"About Hello World", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello world from wxWidgets!");
}

void MainFrame::OnOpenNewContentsFolder(wxCommandEvent& event) {
	wxDirDialog
		contentsDialog(this, "Select your IIDX Contents Folder");

	if (contentsDialog.ShowModal() == wxID_CANCEL) {
		wxLogMessage("Folder not specified. Keeping current folder.");
		return;
	}

	this->contentsDirPath = contentsDialog.GetPath();
	wxLogMessage("%s", contentsDirPath);
}