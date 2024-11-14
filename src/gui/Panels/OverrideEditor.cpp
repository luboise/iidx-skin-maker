#include "OverrideEditor.h"
#include "gui/Components/Buttons.h"
#include "gui/Forms/CallbackBoxes/NumberControl.h"
#include "mod_manager/Overrides/SD9Override.h"

#include "utils.h"

struct OverrideUpdateSet {
    wxStaticText* label{nullptr};
    wxTextCtrl* textCtrl{nullptr};
    wxButton* resetButton{nullptr};

    void addTo(wxSizer* sizer) const {
        sizer->Add(label, 0);
        sizer->Add(textCtrl, 1, wxEXPAND);
        sizer->Add(resetButton, 0, wxALIGN_RIGHT);
    }
};

template <typename T>
OverrideUpdateSet createUpdateSet(wxWindow* parent, const wxString& labelText,
                                  const T& value, std::optional<T>& override) {
    auto* label{new wxStaticText(parent, wxID_ANY, labelText)};
    auto* control{new NumberControl(parent, value, override)};

    auto reset_lambda{
        [control]() { control->reset(); },
    };
    auto* reset_button{Buttons::Reset(reset_lambda, parent)};
    return {label, control, reset_button};
};

void OverrideEditor::onOverrideUpdated(Override* override) {
    this->setOverride(override);
};

void OverrideEditor::onSelectedPathChanged(const PathChangedData& data) {
    this->setOverride(data.override);
};

void OverrideEditor::setOverride(Override* override) {
    this->Clear(true);

    this->currentOverride_ = override;

    if (override == nullptr) {
        this->Layout();
        return;
    }

    auto override_type{override->getType()};
    if (override_type == SD9_TYPE) {
        auto* casted_override{dynamic_cast<SD9Override*>(override)};

        this->edit(*casted_override);
    } else {
        Utils::Dialog::Error(
            "Invalid override type encountered in OverrideEditor: " +
            override_type);
        return;
    }

    this->GetContainingWindow()->Layout();
    // this->Layout();
};

void OverrideEditor::edit(SD9Override& override) {
    const SD9Replacement& override_data{override.getOverrideData()};
    SD9InfoOverride& overrideables{override.getOverrideList()};

    const SD9Info& base_info = override_data.base_info;

    auto* parent{this->GetContainingWindow()};

    // 'Replaced by' label
    /*
auto* temp_sizer_label{new wxStaticText(
    parent, wxID_ANY,
    "Replaced by " + override.getReplacementFilepath().string())};
            */

    OverrideUpdateSet audio_index{createUpdateSet(parent, "Audio Index",
                                                  base_info.unique_index,
                                                  overrideables.unique_index)};

    auto* enabled_label{new wxStaticText(parent, wxID_ANY, "Loop Enabled")};
    auto* new_checkbox{new wxCheckBox(parent, wxID_ANY, "")};
    new_checkbox->SetValue(base_info.loop_enabled != 0);

    OverrideUpdateSet loopStartDetails{
        createUpdateSet(parent, "Loop Start", base_info.loop_start_byte_offset,
                        overrideables.loop_start_byte_offset)};

    OverrideUpdateSet loopEndDetails{
        createUpdateSet(parent, "Loop End", base_info.loop_end_byte_offset,
                        overrideables.loop_end_byte_offset)};

    new_checkbox->Bind(wxEVT_CHECKBOX, [start_ctrl = loopStartDetails.textCtrl,
                                        end_ctrl = loopEndDetails.textCtrl](
                                           wxCommandEvent& event) {
        start_ctrl->Enable(event.IsChecked());
        end_ctrl->Enable(event.IsChecked());
    });

    // 0, info->audio_size);   // Bounds of number callback box ,implement later

    /*
constexpr auto PADDING_SIZE = 10;

auto flags = wxSizerFlags().Expand().Border(wxALL, PADDING_SIZE).GetFlags();
    */

    this->Add(enabled_label);
    this->Add(new_checkbox);
    // TODO: Fix this up to actually work again
    this->Add(new wxButton(parent, wxID_ANY));

    loopStartDetails.addTo(this);
    loopEndDetails.addTo(this);
    audio_index.addTo(this);

    /*
auto* audio_index_ctrl{
    new wxTextCtrl(parent, wxID_ANY, std::to_string(base_info.unique_index),
                   wxDefaultPosition, wxDefaultSize, wxTE_READONLY)};

audio_index_ctrl->SetBackgroundColour(*wxLIGHT_GREY);
audio_index->Add(audio_index_ctrl);
    */
}
