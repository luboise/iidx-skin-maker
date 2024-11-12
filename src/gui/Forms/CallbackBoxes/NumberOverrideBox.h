#pragma once

#include "gui/Components/Buttons.h"
#include "gui/Forms/CallbackBox.h"
#include "gui/Forms/Validators/OptionalIntegerValidator.h"

using NumberCallback = std::function<void(wxString&)>;
using NumberLabel = wxTextCtrl*;

template <Number T>
class NumberOverrideBox : public CallbackBox<T> {
   public:
    NumberOverrideBox(wxWindow* parent, const std::string& label_text,
                      const T& form_value, std::optional<T>& modifiable)
        : CallbackBox<T>(parent, form_value) {
        this->_baseValue = form_value;
        this->_newValue = &modifiable;

        auto* label{new wxStaticText(parent, wxID_ANY, label_text)};
        this->Add(label);

        this->AddStretchSpacer();

        auto* text_ctrl{
            new wxTextCtrl(parent, wxID_ANY, std::to_string(form_value))};
        text_ctrl->Bind(wxEVT_TEXT, &NumberOverrideBox::onValueChanged, this);
        this->Add(text_ctrl);

        /*
[this](wxCommandEvent& event) {
int val = event.GetInt();
*(this->_newValue) = static_cast<T>(val);
},
        */

        this->AddStretchSpacer();

        auto reset_lambda{[this]() { *(this->_newValue) = std::nullopt; }};

        this->Add(Buttons::Reset(reset_lambda, parent));
    }

   private:
    void onValueChanged(const wxCommandEvent& event) override {
        int val = event.GetInt();
        *(this->_newValue) = static_cast<T>(val);
    }

    // T _value*    declared by base
    // std::optional<T>* _newValue    declared by base
};
