
#include <wx/dialog.h>

#include "mod_manager/Mod.h"

struct SD9Info;

class ModLoopChanger : public wxBoxSizer {
   public:
    explicit ModLoopChanger(wxWindow* parent, SD9Info* info);

   private:
    SD9Info* _info;

    wxTextCtrl* _dataDirPreview = nullptr;
    wxCheckBox* _enabledCheckbox = nullptr;
};
