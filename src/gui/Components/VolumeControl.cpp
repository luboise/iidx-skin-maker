
#include "VolumeControl.h"
#include <wx/event.h>
#include <wx/gtk/slider.h>
#include <wx/sizer.h>
#include "audio/AudioHandler.h"

VolumeControl::VolumeControl(wxWindow* parent) : wxBoxSizer(wxHORIZONTAL) {
    this->Add(new wxStaticText(parent, wxID_ANY, "Volume"));

    auto* slider = new wxSlider{parent, wxID_ANY, 100, 0, 100};

    slider->Bind(wxEVT_SLIDER, [](wxCommandEvent& event) {
        AudioHandler::SetVolume(event.GetInt());
    });

    auto* stop_button = new wxButton(parent, wxID_ANY, "Stop");

    stop_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
                      [](wxCommandEvent&) { AudioHandler::Stop(); });

    this->Add(slider);
    this->Add(stop_button);
};
