#ifndef __TESTCASE_H
#define __TESTCASE_H

#include "cpgf/gsharedptr.h"
#include "cpgf/tween/gtweencommon.h"

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

	virtual void render(int viewWidth, int viewHeight) = 0;
	void setEase(int easeIndex);
	
	virtual bool shouldShowCommandButtons();
	virtual bool shouldShowEaseButtons();
	
	void reset();
	void play();
	void pause();
	void resume();
	void setTweenParam(const TweenParam & tweenParam);

protected:
	virtual void doReset();

	void setTweenable(cpgf::GTweenable * tweenable);
	cpgf::GTweenNumber getDuration() const {
		return this->tweenParam.duration;
	}

	void resetEase();
	void resetTweenParam();

private:
	void onTweenableDestroy();

private:
	cpgf::GTweenable * tweenable;
	cpgf::GTweenEaseType ease;
	TweenParam tweenParam;
};

typedef cpgf::GSharedPointer<TestCase> TestCasePtr;

#endif
