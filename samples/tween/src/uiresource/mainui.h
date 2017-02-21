///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef CPGF_mainui__
#define CPGF_mainui__

#include <wx/string.h>
#include <wx/frame.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/statline.h>
#include <wx/checkbox.h>
#include <wx/slider.h>

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
		wxStaticText* m_staticText6;
		wxChoice* choiceTestType;
		wxButton* buttonAboutTestCase;
		
		wxButton* buttonBenchmark;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onTestTypeSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void onButtonAboutThisTestCaseClicked( wxCommandEvent& event ) { event.Skip(); }
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
		wxStaticLine* m_staticline31;
		wxButton* buttonPlay;
		wxButton* buttonPause;
		wxButton* buttonResume;
		
		wxButton* buttonReset;
		wxBoxSizer* sizerParameters;
		wxStaticLine* m_staticline2;
		wxCheckBox* checkBoxUseFrames;
		wxCheckBox* checkBoxBackward;
		wxStaticText* m_staticText5;
		wxChoice* choiceDuration;
		wxStaticText* m_staticText1;
		wxChoice* choiceDelay;
		wxStaticText* m_staticText2;
		wxChoice* choiceTimeScale;
		wxStaticLine* m_staticline4;
		wxStaticText* m_staticText3;
		wxChoice* choiceRepeatCount;
		wxCheckBox* checkBoxYoyo;
		wxStaticText* m_staticText4;
		wxChoice* choiceRepeatDelay;
		wxStaticLine* m_staticline3;
		wxStaticText* m_staticText8;
		wxSlider* currentProgressSlider;
		wxStaticText* m_staticText7;
		wxSlider* totalProgressSlider;
		wxStaticLine* m_staticline5;
		
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
		virtual void onCurrentProgressSliderScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void onCurrentProgressSliderThumbRelease( wxScrollEvent& event ) { event.Skip(); }
		virtual void onCurrentProgressSliderThumbTrack( wxScrollEvent& event ) { event.Skip(); }
		virtual void onTotalProgressSliderScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void onTotalProgressSliderThumbRelease( wxScrollEvent& event ) { event.Skip(); }
		virtual void onTotalProgressSliderThumbTrack( wxScrollEvent& event ) { event.Skip(); }
		
	
	public:
		
		CommandPanelRes( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 428,267 ), long style = wxTAB_TRAVERSAL );
		~CommandPanelRes();
	
};

#endif //__mainui__
