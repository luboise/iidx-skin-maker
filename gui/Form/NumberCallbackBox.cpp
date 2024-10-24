#include "NumberCallbackBox.h"

#include <wx/sizer.h>
#include <wx/spinctrl.h>

NumberCallbackBox::NumberCallbackBox(wxWindow* parent, std::string label_text,
									 number_box_value_t& form_value)
	: _value(form_value), wxBoxSizer(wxHORIZONTAL) {
	auto* label = new wxStaticText(parent, wxID_ANY, label_text);
	this->Add(label);

	this->AddStretchSpacer();

	auto* text_ctrl =
		new wxSpinCtrl(parent, wxID_ANY, std::to_string(form_value));
	this->Add(text_ctrl);

	text_ctrl->Bind(wxEVT_TEXT, &NumberCallbackBox::onNumberboxChanged, this);
}

void NumberCallbackBox::onNumberboxChanged(wxCommandEvent& event) {
	number_box_value_t val = event.GetInt();
	_value = val;
}
