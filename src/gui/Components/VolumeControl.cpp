
#include "VolumeControl.h"
#include <wx/event.h>
#include <wx/sizer.h>
#include "audio/AudioHandler.h"

using volume_control_t = uint8_t;

constexpr volume_control_t MIN_VOLUME = 0;
constexpr volume_control_t MAX_VOLUME = 100;

VolumeControl::VolumeControl(wxWindow* parent) : wxBoxSizer(wxHORIZONTAL) {
    auto* volume_label{new wxStaticText(parent, wxID_ANY, "Volume")};
    this->Add(volume_label);

    volume_t starting_volume_ratio = AudioHandler::GetVolume();
    auto starting_volume =
        static_cast<volume_control_t>(starting_volume_ratio * MAX_VOLUME);
    auto* slider{new wxSlider{parent, wxID_ANY, starting_volume, MIN_VOLUME,
                              MAX_VOLUME}};

    slider->Bind(wxEVT_SLIDER, [](wxCommandEvent& event) {
        AudioHandler::SetVolume(((float)event.GetInt()) / MAX_VOLUME);
    });

    auto* stop_button = new wxButton(parent, wxID_ANY, "Stop");

    stop_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
                      [](wxCommandEvent&) { AudioHandler::Stop(); });

    this->Add(slider);
    this->Add(stop_button);
};
