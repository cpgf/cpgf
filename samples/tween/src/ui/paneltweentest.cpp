#include "paneltweentest.h"
#include "panelcanvas.h"
#include "panelease.h"
#include "toolpanel.h"
#include "easeinfo.h"

#include "cpgf/tween/gtweenlist.h"
#include "cpgf/accessor/gaccessor.h"

using namespace cpgf;

PanelTweenTest::PanelTweenTest(wxWindow * parent)
	: super(parent), timer(NULL)
{
	wxBoxSizer * mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	this->toolPanel = new ToolPanel(this);
	mainSizer->Add(this->toolPanel);

	wxBoxSizer * contentSizer;
	contentSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(contentSizer, 1, wxEXPAND, 5);

	wxBoxSizer * leftSizer;
	leftSizer = new wxBoxSizer(wxVERTICAL);

	this->easePanel = new PanelEase(this);
	leftSizer->Add(this->easePanel, 0, wxEXPAND, 5);

	contentSizer->Add(leftSizer, 1, wxEXPAND, 5);
	
	wxBoxSizer * canvasSizer;
	canvasSizer = new wxBoxSizer(wxVERTICAL);
	
	contentSizer->Add(canvasSizer, 3, wxEXPAND, 5);
	
	this->canvas = new PanelCanvas(this);
	canvasSizer->Add(this->canvas, 1, wxEXPAND, 5); //->SetMinSize(wxSize(500, 500));

	this->easePanel->addCallback(makeCallback(this, &PanelTweenTest::onEaseButtonClicked));
	this->canvas->addRender(makeCallback(this, &PanelTweenTest::onRender));
	this->toolPanel->setOnTestCaseChange(makeCallback(this, &PanelTweenTest::onTestCaseChanged));

	this->SetSizer(mainSizer);
	this->Layout();
}

PanelTweenTest::~PanelTweenTest()
{
	this->end();
}

void PanelTweenTest::onEaseButtonClicked(int easeIndex)
{
	if(this->testCase) {
		this->testCase->setEase(easeIndex);
	}
}

void PanelTweenTest::start()
{
	this->end();

	this->timer = new TestTimer(cpgf::makeCallback(this, &PanelTweenTest::onTimer));
}

void PanelTweenTest::end()
{
	delete this->timer;
	this->timer = NULL;
}

void PanelTweenTest::setTestCase(const TestCasePtr & testCase)
{
	this->testCase = testCase;
	this->easePanel->Show(this->testCase->shouldShowEaseButtons());
	this->Layout();
}

void PanelTweenTest::onRender(int viewWidth, int viewHeight)
{
	if(this->testCase) {
		this->testCase->render(viewWidth, viewHeight);
	}
}

void PanelTweenTest::onTestCaseChanged(const TestCasePtr & testCase)
{
	this->setTestCase(testCase);
}

void PanelTweenTest::onTimer()
{
	GTweenList::getInstance()->tick((GTweenNumber)FrameTime);

	this->canvas->Refresh();
}
