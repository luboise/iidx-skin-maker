#include "MainMenuBar.h"

#include <wx/app.h>
#include <wx/event.h>
#include <wx/utils.h>
#include <wx/wx.h>

#include "mod_manager/ModManager.h"
#include "utils.h"

enum { ID_CHANGE_CONTENTS = wxID_LOWEST - 10, ID_EXPORT_MOD };

MainMenuBar::MainMenuBar(wxWindow* parent) : wxMenuBar() {
    auto& mod_manager = ModManager::getInstance();

    wxMenu* fileMenu = new wxMenu();

    fileMenu->Append(wxID_NEW, "New Mod");
    // fileMenu->Bind(wxID_NEW, [&mod_manager]() { mod_manager.newMod(); });

    fileMenu->Append(wxID_OPEN, "Open Mod");
    // fileMenu->Bind(wxID_OPEN, &MainMenuBar::onOpenMod);

    fileMenu->Append(wxID_SAVE, "Save Mod");
    // fileMenu->Bind(wxID_SAVE, [&mod_manager]() { mod_manager.saveMod(); });

    fileMenu->Append(ID_EXPORT_MOD, "Export Mod");

    fileMenu->Append(wxID_EXIT, "Exit");

    this->Append(fileMenu, "&File");

    wxMenu* modMenu = new wxMenu();
    modMenu->Append(ID_CHANGE_CONTENTS, "Change Contents Folder");
    this->Append(modMenu, "&Mod");

    /*
    modMenu->Bind(
            wxEVT_MENU,
            [parent, &mod_manager](wxCommandEvent& event) {
                    wxDirDialog dlg(parent, _("Locate your contents folder"),
    "", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

                    if (dlg.ShowModal() == wxID_OK) {
                            // Handle the file open logic here
                            mod_manager.changeContentsDirectory(
                                    dlg.GetPath().ToStdString());
                    }
            },
            edit_contents_id);
            */

    wxMenu* helpMenu = new wxMenu();
    helpMenu->Append(wxID_ABOUT, "About");

    this->Append(helpMenu, "&Help");

    Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenuBar::onMenuOptionClicked, this);
}

void MainMenuBar::onOpenMod(wxMenuEvent& event) {
    auto path = Utils::directoryPopup("Locate your contents folder");

    auto& mgr = ModManager::getInstance();
    mgr.loadMod(path);
};

void MainMenuBar::onMenuOptionClicked(wxCommandEvent& event) {
    auto id = event.GetId();

    auto& mmgr = ModManager::getInstance();

    switch (id) {
        case wxID_NEW:
            mmgr.newMod();
            break;
        case wxID_OPEN: {
            auto path = Utils::filePopup("Open your mod", MOD_FILE_EXTENSION);
            mmgr.loadMod(path);
        } break;
        case wxID_SAVE:
            try {
                mmgr.saveMod();
            } catch (std::exception& e) {
                std::cerr << "Unable to save mod.\nError details: " << e.what()
                          << std::endl;
            }
            break;

        case wxID_EXIT:
            wxExit();
            break;

        case ID_EXPORT_MOD: {
            auto path = Utils::directoryPopup("Select the mod's parent folder");
            if (mmgr.exportMod(path)) {
                wxMessageDialog dialog(nullptr, "Mod successfully exported",
                                       "Export Details", wxOK);
                dialog.ShowModal();
            } else {
                wxMessageDialog dialog(nullptr, "Failed to export mod",
                                       "Export Details", wxOK | wxICON_ERROR);
                dialog.ShowModal();
            }

        } break;

        case ID_CHANGE_CONTENTS: {
            auto path = Utils::directoryPopup("Locate your contents folder");
            mmgr.changeContentsDirectory(path);
        } break;
        default:
            std::cerr << "Unhandled event type: " << event.GetId() << std::endl;
    }
};
