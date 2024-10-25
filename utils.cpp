#include "utils.h"

#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include <wx/gtk/msgdlg.h>

#include <cstring>

fs::path Utils::directoryPopup(const wxString& text) {
    auto path = wxDirSelector(text, "");

    if (path.IsEmpty()) {
        return "";
    }

    return fs::path{path.ToStdString()};
}

fs::path Utils::filePopup(const wxString& text, const wxString& filetype) {
    auto path = wxFileSelector(text, "", "", filetype, "*." + filetype);

    if (path.empty()) {
        return "";
    }

    return fs::path{path.ToStdString()};
};

bool Utils::String::StartsWith(char* s1, char* s2) {
    return strncmp(s1, s2, strlen(s2)) != 0;
};

fs::path Utils::saveFilePopup(const wxString& extension) {
    wxString path_string = wxSaveFileSelector(
        "Select a name for your mod file.", extension, "my_mod");

    std::string standard_string = path_string.ToStdString();

    return fs::path{path_string.ToStdString()};
}

bool Utils::booleanPopup(const wxString& message, const wxString& true_option,
                         const wxString& false_option) {
    auto* popup = new wxMessageDialog(nullptr, message, "CAPTION",
                                      wxYES_NO | wxICON_QUESTION);

    return popup->ShowModal() == wxID_YES;
};
