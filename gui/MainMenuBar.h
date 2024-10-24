#include <wx/event.h>
#include <wx/wx.h>

class MainMenuBar : public wxMenuBar {
   public:
	MainMenuBar(wxWindow* parent);

   private:
	void onOpenMod(wxMenuEvent&);
	void onMenuOptionClicked(wxCommandEvent& event);
};
