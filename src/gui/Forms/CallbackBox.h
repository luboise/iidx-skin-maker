#include <optional>

template <typename T>
class CallbackBox : public wxBoxSizer {
   public:
    explicit CallbackBox(wxWindow* parent, const T& base_value)
        : wxBoxSizer(wxHORIZONTAL), baseValue_(base_value) {}

   private:
    virtual void onValueChanged(const wxCommandEvent& event) = 0;

   protected:
    T baseValue_{};
    std::optional<T>* newValue_{nullptr};
};
