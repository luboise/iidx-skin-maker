#include "audio/AudioHandler.h"
#include "gui/MainFrame.h"
#include "mod_manager/ModManager.h"

class SkinMakerApp : public wxApp {
   public:
	virtual bool OnInit();

   private:
	MainFrame* _mainFrame = nullptr;
};

wxIMPLEMENT_APP(SkinMakerApp);

bool SkinMakerApp::OnInit() {
	auto audioSuccess = AudioHandler::Init();

	if (!audioSuccess) {
		wxMessageBox("Unable to initialise sound. Exiting now.");
		exit(EXIT_FAILURE);
	}

	ModManager::getInstance().newMod();

	_mainFrame = new MainFrame();
	_mainFrame->Show(true);

	return true;
}
