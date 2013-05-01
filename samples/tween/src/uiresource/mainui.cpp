///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "mainui.h"

///////////////////////////////////////////////////////////////////////////

MainFrameRes::MainFrameRes( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 900,500 ), wxDefaultSize );
	
	
	this->Centre( wxBOTH );
}

MainFrameRes::~MainFrameRes()
{
}

ToolPanelRes::ToolPanelRes( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Test case:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer1->Add( m_staticText6, 0, wxALL, 5 );
	
	wxString choiceTestTypeChoices[] = { wxT("Basic"), wxT("Animation"), wxT("Follow"), wxT("Timeline"), wxT("Particles (CPU cache friendly)"), wxT("Chart"), wxT("All Charts") };
	int choiceTestTypeNChoices = sizeof( choiceTestTypeChoices ) / sizeof( wxString );
	choiceTestType = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choiceTestTypeNChoices, choiceTestTypeChoices, 0 );
	choiceTestType->SetSelection( 0 );
	choiceTestType->SetMinSize( wxSize( 150,-1 ) );
	
	bSizer1->Add( choiceTestType, 0, wxALL, 5 );
	
	buttonAboutTestCase = new wxButton( this, wxID_ANY, wxT("About this test case"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( buttonAboutTestCase, 0, wxALL, 5 );
	
	
	bSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	buttonBenchmark = new wxButton( this, wxID_ANY, wxT("&Benchmark"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( buttonBenchmark, 0, wxALL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	choiceTestType->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ToolPanelRes::onTestTypeSelected ), NULL, this );
	buttonAboutTestCase->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ToolPanelRes::onButtonAboutThisTestCaseClicked ), NULL, this );
	buttonBenchmark->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ToolPanelRes::onButtonBenchmarkClicked ), NULL, this );
}

ToolPanelRes::~ToolPanelRes()
{
	// Disconnect Events
	choiceTestType->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ToolPanelRes::onTestTypeSelected ), NULL, this );
	buttonAboutTestCase->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ToolPanelRes::onButtonAboutThisTestCaseClicked ), NULL, this );
	buttonBenchmark->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ToolPanelRes::onButtonBenchmarkClicked ), NULL, this );
	
}

