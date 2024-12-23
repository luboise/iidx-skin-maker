#include "ModSettingsPopup.h"

#include <wx/event.h>
#include <wx/font.h>
#include <wx/gdicmn.h>
#include <wx/layout.h>
#include <wx/sizer.h>

#include "gui/Forms/CallbackBoxes/TextEditBox.h"
#include "gui/Forms/NumberCallbackBox.h"
#include "utils.h"

enum {
    MOD_NAME = wxID_HIGHEST + 1,
};

ModSettingsPopup::ModSettingsPopup(Mod& mod)
    : wxDialog(nullptr, wxID_ANY, "Mod Settings", wxDefaultPosition,
               wxDefaultSize),
      _mod(mod) {
    /*
    auto name_callback = [&mod](wxCommandEvent& event) {
            mod.name = event.GetString();
    };
    */

    // Contents sizer

    auto* data_sizer{new wxBoxSizer(wxHORIZONTAL)};

    auto* dp_label = new wxStaticText(this, wxID_ANY, "Data path");
    data_sizer->Add(dp_label, 0);

    data_sizer->AddStretchSpacer();

    auto* new_preview{new wxTextCtrl(this, MOD_NAME, mod.root_dir.string(),
                                     wxDefaultPosition, wxDefaultSize,
                                     wxTE_READONLY)};
    _dataDirPreview = new_preview;
    data_sizer->Add(_dataDirPreview, 3);

    auto* dp_button = new wxButton(this, MOD_NAME, "..");
    dp_button->SetMinSize(wxSize(35, -1));

    // dp_selector->SetMinSize(wxSize(40, 20));
    dp_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
                    &ModSettingsPopup::onOpenNewDataDir, this);

    data_sizer->AddSpacer(3);

    data_sizer->Add(dp_button, 0);

    // All other sizers

    wxSizer* name_sizer{new TextEditBox(this, "Mod Name", mod.name)};

    wxSizer* game_version =
        new NumberEditBox(this, "Game Version", mod.game_version);

    wxSizer* version_major =
        new NumberEditBox(this, "Version Major", mod.version_major);
    wxSizer* version_minor =
        new NumberEditBox(this, "Version Minor", mod.version_minor);

    auto* create_button = new wxButton(this, wxID_ADD, "Create Mod");
    create_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
                        [this](wxCommandEvent&) { this->Close(); });

    auto* main_sizer{new wxBoxSizer(wxVERTICAL)};
    main_sizer->SetMinSize(wxSize(600, -1));

    constexpr auto PADDING_SIZE = 10;

    auto flags = wxSizerFlags().Expand().Border(wxALL, 10).GetFlags();

    main_sizer->Add(name_sizer, 1, flags, PADDING_SIZE);
    main_sizer->Add(data_sizer, 1, flags, PADDING_SIZE);
    main_sizer->Add(game_version, 1, flags, PADDING_SIZE);
    main_sizer->Add(version_major, 1, flags, PADDING_SIZE);
    main_sizer->Add(version_minor, 1, flags, PADDING_SIZE);
    main_sizer->AddSpacer(PADDING_SIZE);
    main_sizer->Add(create_button, 0, wxSizerFlags().Align(wxCENTER).GetFlags(),
                    PADDING_SIZE * 2);
    main_sizer->AddSpacer(PADDING_SIZE);

    this->SetSizer(main_sizer);
    this->Fit();
}

void ModSettingsPopup::onOpenNewDataDir(wxCommandEvent& event) {
    fs::path new_dir = Utils::directoryPopup("Select your data directory.");

    if (fs::exists(new_dir)) {
        _mod.root_dir = new_dir;
        _dataDirPreview->SetValue(new_dir.string());
    }
}
