// wxWidgets "Hello World" Program

#include <string>
using std::string;

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <wx/treectrl.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "files/FileHandler.h"

namespace fs = std::filesystem;

class SkinMaker : public wxApp
{
public:
	virtual bool OnInit();
};

class MainFrame : public wxFrame
{
public:
	MainFrame();

private:
	// Event Handlers
	void OnHello(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnOpenNewContentsFolder(wxCommandEvent& event);
	void OnClickContentsFile(wxTreeEvent& event);

	fs::path contentsDirPath;
	wxTreeCtrl* contentsTree;
};

enum
{
	ID_Hello = 1,
	ID_NewContentsFolder = 2
};

wxIMPLEMENT_APP(SkinMaker);

bool SkinMaker::OnInit()
{
	MainFrame* frame = new MainFrame();
	frame->Show(true);

	auto x = fs::path("/home/luboise/iidx-skin-maker/test/contents/data");
	FileHandler::getFileTree(x);

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
	wxMenuBar* mainBar = new wxMenuBar;
	mainBar->Append(menuFile, "&File");
	mainBar->Append(menuHelp, "&Help");
	SetMenuBar(mainBar);

	this->contentsTree = new wxTreeCtrl(this, wxID_ANY, wxPoint(200, 200), wxSize(150, 500), wxTR_DEFAULT_STYLE);

	auto rootItem = contentsTree->AddRoot("Root");
	contentsTree->AppendItem(rootItem, "item1");
	contentsTree->AppendItem(rootItem, "item2");
	contentsTree->AppendItem(rootItem, "item3");
	contentsTree->AppendItem(rootItem, "item4");

	auto expandableItem = contentsTree->AppendItem(rootItem, "EXPANDABLE");
	contentsTree->AppendItem(expandableItem, "subitem1");
	contentsTree->AppendItem(expandableItem, "subitem2");
	contentsTree->AppendItem(expandableItem, "subitem3");
	contentsTree->AppendItem(expandableItem, "subitem4");

	contentsTree->ExpandAll();

	CreateStatusBar();
	SetStatusText("Welcome to wxWidgets!");




	Bind(wxEVT_MENU, &MainFrame::OnHello, this, ID_Hello);
	Bind(wxEVT_MENU, &MainFrame::OnOpenNewContentsFolder, this, ID_NewContentsFolder);
	Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
	Bind(wxEVT_TREE_ITEM_ACTIVATED, &MainFrame::OnClickContentsFile, this, wxID_ANY);
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

	this->contentsDirPath = fs::path((string)contentsDialog.GetPath());

	wxLogMessage("%s", contentsDirPath.string());
	FileHandler::getFileTree(contentsDirPath);
}

void MainFrame::OnClickContentsFile(wxTreeEvent& event) {
	wxTreeItemId id = event.GetItem();

	if (id.IsOk()) {
		wxMessageBox("You clicked " + contentsTree->GetItemText(id));
	}
	else {
		throw std::logic_error("Invalid ID from clicked button");
	}

}

