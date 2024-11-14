#pragma once

#include <wx/sizer.h>
#include "mod_manager/ModObserver.h"
#include "mod_manager/Overrides/SD9Override.h"

class OverrideEditor : public wxFlexGridSizer, public ModObserver {
   public:
    explicit OverrideEditor(wxWindow* parent, Override* override = nullptr)
        : wxFlexGridSizer(0, 3, 5, 5) {
        this->AddGrowableCol(1);
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
