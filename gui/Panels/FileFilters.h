#pragma once

#include <wx/treectrl.h>
#include <wx/wx.h>

class FileFilters : public wxPanel {
   public:
    FileFilters(wxWindow* parent);

   private:
    void OnButtonClick(wxCommandEvent& event);
    void OnAddEntity(wxCommandEvent& event);

    wxTreeCtrl* contentsTree = nullptr;

    enum { ID_ADD_ENTITY };
};
