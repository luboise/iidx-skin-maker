#include "utils.h"

#include <wx/filedlg.h>

wxString Utils::directoryPopup(wxWindow* parent, wxString text) {
	wxDirDialog dlg(parent, text, "", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (dlg.ShowModal() != wxID_OK) {
		return "";
	}

	return dlg.GetPath();
}

wxString Utils::filePopup(wxString text, wxString filetype) {
	auto path = wxFileSelector(text, "", "", filetype, "*." + filetype);

	if (path.empty()) {
		return "";
	}

	return path;
};
