#include "MainFrame.h"

#include <portaudio.h>
#include <wx/msgdlg.h>
#include <wx/mstream.h>
#include <wx/sound.h>

#include <fstream>
#include <stdexcept>

#include "../audio/AudioHandler.h"
#include "../audio/SD9File.h"
#include "../files/FileHandler.h"

using std::ifstream;

class ContentsTreeItemData : public wxTreeItemData {
   public:
	ContentsTreeItemData(const fs::path& filePath) : filePath(filePath) {}

	fs::path GetFilePath() const { return filePath; }

   private:
	fs::path filePath;
};

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

	// ChangeContentsDirectory("/home/luboise/iidx-skin-maker/test/contents/data");
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

void MainFrame::OnOpenNewContentsFolder(wxCommandEvent& event) {
	const wxString& dir = wxDirSelector("Select your IIDX Contents Folder");

	if (dir.empty()) {
		wxLogMessage("Folder not specified. Keeping current folder.");
		return;
	}

	string path = dir.ToStdString();

	// auto pos = path.find("\\");
	// while (pos != string::npos) {
	// 	path.replace(pos, 1, "/");
	// 	pos = path.find("\\");
	// }

	ChangeContentsDirectory(path);
}

void MainFrame::OnClickContentsFile(wxTreeEvent& event) {
	wxTreeItemId id = event.GetItem();

	if (id.IsOk()) {
		// wxMessageBox("You clicked " + contentsTree->GetItemText(id));

		auto treeItem = *(ContentsTreeItemData*)contentsTree->GetItemData(id);

		fs::path p = treeItem.GetFilePath();

		if (fs::exists(p)) {
			std::string path = p.string();

			try {
				ifstream ifs(p);

				SD9File* song = new SD9File(ifs);
				AudioHandler::PlaySD9(*song);

				// AudioHandler::PlayPCM(bm);
			} catch (std::runtime_error e) {
				std::stringstream ss;
				ss << "ERROR: Unable to play file from path \"" << path
				   << "\". \nError message: " << e.what() << ")";
				wxMessageBox(ss.str().c_str());

			} catch (std::exception e) {
				wxMessageBox("An unhandled exception has occurred.");
			}
		}

		// ((ContentsTreeItemData*)contentsTree->GetItemData(id))->GetFilePath();
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

		auto name = file.filename().string();
		if (!name.ends_with(".sd9")) continue;

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
