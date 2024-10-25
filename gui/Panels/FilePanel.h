#pragma once

#include <wx/treectrl.h>
#include <wx/wx.h>

#include "files/Directory.h"
#include "mod_manager/ModObserver.h"

class FilePanel : public wxPanel, public ModObserver {
   public:
    FilePanel(wxWindow* parent);
    void changeContentsDirectory(const fs::path& newDir);

   private:
    void onButtonClick(wxCommandEvent& event);

    void onClickContentsFile(wxTreeEvent& event);

    void onOpenNewContentsFolder(wxCommandEvent& event);
    void resetContentsTree();

    void rebuildTree();
    void rebuildTreeRec(const wxTreeItemId& currentNode,
                        const Directory& currentDir);

    void remakeTree();

    virtual void onModChanged(const Mod&) override;

    wxTreeCtrl* _treeWidget = nullptr;

    enum { ID_ADD_ENTITY };
};
