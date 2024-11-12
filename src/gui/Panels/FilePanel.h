#pragma once

#include <wx/treectrl.h>
#include <wx/wx.h>

#include "files/Directory.h"
#include "mod_manager/ModObserver.h"

class FilePanel : public wxPanel, public ModObserver {
   public:
    explicit FilePanel(wxWindow* parent);
    void changeContentsDirectory(const fs::path& newDir);

   private:
    void onButtonClick(wxCommandEvent& event);

    void onTreeItemClicked(wxTreeEvent& event);
    void onTreeItemDoubleClicked(wxTreeEvent& event);

    void onOpenNewContentsFolder(wxCommandEvent& event);
    void resetContentsTree();

    void rebuildTree();
    void rebuildTreeRec(const wxTreeItemId& currentNode,
                        const Directory& currentDir);

    void remakeTree();

    void onModChanged(const Mod& mod) override;

    wxTreeCtrl* _treeWidget = nullptr;

    enum { ID_ADD_ENTITY };
};
