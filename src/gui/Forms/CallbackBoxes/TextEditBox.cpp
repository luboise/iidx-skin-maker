#include "TextEditBox.h"

#include <wx/sizer.h>

TextEditBox::TextEditBox(wxWindow* parent, std::string label_text,
                         std::string& form_value)
    : wxBoxSizer(wxHORIZONTAL), _value(form_value) {
    auto* label = new wxStaticText(parent, wxID_ANY, label_text);
    this->Add(label);

    this->AddStretchSpacer();

    auto* text_ctrl = new wxTextCtrl(parent, wxID_ANY, form_value);
    this->Add(text_ctrl);

    text_ctrl->Bind(wxEVT_TEXT, [this](const wxCommandEvent& event) {
        wxString val = event.GetString();
        _value = val;
    });
}

/*
void TextEditBox::onTextboxChanged(wxCommandEvent& event) {
    wxString val = event.GetString();
    _value = val;
}
*/
