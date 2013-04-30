#include "toolpanel.h"

#include "cpgf/tween/gtweenlist.h"
#include "cpgf/accessor/gaccessor.h"
#include "../easeinfo.h"
#include "wx/timer.h"
#include "wx/msgdlg.h"
#include <vector>
using namespace std;
using namespace cpgf;

ToolPanel::ToolPanel(wxWindow * parent)
	: super(parent)
{
}

void ToolPanel::setOnTestCaseChange(const CallbackTestCaseChange & onTestCaseChange)
{
	this->onTestCaseChange = onTestCaseChange;
	this->doOnTestTypeSelected();
}

void ToolPanel::onTestTypeSelected( wxCommandEvent& event )
{
	this->doOnTestTypeSelected();
}

void benchmarkTweens()
{
	const int iteration = 1000 * 1000;
	const int duration = iteration / 2 + 10;
	const int target = iteration / 2;

	int easeCount = getEaseCount();
	vector<GTween *> tweens;
	GTweenList::getInstance()->clear();
	float x = 0;

	for(int i = 0; i < easeCount; ++i) {
		GTween * tween = &GTweenList::getInstance()->tween();
		tweens.push_back(tween);
		tween->duration((float)duration);
		tween->repeat(2);
		tween->useFrames(true);
		for(int k = 0; k < 3; ++k) {
			tween->target(createAccessor(NULL, &x, &x), (float)target);
		}
	}

	wxLongLong start = wxGetLocalTimeMillis();
	for(int i = 0; i < iteration; ++i) {
		for(int k = 0; k < easeCount; ++k) {
			tweens[k]->tick(1);
		}
	}

	wxLongLong end = wxGetLocalTimeMillis();

	int dt = (int)((end - start).ToLong());
	float ft = (float)dt / (float)(easeCount * iteration);
	wxString s = wxString::Format(
		"TweenCount: %d, Iterations: %d, Time: %d\n"
		"Average time per ease: %.6f (ms), %.6f (us)\n"
		"%d tweens can be executed in one second\n"
		"%d tweens can be executed in one frame in 60 FPS\n"
		"%d tweens can be executed in one millisecond\n"
		,
		easeCount, iteration, dt,
		ft, ft * 1000.0f,
		(int)(1000 / ft),
		(int)(1000 / (ft * 60)),
		(int)(1000 / (ft * 1000))
	);
	wxMessageBox(s);
}

void ToolPanel::onButtonBenchmarkClicked( wxCommandEvent& event )
{
	benchmarkTweens();
}

void ToolPanel::onButtonAboutThisTestCaseClicked( wxCommandEvent& event )
{
	wxMessageBox(this->currentTestCase->getDescription(), "About the test case");
}

TestCasePtr createTestCaseBasic();
TestCasePtr createTestCaseAnimation();
TestCasePtr createTestCaseFollow();
TestCasePtr createTestCaseTimeline();
TestCasePtr createTestCaseParticles();
TestCasePtr createTestCaseChart();
TestCasePtr createTestCaseAllCharts();

void ToolPanel::doOnTestTypeSelected()
{
	int selection = this->choiceTestType->GetCurrentSelection();
	TestCasePtr testCase;
	switch(selection) {
		case 0:
			testCase = createTestCaseBasic();
			break;

		case 1:
			testCase = createTestCaseAnimation();
			break;

		case 2:
			testCase = createTestCaseFollow();
			break;

		case 3:
			testCase = createTestCaseTimeline();
			break;
			
		case 4:
			testCase = createTestCaseParticles();
			break;

		case 5:
			testCase = createTestCaseChart();
			break;

		case 6:
			testCase = createTestCaseAllCharts();
			break;
	}
	this->currentTestCase = testCase;
	this->onTestCaseChange(testCase);
}

