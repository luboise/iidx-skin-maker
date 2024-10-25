
#include "EditPanel.h"

#include <wx/gdicmn.h>
#include <wx/msgdlg.h>

#include "audio/SD9File.h"

#include "gui/Form/ModLoopChanger.h"
#include "gui/Form/NumberCallbackBox.h"
#include "gui/Form/TextCallbackBox.h"

EditPanel::EditPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    SD9Info* test = new SD9Info{};
    test->unique_index = 69;

    test->audio_size = 100000;

    test->loop_enabled = 0x1;
    test->loop_start = 1000;
    test->loop_end = 2000;

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

    this->SetSizer(main_sizer);
}

void EditPanel::onModChanged(const Mod& mod) {
    // TODO: Implement this
};
