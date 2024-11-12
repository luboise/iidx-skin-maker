#include <wx/valnum.h>
#include <optional>

// #include "gui/Forms/Validators/OptionalIntegerValidator.h"

template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;

template <typename T>
class OptionalValidator : public wxIntegerValidator<T> {
    using Super = wxIntegerValidator<T>;

   public:
    OptionalValidator(std::optional<T>& optional, T min, T max)
        : Super(), _overrideValue(optional) {};

    bool Validate(wxWindow* parent) const override {
        if (!Super::Validate(parent)) {
            return false;
        }
    };

   private:
    std::optional<T>* _overrideValue{nullptr};
};
