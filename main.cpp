#include "audio/AudioHandler.h"
#include "gui/MainFrame.h"

class SkinMakerApp : public wxApp {
   public:
    bool OnInit() override;

   private:
    MainFrame* _mainFrame = nullptr;
};

wxIMPLEMENT_APP(SkinMakerApp);

bool SkinMakerApp::OnInit() {
    wxAssertIsEqual(sizeof(SD9Info), SD9_HEADER_SIZE);
    auto audioSuccess = AudioHandler::Init();

    if (!audioSuccess) {
        wxMessageBox("Unable to initialise sound. Exiting now.");
        exit(EXIT_FAILURE);
    }

    _mainFrame = new MainFrame();
    _mainFrame->Show(true);

    return true;
}
