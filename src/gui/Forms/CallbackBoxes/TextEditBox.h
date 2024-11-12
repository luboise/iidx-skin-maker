#pragma once

using TextCallback = std::function<void(wxString&)>;
using TextLabel = wxTextCtrl*;

class TextEditBox : public wxBoxSizer {
   public:
    TextEditBox(wxWindow* parent, std::string label, std::string& form_value);

   private:
    // void onTextboxChanged(wxCommandEvent& event);
    std::string& _value;
    // TextLabel _label = nullptr;
    // TextCallback _callback;
};
