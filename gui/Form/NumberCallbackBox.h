#include <wx/sizer.h>

using NumberCallback = std::function<void(wxString&)>;

using number_box_value_t = int32_t;

class NumberCallbackBox : public wxBoxSizer {
   public:
	NumberCallbackBox(wxWindow*, std::string label,
					  number_box_value_t& form_value);

   private:
	void onNumberboxChanged(wxCommandEvent& event);
	int32_t& _value;
};
