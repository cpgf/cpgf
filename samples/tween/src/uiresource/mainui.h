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
#include <wx/statline.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>

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
		
		wxButton* buttonBenchmark;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onTestTypeSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void onButtonBenchmarkClicked( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		ToolPanelRes( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 696,38 ), long style = wxTAB_TRAVERSAL );
		~ToolPanelRes();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CommandPanelRes
///////////////////////////////////////////////////////////////////////////////
class CommandPanelRes : public wxPanel 
{
	private:
	
	protected:
		wxButton* buttonPlay;
		wxButton* buttonPause;
		wxButton* buttonResume;
		
		wxButton* buttonReset;
		wxStaticLine* m_staticline2;
		wxCheckBox* checkBoxUseFrames;
		wxCheckBox* checkBoxBackward;
		wxStaticText* m_staticText5;
		wxChoice* choiceDuration;
		wxStaticText* m_staticText1;
		wxChoice* choiceDelay;
		wxStaticText* m_staticText2;
		wxChoice* choiceTimeScale;
		wxStaticLine* m_staticline3;
		wxStaticText* m_staticText3;
		wxChoice* choiceRepeatCount;
		wxCheckBox* checkBoxYoyo;
		wxStaticText* m_staticText4;
		wxChoice* choiceRepeatDelay;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onButtonPlayClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void onButtonPauseClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void onButtonResumeClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void onButtonResetClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void onCheckBoxUseFramesClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void onCheckBoxBackwardClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChoiceDurationSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChoiceDelaySelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChoiceTimeScaleSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChoiceRepeatSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void onCheckBoxYoyoClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChoiceRepeatDelaySelected( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		CommandPanelRes( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 425,153 ), long style = wxTAB_TRAVERSAL );
		~CommandPanelRes();
	
};

#endif //__mainui__
