#pragma once

#include "mod_manager/ModObserver.h"

class EditPanel : public wxPanel, public ModObserver {
   public:
    explicit EditPanel(wxWindow* parent);

   private:
    void onModChanged(const Mod& mod) override;
};
