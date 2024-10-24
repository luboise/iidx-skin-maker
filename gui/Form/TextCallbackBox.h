#include <wx/sizer.h>

using TextCallback = std::function<void(wxString&)>;
using TextLabel = wxTextCtrl*;

class TextCallbackBox : public wxBoxSizer {
   public:
	TextCallbackBox(wxWindow*, std::string label, std::string& form_value);

   private:
	void onTextboxChanged(wxCommandEvent& event);
	std::string& _value;
	// TextLabel _label = nullptr;
	// TextCallback _callback;
};
