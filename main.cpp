// wxWidgets "Hello World" Program

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/treectrl.h>
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "files/FileHandler.h"
#include "gui/MainFrame.h"

class SkinMakerApp : public wxApp {
   public:
	virtual bool OnInit();
};

wxIMPLEMENT_APP(SkinMakerApp);

bool SkinMakerApp::OnInit() {
	MainFrame* frame = new MainFrame();
	frame->Show(true);

	return true;
}
