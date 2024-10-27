#include "EditPanel.h"
#include <wx/msgdlg.h>
#include <sstream>

#include "audio/SD9File.h"
#include "files/FileHandler.h"

#include "gui/Form/ModLoopChanger.h"
#include "gui/Form/NumberCallbackBox.h"
#include "gui/Form/TextCallbackBox.h"
#include "mod_manager/ModObserver.h"
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
