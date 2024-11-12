#include "OverrideEditor.h"
#include "gui/Forms/CallbackBoxes/NumberOverrideBox.h"
#include "mod_manager/Overrides/SD9Override.h"

#include "gui/Forms/ModLoopChanger.h"

#include "utils.h"

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
    auto* temp_sizer{new wxBoxSizer(wxHORIZONTAL)};
    auto* temp_sizer_label{new wxStaticText(
        parent, wxID_ANY,
        "Replaced by " + override.getReplacementFilepath().string())};
    temp_sizer->Add(temp_sizer_label);

    wxSizer* audio_index{new NumberOverrideBox(parent, "Audio Index",
                                               base_info.unique_index,
                                               overrideables.unique_index)};

    auto* loop_changer{new ModLoopChanger(parent, base_info, overrideables)};
    this->Add(loop_changer);

    /*
auto* audio_index_ctrl{
    new wxTextCtrl(parent, wxID_ANY, std::to_string(base_info.unique_index),
                   wxDefaultPosition, wxDefaultSize, wxTE_READONLY)};

audio_index_ctrl->SetBackgroundColour(*wxLIGHT_GREY);
audio_index->Add(audio_index_ctrl);
    */

    this->Add(temp_sizer);
    this->Add(audio_index);
};

/*
#include <filesystem>

#include "gui/Forms/ModLoopChanger.h"

#include "utils.h"

void OverrideEditor::update(wxWindow* parent) {
    this->Clear(true);

    fs::path full_path = _data.get_full_path();

    if (full_path == "") {
        return;
    }

    if (!fs::exists(full_path)) {
        Utils::Dialog::Error("OverrideEditor: Path " + full_path.string() +
                             " could not be found.");
        return;
    }

    if (full_path.extension() == SUPPORTED_FILE_EXTENSIONS::SD9) {
        if (parent != nullptr) {
            parent->Layout();
        }
    }
}
*/

// SD9InfoOverride& info_override{override->getOverrideInfo()};
//_infoOverride = &info_override;

/*
try {
ifstream ifs(full_path);
auto* read_file{new SD9File(ifs)};

audio_file = read_file;
} catch (std::exception& e) {
Utils::Dialog::Error("Unable to read SD9 file " + full_path.string() +
                     ".\nError: " + e.what());
return;
}

if (audio_file == nullptr) {
throw std::logic_error(
    "Initialised audio_file pointer is equal to nullptr.");
}
*/
