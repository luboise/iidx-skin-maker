#include "FilePanel.h"

#include <wx/event.h>
#include <wx/msgdlg.h>
#include <wx/treebase.h>

#include <algorithm>
#include <utility>

#include "audio/AudioHandler.h"

#include "gui/Components/VolumeControl.h"

#include "mod_manager/ModManager.h"

#include "gui/MainFrame.h"

FilePanel::FilePanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    wxBoxSizer* sizer{new wxBoxSizer(wxVERTICAL)};

    // Volume control here
    auto* volume_control{new VolumeControl(this)};
    sizer->Add(volume_control);

    // wxButton* add_entity_button = new wxButton(this, ID_ADD_ENTITY, "+");

    auto* tree_widget{new wxTreeCtrl(this, wxID_ANY, wxPoint(200, 200),
                                     wxSize(150, 500), wxTR_DEFAULT_STYLE)};
    this->_treeWidget = tree_widget;
    // sizer->Add(add_entity_button);
    sizer->Add(_treeWidget, 0, wxEXPAND | wxALL);

    /*
// Add a list control for objects in the scene
wxListCtrl* objectList = new wxListCtrl(this, wxID_ANY, wxPoint(10, 50),
                                                                            wxSize(360,
200), wxLC_REPORT); objectList->InsertColumn(0, "Objects", wxLIST_FORMAT_LEFT,
300);

// Add example objects to the list
objectList->InsertItem(0, "Object 1");
objectList->InsertItem(1, "Object 2");
objectList->InsertItem(2, "Object 3");
    */

    this->SetSizer(sizer);

    _treeWidget->Bind(wxEVT_TREE_SEL_CHANGED, &FilePanel::onTreeItemClicked,
                      this);
    _treeWidget->Bind(wxEVT_TREE_ITEM_ACTIVATED,
                      &FilePanel::onTreeItemDoubleClicked, this);
}

void FilePanel::onButtonClick(wxCommandEvent& event) {
    wxMessageBox("Button clicked!", "Info", wxOK | wxICON_INFORMATION);
}

class ContentsTreeItemData : public wxTreeItemData {
   public:
    explicit ContentsTreeItemData(fs::path filePath)
        : _path(std::move(filePath)) {}

    [[nodiscard]] fs::path GetFilePath() const { return _path; }

   private:
    fs::path _path;
};

void FilePanel::onTreeItemClicked(wxTreeEvent& event) {
    wxTreeItemId id = event.GetItem();

    if (!id.IsOk()) {
        throw std::logic_error("Invalid ID from clicked button");
    }

    // wxMessageBox("You clicked " + contentsTree->GetItemText(id));

    auto& treeItem =
        *dynamic_cast<ContentsTreeItemData*>(_treeWidget->GetItemData(id));

    fs::path path = treeItem.GetFilePath();

    ModManager::getInstance().selectPath(path);

    // ((ContentsTreeItemData*)contentsTree->GetItemData(id))->GetFilePath();
}

void FilePanel::onTreeItemDoubleClicked(wxTreeEvent& event) {
    // TODO: Move ID extraction into helper function
    wxTreeItemId id = event.GetItem();

    if (!id.IsOk()) {
        throw std::logic_error("Invalid ID from clicked button");
    }

    // wxMessageBox("You clicked " + contentsTree->GetItemText(id));

    auto& treeItem =
        *dynamic_cast<ContentsTreeItemData*>(_treeWidget->GetItemData(id));

    fs::path path = treeItem.GetFilePath();

    AudioHandler::PlaySD9(path);
};

void FilePanel::onOpenNewContentsFolder(wxCommandEvent& event) {
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

    if (!ModManager::getInstance().changeContentsDirectory(path)) {
        wxMessageBox("Unable to open " + path);
    };
}

void FilePanel::rebuildTree() {
    this->resetContentsTree();

    try {
        auto root_dir = ModManager::getInstance().getRootDir();

        auto* new_tree_item{new ContentsTreeItemData(root_dir.getPath())};
        auto rootID = this->_treeWidget->AddRoot(root_dir.getName(), -1, -1,
                                                 new_tree_item);

        rebuildTreeRec(rootID, root_dir);
    } catch (std::exception e) {
        wxMessageBox("Unable to build tree: " + std::string(e.what()));
    }
}

void FilePanel::rebuildTreeRec(const wxTreeItemId& currentNodeID,
                               const Directory& currentDir) {
    size_t supported_files{0};

    // Check everything in the current directory
    for (const auto& file : currentDir.getFiles()) {
        // std::cout << file << std::endl;
        {}
        auto name = file.filename().string();
        if (!name.ends_with(SUPPORTED_FILE_EXTENSIONS::SD9)) {
            continue;
        }
        supported_files++;

        auto* new_tree_item{new ContentsTreeItemData(file)};
        this->_treeWidget->AppendItem(currentNodeID, file.filename().string(),
                                      -1, -1, new_tree_item);
    }

    const std::list<Directory>& directories{currentDir.getDirs()};

    std::ranges::for_each(directories, [this, currentNodeID](const auto& dir) {
        // std::cout << dir->getPath() << std::endl;

        auto* tree_item_data{new ContentsTreeItemData(dir.getPath())};

        auto newNodeID = this->_treeWidget->AppendItem(
            currentNodeID, dir.getName(), -1, -1, tree_item_data);
        rebuildTreeRec(newNodeID, dir);
    });
}

void FilePanel::resetContentsTree() {
    // Delete existing tree
    /*
    if (this->_treeWidget != nullptr) {
            delete this->_treeWidget;
    }

    // Create a new tree
    this->_treeWidget = new wxTreeCtrl(this, wxID_ANY, wxPoint(200, 200),
                                                                       wxSize(150,
    500), wxTR_DEFAULT_STYLE);
                                                                       */

    _treeWidget->DeleteAllItems();
}

void FilePanel::onModChanged(const Mod& /*mod*/) {
    this->rebuildTree();
    this->_treeWidget->ExpandAll();
};
