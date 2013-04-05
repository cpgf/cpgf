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
	
	wxString choiceTestTypeChoices[] = { wxT("Animation"), wxT("Chart"), wxT("All Charts") };
	int choiceTestTypeNChoices = sizeof( choiceTestTypeChoices ) / sizeof( wxString );
	choiceTestType = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choiceTestTypeNChoices, choiceTestTypeChoices, 0 );
	choiceTestType->SetSelection( 0 );
	bSizer1->Add( choiceTestType, 0, wxALL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	choiceTestType->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ToolPanelRes::onTestTypeSelected ), NULL, this );
}

ToolPanelRes::~ToolPanelRes()
{
	// Disconnect Events
	choiceTestType->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ToolPanelRes::onTestTypeSelected ), NULL, this );
	
}
