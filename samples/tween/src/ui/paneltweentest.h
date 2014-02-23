#ifndef CPGF_PANELTWEENTEST_H
#define CPGF_PANELTWEENTEST_H

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include "timer.h"
#include "../testcases/testcase.h"

#include "wx/panel.h"
#include "wx/sizer.h"
#include "wx/button.h"

class ToolPanel;
class PanelCanvas;
class PanelEase;
class PanelCommand;

class PanelTweenTest : public wxPanel
{
private:
	typedef wxPanel super;

public:
	explicit PanelTweenTest(wxWindow * parent);
	~PanelTweenTest();

	void start();
	void end();

	void setTestCase(const TestCasePtr & testCase);

private:
	void onEaseButtonClicked(int easeIndex);
	void onRender(int viewWidth, int viewHeight);
	void onTestCaseChanged(const TestCasePtr & testCase);
	void onTimer(int frameTime);

private:
	ToolPanel * toolPanel;
	PanelEase * easePanel;
	PanelCanvas * canvas;
	PanelCommand * commandPanel;
	TestTimer * timer;
	TestCasePtr testCase;
};


#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
