#include "paneltweentest.h"
#include "panelcanvas.h"
#include "panelease.h"
#include "panelcommand.h"
#include "toolpanel.h"
#include "../easeinfo.h"

#include "cpgf/tween/gtweenlist.h"
#include "cpgf/accessor/gaccessor.h"

#if defined(_WIN32)
    #include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glut.h>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif


using namespace cpgf;

PanelTweenTest::PanelTweenTest(wxWindow * parent)
	: super(parent), timer(NULL)
{
	wxBoxSizer * mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	this->toolPanel = new ToolPanel(this);
	mainSizer->Add(this->toolPanel, 0, wxEXPAND);

	wxBoxSizer * contentSizer;
	contentSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(contentSizer, 1, wxEXPAND, 5);

	wxBoxSizer * leftSizer;
	leftSizer = new wxBoxSizer(wxVERTICAL);
	
	this->commandPanel = new PanelCommand(this);
	leftSizer->Add(this->commandPanel, 0, wxEXPAND, 5);

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
	this->testCase->reset();
	this->testCase->setEase(this->easePanel->getEaseIndex());

	this->commandPanel->setTestCase(testCase);
	this->commandPanel->Show(this->testCase->shouldShowCommandButtons());
	this->commandPanel->showOrHidePauseAndResumeButtons(this->testCase->shouldShowPauseResumeButtons());
	this->easePanel->Show(this->testCase->shouldShowEaseButtons());
	
	this->Layout();
	this->commandPanel->Refresh();
	this->easePanel->Refresh();
}

void drawFrameRate(int frameRate)
{
	if(frameRate == 0) {
		return;
	}

	char str[100];
	sprintf(str, "FPS: %d", frameRate);

	glColor4f(0, 1.0f, 0, 0.5f);
	glPushAttrib(GL_LIGHTING_BIT + GL_CURRENT_BIT);
	glDisable(GL_LIGHTING);
	glRasterPos3f(1.8f, 0.05f, 0.0f);
	int i = 0;
	while(str[i]) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
		++i;
	}
	glEnable(GL_LIGHTING);
	glPopAttrib();
}


void PanelTweenTest::onRender(int viewWidth, int viewHeight)
{
	if(this->testCase) {
		this->testCase->render(viewWidth, viewHeight);
	}
	drawFrameRate(this->timer->getFrameRate());
}

void PanelTweenTest::onTestCaseChanged(const TestCasePtr & testCase)
{
	this->setTestCase(testCase);
}

void PanelTweenTest::onTimer(int frameTime)
{
	GTweenList::getInstance()->tick((GTweenNumber)frameTime);
	
	this->commandPanel->tick();

	this->canvas->Refresh();
}
