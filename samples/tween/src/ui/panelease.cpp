#include "panelease.h"
#include "easeinfo.h"

#include "wx/radiobut.h"

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
		wxRadioButton * button = new wxRadioButton(this, EaseButtonStartID + i, getEase(i)->name, wxDefaultPosition, wxSize(150, 30), 0);
		sizer->Add(button);
		button->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(PanelEase::onEaseButtonClicked), NULL, this );
		if(i == getEaseCount() - 1) {
			button->SetValue(true);
			this->easeIndex = i;
		}
	}
}

void PanelEase::onEaseButtonClicked(wxCommandEvent& e)
{
	this->easeIndex = e.GetId() - EaseButtonStartID;
	this->callbackList.dispatch(this->easeIndex);
}

void PanelEase::addCallback(const CallbackType & callback)
{
	this->callbackList.add(callback);
}

