#include "gui/Panels/FileDetailsPanel.h"
#include "gui/Panels/OverrideEditor.h"

class FilePanel;
class EditPanel;

class MainFrame : public wxFrame {
   public:
    explicit MainFrame(wxWindow* parent = nullptr);

    void onPathClicked(wxCommandEvent& event);

   private:
    // Event Handlers
    void OnExit(wxCommandEvent& event);

    FilePanel* _fileSection{nullptr};
    wxPanel* _editingPanel{nullptr};
    std::unique_ptr<FileDetailsPanel> _fileDetailsSizer{nullptr};
    OverrideEditor* _overrideEditor{nullptr};
};

enum { ID_Hello = 1, ID_NewContentsFolder = 2 };
