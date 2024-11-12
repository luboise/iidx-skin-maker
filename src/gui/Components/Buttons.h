#pragma once

namespace Buttons {
    using button_callback_t = std::function<void()>;

    inline wxButton* Reset(const button_callback_t& callback,
                           wxWindow* parent = nullptr,
                           const wxString& label = "X") {
        wxButton* button{new wxButton(parent, wxID_RESET, label)};

        auto reset_lambda{[callback](wxCommandEvent&) { callback(); }};
        button->Bind(wxEVT_BUTTON, reset_lambda);

        return button;
    };
};  // namespace Buttons
