#pragma once

#include "mod_manager/ModObserver.h"

class EditPanel : public wxPanel, public ModObserver {
   public:
    explicit EditPanel(wxWindow* parent);

   private:
    fs::path _selectedPath = "";

    void rebuildMainSizer();

    // void onModChanged(const Mod& mod) override;
    void onSelectedPathChanged(const PathChangedData& data) override;
};
