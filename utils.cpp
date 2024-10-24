#include "utils.h"

#include <wx/dirdlg.h>
#include <wx/filedlg.h>

fs::path Utils::directoryPopup(wxString text) {
	auto path = wxDirSelector(text, "");

	if (path.IsEmpty()) return "";

	return fs::path(path.ToStdString());
}

fs::path Utils::filePopup(wxString text, wxString filetype) {
	auto path = wxFileSelector(text, "", "", filetype, "*." + filetype);

	if (path.empty()) {
		return "";
	}

	return fs::path(path.ToStdString());
};
