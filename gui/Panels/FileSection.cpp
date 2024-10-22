#include "FileSection.h"

#include "audio/AudioHandler.h"
#include "audio/SD9File.h"

FileSection::FileSection(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxButton* add_entity_button = new wxButton(this, ID_ADD_ENTITY, "+");

	this->_contentsTree = new wxTreeCtrl(this, wxID_ANY, wxPoint(200, 200),
										 wxSize(150, 500), wxTR_DEFAULT_STYLE);
	sizer->Add(add_entity_button);
	sizer->Add(_contentsTree);

	/*
// Add a list control for objects in the scene
wxListCtrl* objectList = new wxListCtrl(this, wxID_ANY, wxPoint(10, 50),
										wxSize(360, 200), wxLC_REPORT);
objectList->InsertColumn(0, "Objects", wxLIST_FORMAT_LEFT, 300);

// Add example objects to the list
objectList->InsertItem(0, "Object 1");
objectList->InsertItem(1, "Object 2");
objectList->InsertItem(2, "Object 3");
	*/

	this->SetSizer(sizer);
}

void FileSection::OnButtonClick(wxCommandEvent& event) {
	wxMessageBox("Button clicked!", "Info", wxOK | wxICON_INFORMATION);
}

class ContentsTreeItemData : public wxTreeItemData {
   public:
	ContentsTreeItemData(const fs::path& filePath) : filePath(filePath) {}

	fs::path GetFilePath() const { return filePath; }

   private:
	fs::path filePath;
};

void FileSection::OnClickContentsFile(wxTreeEvent& event) {
	wxTreeItemId id = event.GetItem();

	if (id.IsOk()) {
		// wxMessageBox("You clicked " + contentsTree->GetItemText(id));

		auto treeItem = *(ContentsTreeItemData*)_contentsTree->GetItemData(id);

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

void FileSection::OnOpenNewContentsFolder(wxCommandEvent& event) {
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

void FileSection::BuildContentsTree() {
	this->ResetContentsTree();

	auto rootID = this->_contentsTree->AddRoot(
		this->_rootDir->getName(), -1, -1,
		new ContentsTreeItemData(this->_rootDir->getPath()));

	BuildContentsTreeRecursive(rootID, this->_rootDir);
}

void FileSection::BuildContentsTreeRecursive(const wxTreeItemId& currentNodeID,
											 Directory* currentDir) {
	// Check everything in the current directory
	for (const auto& file : currentDir->getFiles()) {
		// std::cout << file << std::endl;

		auto name = file.filename().string();
		if (!name.ends_with(".sd9")) continue;

		this->_contentsTree->AppendItem(currentNodeID, file.filename().string(),
										-1, -1, new ContentsTreeItemData(file));
	}

	for (const auto& dir : currentDir->getDirs()) {
		// std::cout << dir->getPath() << std::endl;
		auto newNodeID = this->_contentsTree->AppendItem(
			currentNodeID, dir->getName(), -1, -1,
			new ContentsTreeItemData(dir->getPath()));
		BuildContentsTreeRecursive(newNodeID, dir);
	}
}

void FileSection::ChangeContentsDirectory(const fs::path& newDir) {
	this->_rootDir = FileHandler::scanDirectory(newDir);
	this->BuildContentsTree();
	_contentsTree->ExpandAll();
}

void FileSection::ResetContentsTree() {
	// Delete existing tree
	if (this->_contentsTree != nullptr) {
		delete this->_contentsTree;
	}

	// Create a new tree
	this->_contentsTree = new wxTreeCtrl(this, wxID_ANY, wxPoint(200, 200),
										 wxSize(150, 500), wxTR_DEFAULT_STYLE);
}
