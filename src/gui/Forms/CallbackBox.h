#include <optional>

template <typename T>
class CallbackBox : public wxBoxSizer {
   public:
    explicit CallbackBox(wxWindow* parent, const T& base_value,
                         bool enabled = true)
        : wxBoxSizer(wxHORIZONTAL), baseValue_(base_value), enabled_(enabled) {}

    void disable() {
        this->enabled_ = false;
        this->onDisable();
    }

    void enable(bool value = true) {
        if (!value) {
            this->disable();
            return;
        }

        this->enabled_ = true;
        this->onEnable();
    }

   private:
    virtual void onValueChanged(const wxCommandEvent& event) = 0;

    virtual void onDisable() = 0;
    virtual void onEnable() = 0;
    bool enabled_{};

   protected:
    T baseValue_{};
    std::optional<T>* newValue_{nullptr};
};
