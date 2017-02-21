#ifndef CPGF_PANELCOMMAND_H
#define CPGF_PANELCOMMAND_H

#include "../uiresource/mainui.h"

#include "../testcases/testcase.h"

#include <vector>


class PanelCommand : public CommandPanelRes
{
private:
	typedef CommandPanelRes super;
	
public:
	PanelCommand(wxWindow* parent);
	
	void setTestCase(const TestCasePtr & testCase);
	void showOrHidePauseAndResumeButtons(bool show);
	void tick();
	
protected:
	virtual void onButtonPlayClicked( wxCommandEvent& event );
	virtual void onButtonPauseClicked( wxCommandEvent& event );
	virtual void onButtonResumeClicked( wxCommandEvent& event );
	virtual void onButtonResetClicked( wxCommandEvent& event );
	virtual void onCheckBoxUseFramesClicked( wxCommandEvent& event );
	virtual void onCheckBoxBackwardClicked( wxCommandEvent& event );
	virtual void onChoiceDurationSelected( wxCommandEvent& event );
	virtual void onChoiceDelaySelected( wxCommandEvent& event );
	virtual void onChoiceTimeScaleSelected( wxCommandEvent& event );
	virtual void onChoiceRepeatSelected( wxCommandEvent& event );
	virtual void onCheckBoxYoyoClicked( wxCommandEvent& event );
	virtual void onChoiceRepeatDelaySelected( wxCommandEvent& event );
	virtual void onCurrentProgressSliderScroll( wxScrollEvent& event );
	virtual void onTotalProgressSliderScroll( wxScrollEvent& event );
	virtual void onCurrentProgressSliderThumbRelease( wxScrollEvent& event ) { this->enableUpdateCurrentProgressSlider = true; }
	virtual void onCurrentProgressSliderThumbTrack( wxScrollEvent& event ) { this->enableUpdateCurrentProgressSlider = false; }
	virtual void onTotalProgressSliderThumbRelease( wxScrollEvent& event ) { this->enableUpdateTotalProgressSlider = true; }
	virtual void onTotalProgressSliderThumbTrack( wxScrollEvent& event ) { this->enableUpdateTotalProgressSlider = false; }

private:
	void resetTimeChoicesUnit();
	void loadTweenParam();
	int getTimeChoiceSelection(wxChoice * choice);

private:
	TestCasePtr testCase;
	std::vector<wxChoice *> timeChoiceList;
	TweenParam tweenParam;
	bool enableUpdateCurrentProgressSlider;
	bool enableUpdateTotalProgressSlider;
};


#endif
