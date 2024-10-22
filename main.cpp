#include "audio/AudioHandler.h"
// #include "files/FileHandler.h"
#include "gui/MainFrame.h"

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

	_mainFrame = new MainFrame();
	_mainFrame->Show(true);

	return true;
}
