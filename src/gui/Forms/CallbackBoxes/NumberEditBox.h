#pragma once

#include <wx/spinctrl.h>
#include <wx/valnum.h>

using NumberCallback = std::function<void(wxString&)>;

template <typename T>
class NumberEditBox : public wxBoxSizer {
   public:
    NumberEditBox(wxWindow* parent, std::string label_text, const T& form_value,
                  int min_value = -1, int max_value = -1)
        : _value(form_value), wxBoxSizer(wxHORIZONTAL) {
        auto* label = new wxStaticText(parent, wxID_ANY, label_text);
        this->Add(label);

        this->AddStretchSpacer();

        auto* text_ctrl{
            new wxSpinCtrl(parent, wxID_ANY, std::to_string(form_value))};

        if (min_value > 0 && max_value > min_value) {
            text_ctrl->SetRange(min_value, max_value);
        }

        this->Add(text_ctrl);

        text_ctrl->Bind(wxEVT_TEXT, &NumberEditBox::onNumberboxChanged, this);
    }

   private:
    T& _value;

    void onNumberboxChanged(wxCommandEvent& event) {
        T val = event.GetInt();
        _value = val;
    }
};
