#include "EditPanel.h"

#include "files/FileHandler.h"

#include "mod_manager/ModManager.h"
#include "mod_manager/ModObserver.h"

#include "OverrideEditor.h"

#include "gui/Forms/ModLoopChanger.h"

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
                            auto* override = new SD9Override(from_path);
                            override->setReplacementFilepath(to_path);

                            ModManager::getInstance().addOverride(override);
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
        auto full_path = _data.get_full_path();
        _filenameCtrl->SetValue(full_path.string());

        if (!fs::exists(full_path)) {
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

    void onOverrideUpdated(Override* override) override {
        this->_data.override = override;
        this->refresh();
    }
};

EditPanel::EditPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    _fileDetailsSizer = new FileDetailsSizer(this);
    main_sizer->Add(_fileDetailsSizer);

    _filePreviewSizer = new OverrideEditor(this);
    main_sizer->Add(_filePreviewSizer);

    this->SetSizer(main_sizer);
    // this->Layout();

    // this->update();
}

void EditPanel::onSelectedPathChanged(const PathChangedData& data) {
    _selectedPath = data.root_path / data.tail_path;
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
