#include <wx/gdicmn.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>

using NumberCallback = std::function<void(wxString&)>;

template <typename T>
class NumberCallbackBox : public wxBoxSizer {
   public:
    NumberCallbackBox(wxWindow* parent, std::string label_text, T& form_value,
                      int min_value = -1, int max_value = -1)
        : _value(form_value), wxBoxSizer(wxHORIZONTAL) {
        auto* label = new wxStaticText(parent, wxID_ANY, label_text);
        this->Add(label);

        this->AddStretchSpacer();

        auto* text_ctrl =
            new wxSpinCtrl(parent, wxID_ANY, std::to_string(form_value));

        if (min_value > 0 && max_value > min_value) {
            text_ctrl->SetRange(min_value, max_value);
        }

        this->Add(text_ctrl);

        text_ctrl->Bind(wxEVT_TEXT, &NumberCallbackBox::onNumberboxChanged,
                        this);
    }

   private:
    T& _value;

    void onNumberboxChanged(wxCommandEvent& event) {
        T val = event.GetInt();
        _value = val;
    }
};
