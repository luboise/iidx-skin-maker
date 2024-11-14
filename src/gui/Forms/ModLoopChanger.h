#pragma once

#include <wx/dialog.h>

#include "mod_manager/Overrides/SD9Override.h"

struct SD9Info;

class ModLoopChanger : public wxBoxSizer {
   public:
    explicit ModLoopChanger(wxWindow* parent, const SD9Info& base_info,
                            SD9InfoOverride& override_info);

   private:
    SD9Info _info{};

    wxTextCtrl* _dataDirPreview = nullptr;
    wxCheckBox* _enabledCheckbox = nullptr;

    void refreshControls();
};
