#include "OverrideEditor.h"
#include <filesystem>

#include "audio/AudioHandler.h"
#include "gui/Forms/ModLoopChanger.h"
#include "mod_manager/Overrides/SD9Override.h"

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
        SD9File* audio_file = nullptr;

        try {
            ifstream ifs(full_path);
            audio_file = new SD9File(ifs);
            if (audio_file == nullptr) {
                throw std::logic_error(
                    "Initialised audio_file pointer is equal to nullptr.");
            }
        } catch (std::exception& e) {
            Utils::Dialog::Error("Unable to read SD9 file " +
                                 full_path.string() + ".\nError: " + e.what());
            return;
        }

        SD9Info info = audio_file->getSD9Info();

        // TODO: Move this somewhere else
        auto* preview_audio_button = new wxButton(parent, wxID_ANY, "Preview");
        preview_audio_button->Bind(
            wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent&) {
                try {
                    AudioHandler::PlaySD9(_data.get_full_path());
                } catch (std::exception& e) {
                    Utils::Dialog::Error("Unable to preview SD9 file " +
                                         _data.get_full_path().string() +
                                         ".\nError: " + e.what());
                }
            });
        this->Add(preview_audio_button);

        Override* override = _data.override;
        if (override != nullptr) {
            auto* dnc = (SD9Override*)(override);
            fs::path replacement_filepath = dnc->getReplacementFilepath();

            auto* temp_sizer = new wxBoxSizer(wxHORIZONTAL);
            temp_sizer->Add(new wxStaticText(
                parent, wxID_ANY,
                "Replaced by " + replacement_filepath.string()));

            this->Add(temp_sizer);
        }

        auto* audio_index = new wxBoxSizer(wxHORIZONTAL);
        audio_index->Add(new wxStaticText(parent, wxID_ANY, "Audio Index"));
        audio_index->AddStretchSpacer();
        auto* audio_index_ctrl =
            new wxTextCtrl(parent, wxID_ANY, std::to_string(info.unique_index),
                           wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
        audio_index_ctrl->SetBackgroundColour(*wxLIGHT_GREY);
        audio_index->Add(audio_index_ctrl);

        this->Add(audio_index);

        auto* loop_sizer = new ModLoopChanger(parent, &info);

        this->Add(loop_sizer);
    }

    if (parent != nullptr) {
        parent->Layout();
    }
}
