#pragma once

#include <wx/event.h>
#include <wx/sizer.h>
#include <cmath>
#include "audio/SD9File.h"
#include "mod_manager/ModObserver.h"
#include "mod_manager/Overrides/SD9Override.h"
#include "utils.h"

class FileDetailsPanel : public wxPanel, public ModObserver {
   public:
    explicit FileDetailsPanel(wxWindow* parent) : wxPanel(parent) {
        auto* game_file_label{new wxStaticText(this, wxID_ANY, "Game File")};

        auto* filename_ctrl{new wxTextCtrl(this, wxID_ANY)};
        _filenameCtrl = filename_ctrl;

        auto* file_size_label{new wxStaticText(this, wxID_ANY, "0B")};
        _fileSizeLabel = file_size_label;

        auto* preview_button{new wxButton(this, wxID_ANY, "Preview")};

        preview_button->Bind(
            wxEVT_BUTTON, [this](wxCommandEvent& /* unused */) {
                FileHandler::Preview(this->_data.get_full_path());
            });

        auto* new_override_button{new wxButton(this, wxID_ANY, "Override")};
        _overrideButton = new_override_button;
        _overrideButton->Bind(wxEVT_BUTTON,
                              &FileDetailsPanel::onOverrideButtonClicked, this);

        // Top row
        auto* top_row{new wxBoxSizer(wxHORIZONTAL)};
        top_row->Add(game_file_label, 0);
        top_row->Add(_filenameCtrl, 1, wxEXPAND);
        top_row->Add(_fileSizeLabel, 0);

        // Bottom row
        auto* bottom_row{new wxBoxSizer(wxHORIZONTAL)};
        bottom_row->AddStretchSpacer(1);
        bottom_row->Add(preview_button, 2, wxEXPAND);
        bottom_row->AddStretchSpacer(1);
        bottom_row->Add(_overrideButton, 2, wxEXPAND);
        bottom_row->AddStretchSpacer(1);

        auto* main_sizer{new wxBoxSizer(wxVERTICAL)};
        main_sizer->Add(top_row, 0, wxEXPAND | wxALL, 0);
        main_sizer->Add(bottom_row, 1, wxEXPAND | wxALL, 0);
        main_sizer->AddStretchSpacer();

        this->SetSizer(main_sizer);
    };

   private:
    PathChangedData _data{};

    wxTextCtrl* _filenameCtrl = nullptr;
    wxStaticText* _fileSizeLabel = nullptr;
    wxButton* _overrideButton = nullptr;

    void refresh() {
        auto path = _data.get_full_path();
        _filenameCtrl->SetValue(_data.tail_path.string());

        if (!fs::exists(path)) {
            _overrideButton->Disable();
            return;
        }

        _overrideButton->Enable();
        if (_data.override != nullptr) {
            _overrideButton->SetLabel("Reset");
        } else {
            _overrideButton->SetLabel("Override");
        }

        size_t file_size{FileHandler::GetFileSize(path)};
        // Convert file size to kb
        _fileSizeLabel->SetLabel(
            std::format("{}KB", std::ceil((double)file_size / 1024)));
    };

    void onSelectedPathChanged(const PathChangedData& data) override {
        _data = data;
        this->refresh();
    };

    void onOverrideUpdated(Override* override) override {
        this->_data.override = override;
        this->refresh();
    }

    void onOverrideButtonClicked(wxCommandEvent& /* unused */) {
        if (_data.override != nullptr) {
            ModManager::getInstance().removeOverride(_data.override);
            return;
        }

        const fs::path& from_path = _data.tail_path;

        fs::path extension = from_path.extension();

        if (extension == SUPPORTED_FILE_EXTENSIONS::SD9) {
            fs::path to_path =
                Utils::filePopup("Select replacement audio", "wav");

            if (fs::exists(to_path)) {
                try {
                    auto* override{new SD9Override(from_path)};
                    override->setReplacementFilepath(to_path);

                    ModManager::getInstance().addOverride(override);
                } catch (std::exception& e) {
                    Utils::Dialog::Error("Unable to create SD9 override for " +
                                         from_path.string() +
                                         ".\nError: " + e.what());
                }
            }
        }
    }
}

;
