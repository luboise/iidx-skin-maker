#include <optional>

template <typename T>
class CallbackBox : public wxBoxSizer {
   public:
    explicit CallbackBox(wxWindow* parent, const T& base_value)
        : wxBoxSizer(wxHORIZONTAL), _baseValue(base_value) {}

   private:
    virtual void onValueChanged(const wxCommandEvent& event) = 0;

   protected:
    T _baseValue{};
    std::optional<T>* _newValue{nullptr};
};
