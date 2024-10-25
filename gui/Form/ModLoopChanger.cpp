#include "ModLoopChanger.h"

#include <wx/event.h>
#include <wx/font.h>
#include <wx/gdicmn.h>
#include <wx/gtk/checkbox.h>
#include <wx/layout.h>
#include <wx/sizer.h>

#include "audio/SD9File.h"
#include "gui/Form/NumberCallbackBox.h"
#include "gui/Form/TextCallbackBox.h"

enum {
    MOD_NAME = wxID_HIGHEST + 1,
};

ModLoopChanger::ModLoopChanger(wxWindow* parent, SD9Info* info)
    : wxBoxSizer(wxVERTICAL), _info(info) {
    auto* enabled_sizer = new wxBoxSizer(wxHORIZONTAL);
    enabled_sizer->Add(new wxStaticText(parent, wxID_ANY, "Loop Enabled"));
    enabled_sizer->AddStretchSpacer();

    _enabledCheckbox = new wxCheckBox(parent, wxID_ANY, "");
    _enabledCheckbox->SetValue(info->loop_enabled != 0);

    enabled_sizer->Add(_enabledCheckbox);

    auto* loop_start_sizer =
        new NumberCallbackBox(parent, "Loop Start", info->loop_start);
    auto* loop_end_sizer =
        new NumberCallbackBox(parent, "Loop End", info->loop_end);

    constexpr auto PADDING_SIZE = 10;

    auto flags = wxSizerFlags().Expand().Border(wxALL, 10).GetFlags();

    this->Add(enabled_sizer, 1, flags, PADDING_SIZE);
    this->Add(loop_start_sizer, 1, flags, PADDING_SIZE);
    this->Add(loop_end_sizer, 1, flags, PADDING_SIZE);
}
