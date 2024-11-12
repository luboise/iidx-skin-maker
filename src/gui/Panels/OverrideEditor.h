#pragma once

#include "mod_manager/ModObserver.h"
#include "mod_manager/Overrides/SD9Override.h"

class OverrideEditor : public wxBoxSizer, public ModObserver {
   public:
    explicit OverrideEditor(wxWindow* parent, Override* override = nullptr)
        : wxBoxSizer(wxVERTICAL) {
        onOverrideUpdated(override);
    };

   private:
    Override* currentOverride_{nullptr};
    /*
void onSelectedPathChanged(const PathChangedData& data) override {
    _data = data;
    this->update();
};
    */

    void setOverride(Override* override);

    void onSelectedPathChanged(const PathChangedData& data) override;
    void onOverrideUpdated(Override* override) override;

    void edit(SD9Override& override);
};
