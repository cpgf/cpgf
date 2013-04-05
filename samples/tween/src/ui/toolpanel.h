#ifndef __TOOLPANEL_H
#define __TOOLPANEL_H

#include "mainui.h"

#include "testcase.h"
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
	void doOnTestTypeSelected();

private:
	CallbackTestCaseChange onTestCaseChange;
};


#endif
