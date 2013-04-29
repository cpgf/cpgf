#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include "testcase.h"

#include "../easeinfo.h"
#include "cpgf/tween/easing/linear.h"

#include "cpgf/tween/gtween.h"
#include "cpgf/tween/gtimeline.h"

#include "wx/log.h"

using namespace cpgf;

TestCase::TestCase()
	: tweenable(NULL), ease(LinearEase::ease())
{
}

TestCase::~TestCase()
{
	this->setTweenable(NULL);
}

bool TestCase::shouldShowCommandButtons()
{
	return true;
}

bool TestCase::shouldShowEaseButtons()
{
	return true;
}

void TestCase::resetEase()
{
	if(this->tweenable != NULL) {
		if(dynamic_cast<GTween *>(this->tweenable)) {
			dynamic_cast<GTween *>(this->tweenable)->ease(this->ease);
		}
	}
}

void TestCase::setEase(int easeIndex)
{
	this->ease = getEase(easeIndex)->ease;
	this->resetEase();
}

void TestCase::setTweenParam(const TweenParam & tweenParam)
{
	this->tweenParam = tweenParam;
	this->resetTweenParam();
}

void TestCase::resetTweenParam()
{
	if(this->tweenable != NULL) {
		if(dynamic_cast<GTween *>(this->tweenable)) {
			GTween * t = dynamic_cast<GTween *>(this->tweenable);
			t->duration(this->tweenParam.duration);

			t->useFrames(this->tweenParam.useFrames);
			t->backward(this->tweenParam.backward);
			t->delay(this->tweenParam.delay);
			t->timeScale(this->tweenParam.timeScale);
			t->repeat(this->tweenParam.repeat);
			t->yoyo(this->tweenParam.yoyo);
			t->repeatDelay(this->tweenParam.repeatDelay);
		}
		else {
			GTimeline * t = dynamic_cast<GTimeline *>(this->tweenable);

			t->useFrames(this->tweenParam.useFrames);
			t->backward(this->tweenParam.backward);
			t->delay(this->tweenParam.delay);
			t->timeScale(this->tweenParam.timeScale);
			t->repeat(this->tweenParam.repeat);
			t->yoyo(this->tweenParam.yoyo);
			t->repeatDelay(this->tweenParam.repeatDelay);

			t->immediateTick();
		}
	}
}

void TestCase::reset()
{
	GTweenList::getInstance()->clear();

	this->doReset();

	this->resetTweenParam();
}

void TestCase::doReset()
{
}

void TestCase::play()
{
	this->reset();

	if(this->tweenable != NULL) {
		this->tweenable->restartWithDelay();
		this->tweenable->resume();
	}
}

void TestCase::pause()
{
	if(this->tweenable != NULL) {
		this->tweenable->pause();
	}
}

void TestCase::resume()
{
	if(this->tweenable != NULL) {
		this->tweenable->resume();
	}
}

struct OnComplete
{
	OnComplete() {}
	void operator() () const {
		wxLogDebug("Tween complete");
	}
};

void TestCase::setTweenable(GTweenable * tweenable)
{
	this->detachCurrentTweenable();
	this->tweenable = tweenable;

	if(this->tweenable != NULL) {
		if(dynamic_cast<GTween *>(this->tweenable)) {
			dynamic_cast<GTween *>(this->tweenable)->onComplete(OnComplete())
				.onDestroy(makeCallback(this, &TestCase::onTweenableDestroy))
				.pause()
			;
		}
		else {
			dynamic_cast<GTimeline *>(this->tweenable)->onComplete(OnComplete())
				.onDestroy(makeCallback(this, &TestCase::onTweenableDestroy))
				.pause()
			;
		}
	}
	this->resetEase();
	this->resetTweenParam();
}

void TestCase::onTweenableDestroy()
{
	this->tweenable = NULL;
}

void TestCase::detachCurrentTweenable()
{
	if(this->tweenable != NULL) {
		if(dynamic_cast<GTween *>(this->tweenable)) {
			dynamic_cast<GTween *>(this->tweenable)->onDestroy(GTweenCallback());
		}
		else {
			dynamic_cast<GTimeline *>(this->tweenable)->onDestroy(GTweenCallback());
		}
	}
}
