#include "toolpanel.h"

#include "cpgf/tween/gtweenlist.h"
#include "cpgf/accessor/gaccessor.h"
#include "easeinfo.h"
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

void ToolPanel::onButtonResetClicked( wxCommandEvent& event )
{
	this->currentTestCase->reset();
}

void benchmarkTweens()
{
	const int iteration = 1000 * 1000;
	const int duration = iteration / 2 + 10;
	const int target = iteration / 2;

	int easeCount = getEaseCount();
	vector<GTween *> tweens;
	vector<float> values[3];
	GTweenList::getInstance()->clear();
	float x = 0;

	for(int i = 0; i < easeCount; ++i) {
		for(int k = 0; k < 3; ++k) {
			values[k].push_back(0);
		}
	}
	for(int i = 0; i < easeCount; ++i) {
		GTween * tween = &GTweenList::getInstance()->to((float)duration);
		tweens.push_back(tween);
		tween->repeat(2);
		tween->useFrames(true);
		for(int k = 0; k < 3; ++k) {
			tween->tween(createAccessor(NULL, &values[k][i], &values[k][i]), (float)target);
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

TestCasePtr createTestCaseAnimation();
TestCasePtr createTestCaseFollow();
TestCasePtr createTestCaseChart();
TestCasePtr createTestCaseAllCharts();

void ToolPanel::doOnTestTypeSelected()
{
	int selection = this->choiceTestType->GetCurrentSelection();
	TestCasePtr testCase;
	switch(selection) {
		case 0:
			testCase = createTestCaseAnimation();
			break;

		case 1:
			testCase = createTestCaseFollow();
			break;

		case 2:
			testCase = createTestCaseChart();
			break;

		case 3:
			testCase = createTestCaseAllCharts();
			break;
	}
	this->currentTestCase = testCase;
	this->onTestCaseChange(testCase);
}

