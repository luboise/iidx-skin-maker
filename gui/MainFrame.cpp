#include "MainFrame.h"

#include <wx/mstream.h>
#include <wx/sound.h>

#include "../files/FileHandler.h"

class ContentsTreeItemData : public wxTreeItemData {
   public:
	ContentsTreeItemData(const fs::path& filePath) : filePath(filePath) {}

	fs::path GetFilePath() const { return filePath; }

   private:
	fs::path filePath;
};

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Hello World") {
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

	ChangeContentsDirectory("/home/luboise/iidx-skin-maker/test/contents/data");

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

void MainFrame::OnExit(wxCommandEvent& event) { Close(true); }

void MainFrame::OnAbout(wxCommandEvent& event) {
	wxMessageBox("This is a wxWidgets Hello World example", "About Hello World",
				 wxOK | wxICON_INFORMATION);
}

void MainFrame::OnHello(wxCommandEvent& event) {
	wxLogMessage("Hello world from wxWidgets!");
}

void MainFrame::OnOpenNewContentsFolder(wxCommandEvent& event) {
	wxDirDialog contentsDialog(this, "Select your IIDX Contents Folder");

	if (contentsDialog.ShowModal() == wxID_CANCEL) {
		wxLogMessage("Folder not specified. Keeping current folder.");
		return;
	}

	ChangeContentsDirectory(contentsDialog.GetPath().ToStdString());
}

void MainFrame::OnClickContentsFile(wxTreeEvent& event) {
	wxTreeItemId id = event.GetItem();

	if (id.IsOk()) {
		// wxMessageBox("You clicked " + contentsTree->GetItemText(id));

		auto treeItem = *(ContentsTreeItemData*)contentsTree->GetItemData(id);

		fs::path p = treeItem.GetFilePath();

		if (fs::exists(p)) {
			// Read the first 32 bytes of the file and ignore them
			size_t dataSize;
			char* data = FileHandler::readSD9File(p, dataSize);

			// Need to play the audio
		}

		((ContentsTreeItemData*)contentsTree->GetItemData(id))->GetFilePath();
	} else {
		throw std::logic_error("Invalid ID from clicked button");
	}
}

void MainFrame::BuildContentsTree() {
	this->ResetContentsTree();

	auto rootID = this->contentsTree->AddRoot(
		this->rootDir->getName(), -1, -1,
		new ContentsTreeItemData(this->rootDir->getPath()));

	BuildContentsTreeRecursive(rootID, this->rootDir);
}

void MainFrame::BuildContentsTreeRecursive(const wxTreeItemId& currentNodeID,
										   Directory* currentDir) {
	// Check everything in the current directory
	for (const auto& file : currentDir->getFiles()) {
		// std::cout << file << std::endl;
		this->contentsTree->AppendItem(currentNodeID, file.filename().string(),
									   -1, -1, new ContentsTreeItemData(file));
	}

	for (const auto& dir : currentDir->getDirs()) {
		// std::cout << dir->getPath() << std::endl;
		auto newNodeID = this->contentsTree->AppendItem(
			currentNodeID, dir->getName(), -1, -1,
			new ContentsTreeItemData(dir->getPath()));
		BuildContentsTreeRecursive(newNodeID, dir);
	}
}

void MainFrame::ChangeContentsDirectory(const fs::path& newDir) {
	this->rootDir = FileHandler::scanDirectory(newDir);
	this->BuildContentsTree();
	contentsTree->ExpandAll();
}

void MainFrame::ResetContentsTree() {
	// Delete existing tree
	if (this->contentsTree != nullptr) {
		delete this->contentsTree;
	}

	// Create a new tree
	this->contentsTree = new wxTreeCtrl(this, wxID_ANY, wxPoint(200, 200),
										wxSize(150, 500), wxTR_DEFAULT_STYLE);
}