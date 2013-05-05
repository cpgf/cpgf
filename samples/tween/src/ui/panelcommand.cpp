#include "panelcommand.h"

#include "cpgf/tween/gtweencommon.h"

using namespace std;

namespace {

const int TimeCount = 7;

const int TimeMilliseconds[] = {
	0, 1000, 2000, 3000, 4000, 5000, 6000
};

const int TimeFrames[] = {
	0, 60, 120, 180, 240, 300, 360
};

const float TimeScales[] = {
	0.2f, 0.5f, 1.0f, 1.5f, 2.0f, 3.0f, 5.0f
};

}

PanelCommand::PanelCommand(wxWindow* parent)
	: super(parent), enableUpdateCurrentProgressSlider(true), enableUpdateTotalProgressSlider(true)
{
	this->totalProgressSlider->SetPageSize(this->totalProgressSlider->GetMax() / 10);

	this->timeChoiceList.push_back(this->choiceDuration);
	this->timeChoiceList.push_back(this->choiceDelay);
	this->timeChoiceList.push_back(this->choiceRepeatDelay);

	this->resetTimeChoicesUnit();
}

void PanelCommand::resetTimeChoicesUnit()
{
	const int * times = this->tweenParam.useFrames ? TimeFrames : TimeMilliseconds;
	const char * postfix = this->tweenParam.useFrames ? "frames" : "ms";
	for(size_t i = 0; i < this->timeChoiceList.size(); ++i) {
		int selection = this->timeChoiceList[i]->GetSelection();
		for(int k = 0; k < TimeCount; ++k) {
			wxString s = wxString::Format("%d %s", times[k], postfix);
			this->timeChoiceList[i]->SetString(k, s);
		}
		this->timeChoiceList[i]->SetSelection(selection);
	}
}

int PanelCommand::getTimeChoiceSelection(wxChoice * choice)
{
	const int * times = this->tweenParam.useFrames ? TimeFrames : TimeMilliseconds;
	return times[choice->GetSelection()];
}

void PanelCommand::loadTweenParam()
{
	this->tweenParam.useFrames = this->checkBoxUseFrames->GetValue();
	this->tweenParam.backward = this->checkBoxBackward->GetValue();
	this->tweenParam.duration = this->getTimeChoiceSelection(this->choiceDuration);
	this->tweenParam.delay = this->getTimeChoiceSelection(this->choiceDelay);
	this->tweenParam.repeatDelay = this->getTimeChoiceSelection(this->choiceRepeatDelay);
	this->tweenParam.repeat = this->choiceRepeatCount->GetSelection() - 1;
	this->tweenParam.yoyo = this->checkBoxYoyo->GetValue();
	this->tweenParam.timeScale = TimeScales[this->choiceTimeScale->GetSelection()];
}

void PanelCommand::setTestCase(const TestCasePtr & testCase)
{
	this->testCase = testCase;
	this->loadTweenParam();
	this->testCase->setTweenParam(this->tweenParam);
}

void PanelCommand::showOrHidePauseAndResumeButtons(bool show)
{
	this->buttonPause->Show(show);
	this->buttonResume->Show(show);
	this->sizerParameters->Show(show);
}

void PanelCommand::tick()
{
	if(this->enableUpdateCurrentProgressSlider && this->enableUpdateTotalProgressSlider) {
		cpgf::GTweenable * tweenable = this->testCase->getTweenable();
		if(tweenable != NULL) {
			float progress;

			progress = tweenable->getCurrentProgress();
			progress *= (float)(this->currentProgressSlider->GetMax());
			this->currentProgressSlider->SetValue((int)progress);

			progress = tweenable->getTotalProgress();
			progress *= (float)(this->totalProgressSlider->GetMax());
			this->totalProgressSlider->SetValue((int)progress);
		}
	}
}

void PanelCommand::onButtonPlayClicked( wxCommandEvent& event )
{
	this->testCase->play();
}

void PanelCommand::onButtonPauseClicked( wxCommandEvent& event )
{
	this->testCase->pause();
}

void PanelCommand::onButtonResumeClicked( wxCommandEvent& event )
{
	this->testCase->resume();
}

void PanelCommand::onButtonResetClicked( wxCommandEvent& event )
{
	this->testCase->reset();
}

void PanelCommand::onCheckBoxUseFramesClicked( wxCommandEvent& event )
{
	this->loadTweenParam();
	this->testCase->setTweenParam(this->tweenParam);

	this->resetTimeChoicesUnit();
}

void PanelCommand::onCheckBoxBackwardClicked( wxCommandEvent& event )
{
	this->loadTweenParam();
	this->testCase->setTweenParam(this->tweenParam);
}

void PanelCommand::onChoiceDurationSelected( wxCommandEvent& event )
{
	this->loadTweenParam();
	this->testCase->setTweenParam(this->tweenParam);
}

void PanelCommand::onChoiceDelaySelected( wxCommandEvent& event )
{
	this->loadTweenParam();
	this->testCase->setTweenParam(this->tweenParam);
}

void PanelCommand::onChoiceTimeScaleSelected( wxCommandEvent& event )
{
	this->loadTweenParam();
	this->testCase->setTweenParam(this->tweenParam);
}

void PanelCommand::onChoiceRepeatSelected( wxCommandEvent& event )
{
	this->loadTweenParam();
	this->testCase->setTweenParam(this->tweenParam);
}

void PanelCommand::onCheckBoxYoyoClicked( wxCommandEvent& event )
{
	this->loadTweenParam();
	this->testCase->setTweenParam(this->tweenParam);
}

void PanelCommand::onChoiceRepeatDelaySelected( wxCommandEvent& event )
{
	this->loadTweenParam();
	this->testCase->setTweenParam(this->tweenParam);
}

void PanelCommand::onCurrentProgressSliderScroll( wxScrollEvent& event )
{
	cpgf::GTweenable * tweenable = this->testCase->getTweenable();
	if(tweenable != NULL) {
		float progress = (float)(this->currentProgressSlider->GetValue());
		progress /= (float)(this->totalProgressSlider->GetMax());
		if(tweenable->isCompleted()) {
			tweenable->restart();
		}
		tweenable->setCurrentProgress(progress);
		tweenable->immediateTick();
	}
}

void PanelCommand::onTotalProgressSliderScroll( wxScrollEvent& event )
{
	cpgf::GTweenable * tweenable = this->testCase->getTweenable();
	if(tweenable != NULL) {
		float progress = (float)(this->totalProgressSlider->GetValue());
		progress /= (float)(this->totalProgressSlider->GetMax());
		if(! tweenable->isRunning() || tweenable->isCompleted()) {
			tweenable->restart();
		}
		tweenable->setTotalProgress(progress);
		tweenable->immediateTick();
	}
}

