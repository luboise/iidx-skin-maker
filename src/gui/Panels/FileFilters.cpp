#include "FileFilters.h"

FileFilters::FileFilters(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    // wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    auto* add_entity_button{new wxButton(this, ID_ADD_ENTITY, "+")};
    add_entity_button->Bind(wxEVT_BUTTON, &FileFilters::OnAddEntity, this);

    auto* top_sizer{new wxBoxSizer(wxHORIZONTAL)};

    auto* tree{new wxTreeCtrl()};
    top_sizer->Add(tree);

    /*
// Add a list control for objects in the scene
wxListCtrl* objectList = new wxListCtrl(this, wxID_ANY, wxPoint(10, 50),
                                                                            wxSize(360,
200), wxLC_REPORT); objectList->InsertColumn(0, "Objects", wxLIST_FORMAT_LEFT,
300);

// Add example objects to the list
objectList->InsertItem(0, "Object 1");
objectList->InsertItem(1, "Object 2");
objectList->InsertItem(2, "Object 3");
    */

    auto* main_sizer{new wxBoxSizer(wxVERTICAL)};
    main_sizer->Add(top_sizer);

    this->SetSizer(main_sizer);
}

void FileFilters::OnButtonClick(wxCommandEvent& event) {
    wxMessageBox("Button clicked!", "Info", wxOK | wxICON_INFORMATION);
}

void FileFilters::OnAddEntity(wxCommandEvent& event) {
    // TODO: Implement this
}
