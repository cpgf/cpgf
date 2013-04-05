#include "toolpanel.h"

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

TestCasePtr createTestCaseAnimation();
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
			testCase = createTestCaseChart();
			break;

		case 2:
			testCase = createTestCaseAllCharts();
			break;
	}
	this->onTestCaseChange(testCase);
}

