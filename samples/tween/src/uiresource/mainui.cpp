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
	
	wxString choiceTestTypeChoices[] = { wxT("Animation"), wxT("Follow"), wxT("Timeline"), wxT("Particles"), wxT("Chart"), wxT("All Charts") };
	int choiceTestTypeNChoices = sizeof( choiceTestTypeChoices ) / sizeof( wxString );
	choiceTestType = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choiceTestTypeNChoices, choiceTestTypeChoices, 0 );
	choiceTestType->SetSelection( 0 );
	choiceTestType->SetMinSize( wxSize( 120,-1 ) );
	
	bSizer1->Add( choiceTestType, 0, wxALL, 5 );
	
	buttonReset = new wxButton( this, wxID_ANY, wxT("&Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( buttonReset, 0, wxALL, 5 );
	
	
	bSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	buttonBenchmark = new wxButton( this, wxID_ANY, wxT("&Benchmark"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( buttonBenchmark, 0, wxALL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	choiceTestType->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ToolPanelRes::onTestTypeSelected ), NULL, this );
	buttonReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ToolPanelRes::onButtonResetClicked ), NULL, this );
	buttonBenchmark->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ToolPanelRes::onButtonBenchmarkClicked ), NULL, this );
}

ToolPanelRes::~ToolPanelRes()
{
	// Disconnect Events
	choiceTestType->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ToolPanelRes::onTestTypeSelected ), NULL, this );
	buttonReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ToolPanelRes::onButtonResetClicked ), NULL, this );
	buttonBenchmark->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ToolPanelRes::onButtonBenchmarkClicked ), NULL, this );
	
}
