#include "ModLoopChanger.h"
#include <wx/event.h>

#include "audio/SD9File.h"
#include "gui/Forms/CallbackBoxes/NumberOverrideBox.h"

enum {
    MOD_NAME = wxID_HIGHEST + 1,
};

ModLoopChanger::ModLoopChanger(wxWindow* parent, const SD9Info& base_info,
                               SD9InfoOverride& override_info)
    : wxBoxSizer(wxVERTICAL), _info(base_info) {
    auto* enabled_sizer{new wxBoxSizer(wxHORIZONTAL)};

    auto* enabled_label{new wxStaticText(parent, wxID_ANY, "Loop Enabled")};
    enabled_sizer->Add(enabled_label);

    enabled_sizer->AddStretchSpacer();

    auto* new_checkbox{new wxCheckBox(parent, wxID_ANY, "")};
    _enabledCheckbox = new_checkbox;
    _enabledCheckbox->SetValue(base_info.loop_enabled != 0);

    enabled_sizer->Add(_enabledCheckbox);

    auto* loop_start_sizer{new NumberOverrideBox(
        parent, "Loop Start", _info.loop_start_byte_offset,
        override_info.loop_start_byte_offset)};

    auto* loop_end_sizer{
        new NumberOverrideBox(parent, "Loop Start", _info.loop_end_byte_offset,
                              override_info.loop_end_byte_offset)};

    _enabledCheckbox->Bind(wxEVT_CHECKBOX, [loop_start_sizer, loop_end_sizer](
                                               wxCommandEvent& event) {
        loop_start_sizer->enable(event.IsChecked());
        loop_end_sizer->enable(event.IsChecked());
    });

    // 0, info->audio_size);   // Bounds of number callback box ,implement later

    constexpr auto PADDING_SIZE = 10;

    auto flags = wxSizerFlags().Expand().Border(wxALL, PADDING_SIZE).GetFlags();

    this->Add(enabled_sizer, 1, flags, PADDING_SIZE);
    this->Add(loop_start_sizer, 1, flags, PADDING_SIZE);
    this->Add(loop_end_sizer, 1, flags, PADDING_SIZE);
}
