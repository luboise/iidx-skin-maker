#include "TextOverrideBox.h"
#include "gui/Components/Buttons.h"

#include <wx/sizer.h>
#include <optional>

TextOverrideBox::TextOverrideBox(wxWindow* parent, std::string label_text,
                                 const std::string& form_value,
                                 std::optional<std::string>& modifiable)
    : CallbackBox(parent, form_value) {
    _baseValue = form_value;
    _newValue = &modifiable;

    auto* label = new wxStaticText(parent, wxID_ANY, label_text);
    this->Add(label);

    this->AddStretchSpacer();

    auto* text_ctrl = new wxTextCtrl(parent, wxID_ANY, form_value);
    this->Add(text_ctrl);

    text_ctrl->Bind(wxEVT_TEXT, &TextOverrideBox::onValueChanged, this);

    this->AddStretchSpacer();

    auto reset_lambda{[this]() { *(this->_newValue) = std::nullopt; }};
    this->Add(Buttons::Reset(reset_lambda, parent));
}

void TextOverrideBox::onValueChanged(const wxCommandEvent& event) {
    wxString val = event.GetString();
    *_newValue = val.ToStdString();
}
