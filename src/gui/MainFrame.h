#include <wx/treectrl.h>

class FilePanel;
class EditPanel;

class MainFrame : public wxFrame {
   public:
    explicit MainFrame(wxWindow* parent = nullptr);

    void onPathClicked(wxCommandEvent& event);

   private:
    // Event Handlers
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    FilePanel* _fileSection = nullptr;
    EditPanel* _editSection = nullptr;
};

enum { ID_Hello = 1, ID_NewContentsFolder = 2 };
