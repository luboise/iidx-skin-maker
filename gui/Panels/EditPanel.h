#pragma once

#include "mod_manager/ModObserver.h"

class FileDetailsSizer;
class FilePreviewSizer;

class EditPanel : public wxPanel, public ModObserver {
   public:
    explicit EditPanel(wxWindow* parent);

   private:
    fs::path _selectedPath = "";
    FileDetailsSizer* _fileDetailsSizer = nullptr;
    FilePreviewSizer* _filePreviewSizer = nullptr;

    // void update();

    // void onModChanged(const Mod& mod) override;
    void onSelectedPathChanged(const PathChangedData& data) override;
    // void onOverrideUpdated(const Override* override) override;
};