CommandPanelRes::CommandPanelRes( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_staticline31 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer2->Add( m_staticline31, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	buttonPlay = new wxButton( this, wxID_ANY, wxT("Play"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( buttonPlay, 0, wxALL, 5 );
	
	buttonPause = new wxButton( this, wxID_ANY, wxT("Pause"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( buttonPause, 0, wxALL, 5 );
	
	buttonResume = new wxButton( this, wxID_ANY, wxT("Resume"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( buttonResume, 0, wxALL, 5 );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	buttonReset = new wxButton( this, wxID_ANY, wxT("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( buttonReset, 0, wxALL, 5 );
	
	bSizer2->Add( bSizer3, 0, wxEXPAND, 5 );
	
	sizerParameters = new wxBoxSizer( wxVERTICAL );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerParameters->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	checkBoxUseFrames = new wxCheckBox( this, wxID_ANY, wxT("useFrames"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( checkBoxUseFrames, 0, wxALL, 5 );
	
	checkBoxBackward = new wxCheckBox( this, wxID_ANY, wxT("backward"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( checkBoxBackward, 0, wxALL, 5 );
	
	sizerParameters->Add( bSizer5, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("duration:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer6->Add( m_staticText5, 0, wxALL, 5 );
	
	wxString choiceDurationChoices[] = { wxT("0"), wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6") };
	int choiceDurationNChoices = sizeof( choiceDurationChoices ) / sizeof( wxString );
	choiceDuration = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choiceDurationNChoices, choiceDurationChoices, 0 );
	choiceDuration->SetSelection( 2 );
	bSizer6->Add( choiceDuration, 0, wxALL, 5 );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("delay:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer6->Add( m_staticText1, 0, wxALL, 5 );
	
	wxString choiceDelayChoices[] = { wxT("0"), wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6") };
	int choiceDelayNChoices = sizeof( choiceDelayChoices ) / sizeof( wxString );
	choiceDelay = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choiceDelayNChoices, choiceDelayChoices, 0 );
	choiceDelay->SetSelection( 0 );
	bSizer6->Add( choiceDelay, 0, wxALL, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("timeScale:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer6->Add( m_staticText2, 0, wxALL, 5 );
	
	wxString choiceTimeScaleChoices[] = { wxT("0.2"), wxT("0.5"), wxT("1.0"), wxT("1.5"), wxT("2.0"), wxT("3.0"), wxT("5.0") };
	int choiceTimeScaleNChoices = sizeof( choiceTimeScaleChoices ) / sizeof( wxString );
	choiceTimeScale = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choiceTimeScaleNChoices, choiceTimeScaleChoices, 0 );
	choiceTimeScale->SetSelection( 2 );
	bSizer6->Add( choiceTimeScale, 0, wxALL, 5 );
	
	sizerParameters->Add( bSizer6, 0, wxEXPAND, 5 );
	
	m_staticline4 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerParameters->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("repeat:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer4->Add( m_staticText3, 0, wxALL, 5 );
	
	wxString choiceRepeatCountChoices[] = { wxT("Infinitely (-1)"), wxT("No repeat (0)"), wxT("One time (1)"), wxT("Two times (2)"), wxT("Three times (3)") };
	int choiceRepeatCountNChoices = sizeof( choiceRepeatCountChoices ) / sizeof( wxString );
	choiceRepeatCount = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choiceRepeatCountNChoices, choiceRepeatCountChoices, 0 );
	choiceRepeatCount->SetSelection( 1 );
	bSizer4->Add( choiceRepeatCount, 0, wxALL, 5 );
	
	checkBoxYoyo = new wxCheckBox( this, wxID_ANY, wxT("yoyo"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( checkBoxYoyo, 0, wxALL, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("repeatDelay:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer4->Add( m_staticText4, 0, wxALL, 5 );
	
	wxString choiceRepeatDelayChoices[] = { wxT("0"), wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6") };
	int choiceRepeatDelayNChoices = sizeof( choiceRepeatDelayChoices ) / sizeof( wxString );
	choiceRepeatDelay = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choiceRepeatDelayNChoices, choiceRepeatDelayChoices, 0 );
	choiceRepeatDelay->SetSelection( 0 );
	bSizer4->Add( choiceRepeatDelay, 0, wxALL, 5 );
	
	sizerParameters->Add( bSizer4, 0, wxEXPAND, 5 );
	
	m_staticline3 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerParameters->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Current:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer9->Add( m_staticText8, 0, wxALL, 5 );
	
	currentProgressSlider = new wxSlider( this, wxID_ANY, 0, 0, 1000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer9->Add( currentProgressSlider, 1, wxALL, 5 );
	
	sizerParameters->Add( bSizer9, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Total:  "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer8->Add( m_staticText7, 0, wxALL, 5 );
	
	totalProgressSlider = new wxSlider( this, wxID_ANY, 0, 0, 1000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer8->Add( totalProgressSlider, 1, wxALL|wxEXPAND, 5 );
	
	sizerParameters->Add( bSizer8, 1, wxEXPAND, 5 );
	
	m_staticline5 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	sizerParameters->Add( m_staticline5, 0, wxEXPAND | wxALL, 5 );
	
	bSizer2->Add( sizerParameters, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
	
	// Connect Events
	buttonPlay->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CommandPanelRes::onButtonPlayClicked ), NULL, this );
	buttonPause->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CommandPanelRes::onButtonPauseClicked ), NULL, this );
	buttonResume->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CommandPanelRes::onButtonResumeClicked ), NULL, this );
	buttonReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CommandPanelRes::onButtonResetClicked ), NULL, this );
	checkBoxUseFrames->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CommandPanelRes::onCheckBoxUseFramesClicked ), NULL, this );
	checkBoxBackward->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CommandPanelRes::onCheckBoxBackwardClicked ), NULL, this );
	choiceDuration->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CommandPanelRes::onChoiceDurationSelected ), NULL, this );
	choiceDelay->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CommandPanelRes::onChoiceDelaySelected ), NULL, this );
	choiceTimeScale->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CommandPanelRes::onChoiceTimeScaleSelected ), NULL, this );
	choiceRepeatCount->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CommandPanelRes::onChoiceRepeatSelected ), NULL, this );
	checkBoxYoyo->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CommandPanelRes::onCheckBoxYoyoClicked ), NULL, this );
	choiceRepeatDelay->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CommandPanelRes::onChoiceRepeatDelaySelected ), NULL, this );
	currentProgressSlider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderScroll ), NULL, this );
	currentProgressSlider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderScroll ), NULL, this );
	currentProgressSlider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderScroll ), NULL, this );
	currentProgressSlider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderScroll ), NULL, this );
	currentProgressSlider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderScroll ), NULL, this );
	currentProgressSlider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderScroll ), NULL, this );
	currentProgressSlider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderScroll ), NULL, this );
	currentProgressSlider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderScroll ), NULL, this );
	currentProgressSlider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderScroll ), NULL, this );
	currentProgressSlider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderThumbRelease ), NULL, this );
	currentProgressSlider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderThumbTrack ), NULL, this );
	totalProgressSlider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderScroll ), NULL, this );
	totalProgressSlider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderScroll ), NULL, this );
	totalProgressSlider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderScroll ), NULL, this );
	totalProgressSlider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderScroll ), NULL, this );
	totalProgressSlider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderScroll ), NULL, this );
	totalProgressSlider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderScroll ), NULL, this );
	totalProgressSlider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderScroll ), NULL, this );
	totalProgressSlider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderScroll ), NULL, this );
	totalProgressSlider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderScroll ), NULL, this );
	totalProgressSlider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderThumbRelease ), NULL, this );
	totalProgressSlider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderThumbTrack ), NULL, this );
}

