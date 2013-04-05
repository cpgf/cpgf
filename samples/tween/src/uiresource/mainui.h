///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __mainui__
#define __mainui__

#include <wx/string.h>
#include <wx/frame.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameRes
///////////////////////////////////////////////////////////////////////////////
class MainFrameRes : public wxFrame 
{
	private:
	
	protected:
	
	public:
		
		MainFrameRes( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 900,580 ), long style = wxCAPTION|wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
		~MainFrameRes();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ToolPanelRes
///////////////////////////////////////////////////////////////////////////////
class ToolPanelRes : public wxPanel 
{
	private:
	
	protected:
		wxChoice* choiceTestType;
		wxButton* buttonReset;
		
		wxButton* buttonBenchmark;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onTestTypeSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void onButtonResetClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void onButtonBenchmarkClicked( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		ToolPanelRes( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,38 ), long style = wxTAB_TRAVERSAL );
		~ToolPanelRes();
	
};

#endif //__mainui__
