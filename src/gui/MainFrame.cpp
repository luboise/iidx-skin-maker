#include "MainFrame.h"

#include <portaudio.h>
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <memory>

#include "gui/MainMenuBar.h"
#include "gui/Panels/FileDetailsPanel.h"
#include "gui/Panels/FilePanel.h"
#include "gui/Panels/OverrideEditor.h"

MainFrame::MainFrame(wxWindow* parent)
    : wxFrame(nullptr, wxID_ANY, "Hello World") {
    // auto* sizer = new wxBoxSizer(wxHORIZONTAL);
    auto* splitter = new wxSplitterWindow(this, wxID_ANY);

    // LEFT half
    auto* new_file_panel{new FilePanel(splitter)};
    _fileSection = new_file_panel;

    // RIGHT half

    wxPanel* editing_panel{new wxPanel(splitter)};
    _editingPanel = editing_panel;

    wxBoxSizer* editing_sizer{new wxBoxSizer(wxVERTICAL)};

    _editingPanel->SetSizer(editing_sizer);

    _fileDetailsSizer = std::make_unique<FileDetailsPanel>(editing_panel);
    editing_sizer->Add(_fileDetailsSizer.get(), 0, wxEXPAND | wxALL, 5);

    auto* override_editor{new OverrideEditor(this)};
    editing_sizer->Add(override_editor, 0, wxEXPAND | wxALL, 5);

    splitter->SplitVertically(_fileSection, editing_panel);
    splitter->SetMinimumPaneSize(200);
    splitter->SetSashPosition(400);

    // sizer->Add(_fileSection);

    // sizer->AddStretchSpacer(1);

    // this->SetSizer(sizer);

    auto* main_sizer{new wxBoxSizer(wxHORIZONTAL)};
    main_sizer->Add(splitter, 1, wxEXPAND | wxALL, 0);
    this->SetSizer(main_sizer);

    this->CreateStatusBar();

    auto* new_main_menu{new MainMenuBar(this)};
    this->SetMenuBar(new_main_menu);

    // Finalise layout and then change the contents dir
    // _fileSection->ChangeContentsDirectory("/home/lucas/desktop/2dx_fake");
}

void MainFrame::OnExit(wxCommandEvent& event) {
    Pa_Terminate();
    Close(true);
}

/*
MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Hello World") {
        this->contentsTree = nullptr;

        wxMenu* menuFile = new wxMenu;
        menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                                         "Help string shown in status bar for
this menu item"); menuFile->AppendSeparator();
        menuFile->Append(ID_NewContentsFolder, "&Open Game Folder",
                                         "Do the thing?????");
        menuFile->AppendSeparator();
        menuFile->Append(wxID_EXIT);

        wxMenu* menuHelp = new wxMenu;
        menuHelp->Append(wxID_ABOUT);

        // Add file menu and help menu to menu bar
        wxMenuBar* mainBar = new wxMenuBar;
        mainBar->Append(menuFile, "&File");
        mainBar->Append(menuHelp, "&Help");
        SetMenuBar(mainBar);

        //
ChangeContentsDirectory("/home/luboise/iidx-skin-maker/test/contents/data");
        // ChangeContentsDirectory("C:/LDJ-003-2022101900/contents/data");

        CreateStatusBar();
        SetStatusText("Welcome to wxWidgets!");

        Bind(wxEVT_MENU, &MainFrame::OnHello, this, ID_Hello);
        Bind(wxEVT_MENU, &MainFrame::OnOpenNewContentsFolder, this,
                 ID_NewContentsFolder);
        Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
        Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
        Bind(wxEVT_TREE_ITEM_ACTIVATED, &MainFrame::OnClickContentsFile, this,
                 wxID_ANY);
}
*/

/*
void MainFrame::onOverrideUpdated(Override* override) {
    if (_overrideEditor == nullptr) {
        this->updateOverrideEditor(override);
    }
};
*/
