#include "audio/AudioHandler.h"
#include "gui/MainFrame.h"
#include "mod_manager/ModManager.h"

class SkinMakerApp : public wxApp {
   public:
    bool OnInit() override;

   private:
    std::unique_ptr<MainFrame> _mainFrame{nullptr};
};

wxIMPLEMENT_APP(SkinMakerApp);

bool SkinMakerApp::OnInit() {
    wxAssertIsEqual(sizeof(SD9Info), SD9_HEADER_SIZE);
    auto audioSuccess = AudioHandler::Init();

    if (!audioSuccess) {
        wxMessageBox("Unable to initialise sound. Exiting now.");
        exit(EXIT_FAILURE);
    }

    _mainFrame = std::make_unique<MainFrame>();
    _mainFrame->Show(true);

    if (this->argc == 2) {
        fs::path path = this->argv[1].ToStdString();
        if (fs::exists(path)) {
            std::cout << "Loading mod from command line argument path " << path
                      << std::endl;
            ModManager::getInstance().loadMod(path);
        } else {
            std::cerr << "Unable to load mod from command line argument "
                      << path << std::endl;
        }
    }

    return true;
}
