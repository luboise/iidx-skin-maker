#include "TextCallbackBox.h"

#include <wx/sizer.h>

TextCallbackBox::TextCallbackBox(wxWindow* parent, std::string label_text,
								 std::string& form_value)
	: _value(form_value), wxBoxSizer(wxHORIZONTAL) {
	auto* label = new wxStaticText(parent, wxID_ANY, label_text);
	this->Add(label);

	this->AddStretchSpacer();

	auto* text_ctrl = new wxTextCtrl(parent, wxID_ANY, form_value);
	this->Add(text_ctrl);

	text_ctrl->Bind(wxEVT_TEXT, &TextCallbackBox::onTextboxChanged, this);
}

void TextCallbackBox::onTextboxChanged(wxCommandEvent& event) {
	wxString val = event.GetString();
	_value = val;
}
