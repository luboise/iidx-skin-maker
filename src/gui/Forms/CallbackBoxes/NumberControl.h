#pragma once

#include <wx/msgdlg.h>
#include "gui/Forms/Validators/OptionalIntegerValidator.h"

#include <wx/textctrl.h>
#include <wx/valnum.h>

#include <limits>

using NumberCallback = std::function<void(wxString&)>;
using NumberLabel = wxTextCtrl*;

template <Number T>
class NumberControl : public wxTextCtrl {
   public:
    NumberControl(wxWindow* parent, const T& form_value,
                  std::optional<T>& modifiable)
        : wxTextCtrl(parent, wxID_ANY),
          baseValue_(form_value),
          newValue_(&modifiable) {
        this->SetValue(std::to_string(
            modifiable.has_value() ? modifiable.value() : form_value));
        wxIntegerValidator<T> validator{};
        validator.SetRange(0, std::numeric_limits<T>::max());

        this->SetValidator(validator);

        this->Bind(wxEVT_TEXT,
                   [this](wxCommandEvent& event) { onValueChanged(event); });
    }

    void reset() { this->SetValue(std::to_string(this->baseValue_)); };
    bool isOverriding() const { return newValue_->has_value(); };

   private:
    // wxButton* resetButton_{nullptr};

    T baseValue_{};
    std::optional<T>* newValue_{nullptr};

    void requestLayout() const { this->GetContainingWindow()->Layout(); }

    void onValueChanged(const wxCommandEvent& event) {
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

        // TODO: Fix this to be working again
        // this->resetButton_->Show(value_is_overridden);
    }
};
