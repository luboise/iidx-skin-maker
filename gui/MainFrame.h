// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/treectrl.h>
#include <wx/wxprec.h>

#include "../files/Directory.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MainFrame : public wxFrame {
   public:
	MainFrame();

   private:
	// Event Handlers
	void OnHello(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnOpenNewContentsFolder(wxCommandEvent& event);
	void OnClickContentsFile(wxTreeEvent& event);

	void BuildContentsTree();
	void ChangeContentsDirectory(const fs::path& newDir);
	void ResetContentsTree();

	fs::path contentsDirPath;
	wxTreeCtrl* contentsTree;
	Directory* rootDir;
};

enum { ID_Hello = 1, ID_NewContentsFolder = 2 };