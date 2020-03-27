#include "mainframe.h"

MainFrame::MainFrame()
	: super(nullptr, wxID_ANY, wxT("cpgf Tween library demo"), wxDefaultPosition, wxSize(1000, 600))
{
	this->tweenPanel = new PanelTweenTest(this);
	this->tweenPanel->start();
}
