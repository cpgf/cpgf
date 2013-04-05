#include "panelease.h"
#include "easeinfo.h"

using namespace cpgf;

PanelEase::PanelEase(wxWindow * parent)
	: super(parent)
{
	wxBoxSizer * mainSizer;
	mainSizer = new wxBoxSizer(wxVERTICAL);

	wxGridSizer * easeSizer;
	easeSizer = new wxGridSizer(3, 5, 3);

	mainSizer->Add(easeSizer, 0, wxEXPAND, 5);
	
	this->initEaseButtons(easeSizer);

	this->SetSizer(mainSizer);
	this->Layout();
}

PanelEase::~PanelEase()
{
}

const int EaseButtonStartID = 1000;

void PanelEase::initEaseButtons(wxSizer * sizer)
{
	for(int i = 0; i < getEaseCount(); ++i) {
		wxButton * button = new wxButton(this, EaseButtonStartID + i, getEase(i)->name, wxDefaultPosition, wxSize(150, 30), 0);
		sizer->Add(button);
		button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(PanelEase::onEaseButtonClicked), NULL, this );
	}
}

void PanelEase::onEaseButtonClicked(wxCommandEvent& e)
{
	int id = e.GetId() - EaseButtonStartID;
	this->callbackList.dispatch(id);
}

void PanelEase::addCallback(const CallbackType & callback)
{
	this->callbackList.add(callback);
}

