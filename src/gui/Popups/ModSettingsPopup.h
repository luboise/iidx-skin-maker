#include <wx/dialog.h>

#include "mod_manager/Mod.h"

class ModSettingsPopup : public wxDialog {
   public:
	ModSettingsPopup(Mod& mod);

   private:
	void onOpenNewDataDir(wxCommandEvent&);
	Mod& _mod;

	wxTextCtrl* _dataDirPreview = nullptr;
};
