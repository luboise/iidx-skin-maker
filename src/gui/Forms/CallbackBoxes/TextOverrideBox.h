#pragma once

#include "gui/Forms/CallbackBox.h"

using TextCallback = std::function<void(wxString&)>;
using TextLabel = wxTextCtrl*;

using text_callback_t = std::string;

class TextOverrideBox : public CallbackBox<text_callback_t> {
   public:
    TextOverrideBox(wxWindow*, std::string label,
                    const text_callback_t& base_value,
                    std::optional<text_callback_t>& modifiable);

   private:
    void onValueChanged(const wxCommandEvent& event) override;

    // std::string _value*    declared by base
    // std::optional<std::string>* _newValue    declared by base
};
