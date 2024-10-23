#include "MainMenuBar.h"

#include <wx/event.h>
#include <wx/wx.h>

#include "mod_manager/ModManager.h"

MainMenuBar::MainMenuBar(wxWindow* parent) : wxMenuBar() {
	wxMenu* fileMenu = new wxMenu();

	fileMenu->Append(wxID_OPEN, "Change Contents Folder");
	fileMenu->Append(wxID_EXIT, "Exit");

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, "About");

	this->Append(fileMenu, "&File");
	this->Append(helpMenu, "&Help");

	auto& mod_manager = ModManager::getInstance();

	fileMenu->Bind(
		wxEVT_MENU,
		[parent, &mod_manager](wxCommandEvent& event) {
			wxDirDialog dlg(parent, _("Locate your contents folder"), "",
							wxFD_OPEN | wxFD_FILE_MUST_EXIST);

			if (dlg.ShowModal() == wxID_OK) {
				// Handle the file open logic here
				mod_manager.changeContentsDirectory(
					dlg.GetPath().ToStdString());
			}
		},

		//	EditorState::Get().loadScene,

		wxID_OPEN);
}
