#include "mod_manager/ModObserver.h"

class OverrideEditor : public wxBoxSizer, public ModObserver {
   public:
    explicit OverrideEditor(wxWindow* parent) : wxBoxSizer(wxVERTICAL) {
        this->update(parent);
    };

   private:
    wxTextCtrl* _previewCtrl = nullptr;
    PathChangedData _data{};

    void onSelectedPathChanged(const PathChangedData& data) override {
        _data = data;
        this->update();
    };

    void update() { this->update(this->GetContainingWindow()); }
    void update(wxWindow* parent);
};
