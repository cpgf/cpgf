#ifndef CPGF_TOOLPANEL_H
#define CPGF_TOOLPANEL_H

#include "../uiresource/mainui.h"

#include "../testcases/testcase.h"
#include "cpgf/gcallback.h"


class ToolPanel : public ToolPanelRes
{
private:
	typedef ToolPanelRes super;
	typedef cpgf::GCallback<void (const TestCasePtr & testCase)> CallbackTestCaseChange;
	
public:
	ToolPanel(wxWindow * parent);

	void setOnTestCaseChange(const CallbackTestCaseChange & onTestCaseChange);

protected:
	virtual void onTestTypeSelected( wxCommandEvent& event );
	virtual void onButtonBenchmarkClicked( wxCommandEvent& event );
	virtual void onButtonAboutThisTestCaseClicked( wxCommandEvent& event );
	
	void doOnTestTypeSelected();

private:
	CallbackTestCaseChange onTestCaseChange;
	TestCasePtr currentTestCase;
};


#endif
