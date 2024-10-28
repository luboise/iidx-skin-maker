#include "EditPanel.h"

#include <wx/event.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <stdexcept>

#include "audio/AudioHandler.h"
#include "audio/SD9File.h"
#include "files/FileHandler.h"

#include "gui/Form/ModLoopChanger.h"
#include "gui/Form/NumberCallbackBox.h"
#include "gui/Form/TextCallbackBox.h"
#include "mod_manager/ModObserver.h"

#include "mod_manager/Overrides/SD9Override.h"

#include "utils.h"

class FileDetailsSizer : public wxBoxSizer, public ModObserver {
   public:
    explicit FileDetailsSizer(wxWindow* parent) : wxBoxSizer(wxHORIZONTAL) {
        this->Add(new wxStaticText(parent, wxID_ANY, "Game File"));

        _filenameCtrl = new wxTextCtrl(parent, wxID_ANY);
        this->Add(_filenameCtrl);

        this->AddStretchSpacer();

        _overrideButton = new wxButton(parent, wxID_ANY, "Override");

        _overrideButton->Bind(
            wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent&) {
                const fs::path& from_path = _data.path;

                fs::path extension = from_path.extension();

                if (extension == SUPPORTED_FILE_EXTENSIONS::SD9) {
                    fs::path to_path =
                        Utils::filePopup("Select replacement audio", "wav");

                    if (fs::exists(to_path)) {
                        try {
                            SD9Override* override = new SD9Override(from_path);
                            auto unique =
                                std::unique_ptr<SD9Override>(override);
                            override->setReplacementFile(to_path);

                            ModManager::getInstance().addOverride(
                                std::unique_ptr<Override>(override));
                        } catch (std::exception& e) {
                            Utils::Dialog::Error(
                                "Unable to create SD9 override for " +
                                from_path.string() + ".\nError: " + e.what());
                        }
                    }
                }
            });

        this->Add(_overrideButton);
    };

   private:
    PathChangedData _data{};

    wxTextCtrl* _filenameCtrl = nullptr;
    wxButton* _overrideButton = nullptr;

    void refresh() {
        _filenameCtrl->SetValue(_data.path.string());

        if (!fs::exists(_data.path)) {
            _overrideButton->Disable();
        } else {
            _overrideButton->Enable();
            if (_data.override != nullptr) {
                _overrideButton->SetLabel("Reset");
            } else {
                _overrideButton->SetLabel("Override");
            }
        }
    };

    void onSelectedPathChanged(const PathChangedData& data) override {
        _data = data;
        this->refresh();
    };
};

class FilePreviewSizer : public wxBoxSizer, public ModObserver {
   public:
    explicit FilePreviewSizer(wxWindow* parent) : wxBoxSizer(wxVERTICAL) {
        this->update(parent);
    };

   private:
    wxTextCtrl* _previewCtrl = nullptr;
    PathChangedData _data{};

    void onSelectedPathChanged(const PathChangedData& data) override {
        _data = data;
        this->update();
    };

    void update() { this->update(this->GetContainingWindow()); }
    void update(wxWindow* parent) {
        this->Clear(true);

        if (_data.path.extension() == SUPPORTED_FILE_EXTENSIONS::SD9) {
            SD9File* audio_file = nullptr;

            try {
                ifstream ifs(_data.path);
                audio_file = new SD9File(ifs);
                if (audio_file == nullptr) {
                    throw std::logic_error(
                        "Initialised audio_file pointer is equal to nullptr.");
                }
            } catch (std::exception& e) {
                Utils::Dialog::Error("Unable to read SD9 file " +
                                     _data.path.string() +
                                     ".\nError: " + e.what());
                return;
            }

            SD9Info info = audio_file->getSD9Info();

            // TODO: Move this somewhere else
            auto* preview_audio_button =
                new wxButton(parent, wxID_ANY, "Preview");
            preview_audio_button->Bind(
                wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent&) {
                    try {
                        AudioHandler::PlaySD9(_data.path);
                    } catch (std::exception& e) {
                        Utils::Dialog::Error("Unable to preview SD9 file " +
                                             _data.path.string() +
                                             ".\nError: " + e.what());
                    }
                });
            this->Add(preview_audio_button);

            Override* override =
                ModManager::getInstance().getOverride(_data.path);
            if (override != nullptr) {
                auto* temp_sizer = new wxBoxSizer(wxHORIZONTAL);
                temp_sizer->Add(new wxStaticText(
                    parent, wxID_ANY,
                    "Replaced by " + (dynamic_cast<SD9Override*>(override))
                                         ->getReplacementFile()
                                         .string()));

                this->Add(temp_sizer);
            }

            auto* audio_index = new wxBoxSizer(wxHORIZONTAL);
            audio_index->Add(new wxStaticText(parent, wxID_ANY, "Audio Index"));
            audio_index->AddStretchSpacer();
            auto* audio_index_ctrl = new wxTextCtrl(
                parent, wxID_ANY, std::to_string(info.unique_index),
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
};

EditPanel::EditPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    _fileDetailsSizer = new FileDetailsSizer(this);
    main_sizer->Add(_fileDetailsSizer);

    _filePreviewSizer = new FilePreviewSizer(this);
    main_sizer->Add(_filePreviewSizer);

    this->SetSizer(main_sizer);
    // this->Layout();

    // this->update();
}

void EditPanel::onSelectedPathChanged(const PathChangedData& data) {
    _selectedPath = data.path;
}

/*

void EditPanel::onOverrideUpdated(const Override* override) { this->update(); };

void EditPanel::update() {
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

    this->Layout();
}
*/
