#ifndef CPGF_TESTCASE_H
#define CPGF_TESTCASE_H

#include "cpgf/gsharedptr.h"
#include "cpgf/tween/gtweencommon.h"

#include <string>

struct TweenParam
{
	TweenParam()
		: useFrames(false), backward(false), duration(2000), delay(0), timeScale(1.0), repeat(0), yoyo(false), repeatDelay(0)
	{}

	bool useFrames;
	bool backward;
	cpgf::GTweenNumber duration;
	cpgf::GTweenNumber delay;
	cpgf::GTweenNumber timeScale;
	int repeat;
	bool yoyo;
	cpgf::GTweenNumber repeatDelay;
};

class TestCase
{
public:
	TestCase();
	virtual ~TestCase();

	virtual std::string getDescription() = 0;
	virtual void render(int viewWidth, int viewHeight) = 0;
	virtual void setEase(int easeIndex);
	
	virtual bool shouldShowCommandButtons();
	virtual bool shouldShowEaseButtons();
	virtual bool shouldShowPauseResumeButtons();
	
	void reset();
	void play();
	void pause();
	void resume();
	void setTweenParam(const TweenParam & tweenParam);
	
	cpgf::GTweenable * getTweenable() const {
		return this->tweenable;
	}

protected:
	virtual void doReset();

	void setTweenable(cpgf::GTweenable * tweenable);
	cpgf::GTweenNumber getDuration() const {
		return this->tweenParam.duration;
	}

	bool useFrames() const {
		return this->tweenParam.useFrames;
	}

	void resetEase();
	void resetTweenParam();

private:
	void onTweenableDestroy();
	void detachCurrentTweenable();

private:
	cpgf::GTweenable * tweenable;
	cpgf::GTweenEaseType ease;
	TweenParam tweenParam;
};

typedef cpgf::GSharedPointer<TestCase> TestCasePtr;

#endif