CommandPanelRes::~CommandPanelRes()
{
	// Disconnect Events
	buttonPlay->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CommandPanelRes::onButtonPlayClicked ), NULL, this );
	buttonPause->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CommandPanelRes::onButtonPauseClicked ), NULL, this );
	buttonResume->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CommandPanelRes::onButtonResumeClicked ), NULL, this );
	buttonReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CommandPanelRes::onButtonResetClicked ), NULL, this );
	checkBoxUseFrames->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CommandPanelRes::onCheckBoxUseFramesClicked ), NULL, this );
	checkBoxBackward->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CommandPanelRes::onCheckBoxBackwardClicked ), NULL, this );
	choiceDuration->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CommandPanelRes::onChoiceDurationSelected ), NULL, this );
	choiceDelay->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CommandPanelRes::onChoiceDelaySelected ), NULL, this );
	choiceTimeScale->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CommandPanelRes::onChoiceTimeScaleSelected ), NULL, this );
	choiceRepeatCount->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CommandPanelRes::onChoiceRepeatSelected ), NULL, this );
	checkBoxYoyo->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CommandPanelRes::onCheckBoxYoyoClicked ), NULL, this );
	choiceRepeatDelay->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CommandPanelRes::onChoiceRepeatDelaySelected ), NULL, this );
	currentProgressSlider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderScroll ), NULL, this );
	currentProgressSlider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderScroll ), NULL, this );
	currentProgressSlider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderScroll ), NULL, this );
	currentProgressSlider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderScroll ), NULL, this );
	currentProgressSlider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderScroll ), NULL, this );
	currentProgressSlider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderScroll ), NULL, this );
	currentProgressSlider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderScroll ), NULL, this );
	currentProgressSlider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderScroll ), NULL, this );
	currentProgressSlider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderScroll ), NULL, this );
	currentProgressSlider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderThumbRelease ), NULL, this );
	currentProgressSlider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CommandPanelRes::onCurrentProgressSliderThumbTrack ), NULL, this );
	totalProgressSlider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderScroll ), NULL, this );
	totalProgressSlider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderScroll ), NULL, this );
	totalProgressSlider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderScroll ), NULL, this );
	totalProgressSlider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderScroll ), NULL, this );
	totalProgressSlider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderScroll ), NULL, this );
	totalProgressSlider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderScroll ), NULL, this );
	totalProgressSlider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderScroll ), NULL, this );
	totalProgressSlider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderScroll ), NULL, this );
	totalProgressSlider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderScroll ), NULL, this );
	totalProgressSlider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderThumbRelease ), NULL, this );
	totalProgressSlider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CommandPanelRes::onTotalProgressSliderThumbTrack ), NULL, this );
	
}
