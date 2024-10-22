#include <wx/treectrl.h>
#include <wx/wx.h>

#include "files/Directory.h"

class FileSection : public wxPanel {
   public:
	FileSection(wxWindow* parent);
	void ChangeContentsDirectory(const fs::path& newDir);

   private:
	void OnButtonClick(wxCommandEvent& event);

	void OnClickContentsFile(wxTreeEvent& event);

	void OnOpenNewContentsFolder(wxCommandEvent& event);
	void ResetContentsTree();

	void BuildContentsTree();
	void BuildContentsTreeRecursive(const wxTreeItemId& currentNode,
									Directory* currentDir);

	wxTreeCtrl* _contentsTree = nullptr;

	enum { ID_ADD_ENTITY };

	fs::path _contentsDirPath;
	Directory* _rootDir;
};
