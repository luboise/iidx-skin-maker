#pragma once

#include <wx/msgdlg.h>
#include "gui/Components/Buttons.h"
#include "gui/Forms/CallbackBox.h"
#include "gui/Forms/Validators/OptionalIntegerValidator.h"

#include <wx/textctrl.h>
#include <wx/valnum.h>

#include <limits>

using NumberCallback = std::function<void(wxString&)>;
using NumberLabel = wxTextCtrl*;

template <Number T>
class NumberOverrideBox : public CallbackBox<T> {
   public:
    NumberOverrideBox(wxWindow* parent, const std::string& label_text,
                      const T& form_value, std::optional<T>& modifiable)
        : CallbackBox<T>(parent, form_value) {
        T initial_value{modifiable.has_value() ? modifiable.value()
                                               : form_value};

        auto* text_ctrl{
            new wxTextCtrl(parent, wxID_ANY, std::to_string(initial_value))};

        this->textCtrl_ = text_ctrl;

        this->baseValue_ = form_value;
        this->newValue_ = &modifiable;

        auto* label{new wxStaticText(parent, wxID_ANY, label_text)};
        this->Add(label);

        this->AddStretchSpacer();

        wxIntegerValidator<T> validator{};
        validator.SetRange(0, std::numeric_limits<T>::max());

        this->textCtrl_->SetValidator(validator);

        this->textCtrl_->Bind(wxEVT_TEXT, &NumberOverrideBox::onValueChanged,
                              this);
        this->Add(this->textCtrl_);

        /*
[this](wxCommandEvent& event) {
int val = event.GetInt();
*(this->newValue_) = static_cast<T>(val);
},
        */

        this->AddStretchSpacer();

        auto reset_lambda{[this]() {
            this->textCtrl_->SetValue(std::to_string(this->baseValue_));
        }};
        resetButton_ = Buttons::Reset(reset_lambda, parent);
        this->Add(resetButton_);

        resetButton_->Show(this->newValue_->has_value());
    }

   private:
    wxButton* resetButton_{nullptr};
    wxTextCtrl* textCtrl_{nullptr};

    void requestLayout() const { this->GetContainingWindow()->Layout(); }

    void onValueChanged(const wxCommandEvent& event) override {
        int new_val{};
        auto text{event.GetString()};

        if (text.IsEmpty()) {
            new_val = 0;
        } else if (!event.GetString().ToInt(&new_val)) {
            wxMessageBox("Unable to parse int from text entry.");
            return;
        }

        if (new_val == this->baseValue_) {
            this->newValue_->reset();
        } else {
            *(this->newValue_) = static_cast<T>(new_val);
        }

        bool value_is_overridden{this->newValue_->has_value()};
        this->resetButton_->Show(value_is_overridden);
    }

    void onDisable() override { this->textCtrl_->Disable(); }
    void onEnable() override { this->textCtrl_->Enable(); }
};
