#include "EditPanel.h"
#include <wx/event.h>
#include <wx/msgdlg.h>

#include "audio/AudioHandler.h"
#include "audio/SD9File.h"
#include "files/FileHandler.h"

#include "gui/Form/ModLoopChanger.h"
#include "gui/Form/NumberCallbackBox.h"
#include "gui/Form/TextCallbackBox.h"
#include "mod_manager/ModObserver.h"

#include "mod_manager/Overrides/SD9Override.h"

#include "utils.h"

EditPanel::EditPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(main_sizer);

    this->rebuildMainSizer();
}

void EditPanel::onSelectedPathChanged(const PathChangedData& data) {
    _selectedPath = data.path;
    this->rebuildMainSizer();
}

void EditPanel::onOverrideUpdated(const Override* override) {
    this->rebuildMainSizer();
};

void EditPanel::rebuildMainSizer() {
    // Undefined edge case
    if (_selectedPath == "") {
        return;
    }

    if (!fs::exists(_selectedPath)) {
        wxMessageBox(std::string("File ") + _selectedPath.string() +
                     std::string(" does not exist. Please refresh your "
                                 "contents folder."));
        return;
    }

    wxSizer* main_sizer = GetSizer();
    main_sizer->Clear(true);

    if (_selectedPath.extension() == SUPPORTED_FILE_EXTENSIONS::SD9) {
        SD9File* audio_file = nullptr;

        try {
            ifstream ifs(_selectedPath);
            audio_file = new SD9File(ifs);
        } catch (std::exception& e) {
            Utils::Dialog::Error("Unable to read SD9 file " +
                                 _selectedPath.string() +
                                 ".\nError: " + e.what());
            return;
        }

        SD9Info* test = new SD9Info(audio_file->getSD9Info());

        delete audio_file;

        auto* preview_sizer = new wxBoxSizer(wxHORIZONTAL);

        preview_sizer->Add(new wxStaticText(this, wxID_ANY, "Game File"));

        auto* preview_path =
            new wxTextCtrl(this, wxID_ANY, _selectedPath.string());
        preview_sizer->Add(preview_path);

        auto* preview_audio_button = new wxButton(this, wxID_ANY, "Preview");
        preview_audio_button->Bind(
            wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent&) {
                try {
                    AudioHandler::PlaySD9(_selectedPath);
                } catch (std::exception& e) {
                    Utils::Dialog::Error("Unable to preview SD9 file " +
                                         _selectedPath.string() +
                                         ".\nError: " + e.what());
                }
            });
        preview_sizer->Add(preview_audio_button);

        auto* select_button = new wxButton(this, wxID_ANY, "Change");
        select_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, preview_path,
                                                           test](
                                                              wxCommandEvent&) {
            fs::path extension = _selectedPath.extension();

            if (extension == SUPPORTED_FILE_EXTENSIONS::SD9) {
                fs::path new_file =
                    Utils::filePopup("Select replacement audio", "wav");

                if (fs::exists(new_file)) {
                    try {
                        auto* override = new SD9Override(_selectedPath, *test);
                        override->setReplacementFile(new_file);

                        preview_path->SetValue("Implement me.");

                        ModManager::getInstance().addOverride(_selectedPath,
                                                              override);
                    } catch (std::exception& e) {
                        Utils::Dialog::Error(
                            "Unable to create SD9 override for " +
                            _selectedPath.string() + ".\nError: " + e.what());
                    }
                }
            }
        });

        preview_sizer->Add(select_button);

        main_sizer->Add(preview_sizer);

        Override* override =
            ModManager::getInstance().getOverride(_selectedPath);
        if (override != nullptr) {
            auto* temp_sizer = new wxBoxSizer(wxHORIZONTAL);
            temp_sizer->Add(new wxStaticText(
                this, wxID_ANY,
                "Replaced by " + (dynamic_cast<SD9Override*>(override))
                                     ->getReplacementFile()
                                     .string()));

            main_sizer->Add(temp_sizer);
        }

        auto* audio_index = new wxBoxSizer(wxHORIZONTAL);
        audio_index->Add(new wxStaticText(this, wxID_ANY, "Audio Index"));
        audio_index->AddStretchSpacer();
        auto* audio_index_ctrl =
            new wxTextCtrl(this, wxID_ANY, std::to_string(test->unique_index),
                           wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
        audio_index_ctrl->SetBackgroundColour(*wxLIGHT_GREY);
        audio_index->Add(audio_index_ctrl);
        main_sizer->Add(audio_index);

        auto* loop_sizer = new ModLoopChanger(this, test);

        main_sizer->Add(loop_sizer);
    }

    this->Layout();
}
