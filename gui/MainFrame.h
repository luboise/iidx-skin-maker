#include <wx/treectrl.h>

class FilePanel;

class MainFrame : public wxFrame {
   public:
	MainFrame(wxWindow* parent = nullptr);

   private:
	// Event Handlers
	void OnHello(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);

	FilePanel* _fileSection = nullptr;
};

enum { ID_Hello = 1, ID_NewContentsFolder = 2 };
