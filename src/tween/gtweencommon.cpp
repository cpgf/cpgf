#include "cpgf/tween/gtweencommon.h"

#include <cmath>
#include <algorithm>

using namespace std;


namespace cpgf {

const GTweenNumber invalidPreviousAppliedTime = -1.0f;

GTweenable::GTweenable()
	: elapsedTime(0), delayTime(0), repeatDelayTime(0), repeatCount(0), cycleCount(0), timeScaleTime(1.0f), flags(), previousAppliedTime(invalidPreviousAppliedTime)
{
}

GTweenable::~GTweenable()
{
	if(this->callbackOnDestroy) {
		this->callbackOnDestroy();
	}
}

void GTweenable::doTick(GTweenNumber frameDuration, bool forceReversed, bool forceUseFrames)
{
	if(this->isCompleted()) {
		return;
	}

	if(this->isPaused()) {
		return;
	}

	GTweenNumber d = 0.0f;
	if(frameDuration > 0) {
		frameDuration *= this->timeScaleTime;
		this->elapsedTime += frameDuration;
		if(this->elapsedTime <= this->delayTime) {
			return;
		}

		if(this->repeatCount >= 0) {
			GTweenNumber total = this->getTotalDuration();
			if(this->elapsedTime > total + this->delayTime) {
				this->elapsedTime = total + this->delayTime;
			}
		}

		d = this->elapsedTime - this->delayTime;
	}
	else {
		if(this->elapsedTime > this->delayTime) {
			d = this->elapsedTime - this->delayTime;
		}
	}

	if(! this->flags.has(tfInitialized)) {
		this->flags.set(tfInitialized);

		if(this->callbackOnInitialize) {
			this->callbackOnInitialize();
		}

		this->initialize();
	}

	this->performTime(d, frameDuration, forceReversed, forceUseFrames);
}

void GTweenable::doComplete(bool emitEvent)
{
	if(this->isCompleted()) {
		return;
	}

	this->flags.set(tfCompleted);
	this->elapsedTime = this->getTotalDuration() + this->delayTime;

	if(emitEvent && this->callbackOnComplete) {
		this->callbackOnComplete();
	}
}

void GTweenable::doRestartChildren()
{
}

void GTweenable::doRestartChildrenWithDelay()
{
}

void GTweenable::initialize()
{
}

GTweenNumber GTweenable::getTotalDuration() const
{
	if(this->repeatCount >= 0) {
		return this->getDuration() * (this->repeatCount + 1) + this->repeatDelayTime * this->repeatCount;
	}
	else {
		return this->getDuration() * 99999999.0f + this->repeatDelayTime * (99999999.0f - 1.0f);
	}
}

GTweenNumber GTweenable::getCurrentTime() const
{
	if(this->elapsedTime <= this->delayTime) {
		return 0.0f;
	}
	
	if(this->cycleCount > 0) {
		return this->elapsedTime - this->delayTime - ((this->getDuration() + this->repeatDelayTime) * this->cycleCount);
	}
	else {
		return this->elapsedTime - this->delayTime;
	}
}

void GTweenable::setCurrentTime(GTweenNumber value)
{
	if(value < 0.0f) {
		value = 0.0f;
	}
	GTweenNumber d = this->getDuration();
	if(value > d) {
		value = d;
	}
	if(this->cycleCount > 0) {
		value += (d + this->repeatDelayTime) * this->cycleCount;
	}
	this->elapsedTime = min(value, this->getTotalDuration()) + this->delayTime;
	
	this->doRestartChildren();
}

GTweenNumber GTweenable::getTotalTime() const
{
	return this->elapsedTime - this->delayTime;
}

void GTweenable::setTotalTime(GTweenNumber value)
{
	if(value < 0.0f) {
		value = 0.0f;
	}
	this->elapsedTime = min(value, this->getTotalDuration()) + this->delayTime;

	GTweenNumber cycleDuration = this->getDuration() + this->repeatDelayTime;
	if(cycleDuration > 0) {
		this->cycleCount = (int)(floor(this->elapsedTime / cycleDuration));
	}
	else {
		this->cycleCount = 0;
	}
	
	this->doRestartChildren();
}

GTweenNumber GTweenable::getCurrentProgress() const
{
	GTweenNumber d = this->getDuration();
	if(d <= 0.0f) {
		return 0.0f;
	}
	return this->getCurrentTime() / d;
}

void GTweenable::setCurrentProgress(GTweenNumber value)
{
	this->setCurrentTime(value * this->getDuration());
}

GTweenNumber GTweenable::getTotalProgress() const
{
	GTweenNumber d = this->getTotalDuration();
	if(d <= 0.0f) {
		return 0.0f;
	}
	return this->getTotalTime() / d;
}

void GTweenable::setTotalProgress(GTweenNumber value)
{
	this->setTotalTime(value * this->getTotalDuration());
}

void GTweenable::pause()
{
	this->flags.set(tfPaused);
}

void GTweenable::resume()
{
	this->flags.clear(tfPaused);
}

void GTweenable::restart()
{
	this->elapsedTime = this->delayTime;
	this->previousAppliedTime = invalidPreviousAppliedTime;
	this->cycleCount = 0;
	this->flags.clear(tfCompleted);
	
	this->doRestartChildren();
}

void GTweenable::restartWithDelay()
{
	this->elapsedTime = 0;
	this->previousAppliedTime = invalidPreviousAppliedTime;
	this->cycleCount = 0;
	this->flags.clear(tfCompleted);
	
	this->doRestartChildrenWithDelay();
}

GTweenable & GTweenable::backward(bool value)
{
	this->flags.setByBool(tfBackward, value);
	return *this;
}

GTweenable & GTweenable::useFrames(bool value)
{
	this->flags.setByBool(tfUseFrames, value);
	return *this;
}

GTweenable & GTweenable::delay(GTweenNumber value)
{
	this->delayTime = value;
	return *this;
}

GTweenable & GTweenable::timeScale(GTweenNumber value)
{
	this->timeScaleTime = value;
	return *this;
}

GTweenable & GTweenable::repeat(int repeatCount)
{
	this->repeatCount = repeatCount;
	return *this;
}

GTweenable & GTweenable::repeatDelay(GTweenNumber value)
{
	this->repeatDelayTime = value;
	return *this;
}

GTweenable & GTweenable::yoyo(bool value)
{
	this->flags.setByBool(tfReverseWhenRepeat, value);
	return *this;
}

GTweenable & GTweenable::onInitialize(const GTweenCallback & value)
{
	this->callbackOnInitialize = value;
	return *this;
}

GTweenable & GTweenable::onComplete(const GTweenCallback & value)
{
	this->callbackOnComplete = value;
	return *this;
}

GTweenable & GTweenable::onDestroy(const GTweenCallback & value)
{
	this->callbackOnDestroy = value;
	return *this;
}

GTweenable & GTweenable::onUpdate(const GTweenCallback & value)
{
	this->callbackOnUpdate = value;
	return *this;
}

GTweenable & GTweenable::onRepeat(const GTweenCallback & value)
{
	this->callbackOnRepeat = value;
	return *this;
}

void GTweenable::immediateTick()
{
	bool paused = this->isPaused();
	this->flags.clear(tfPaused);

	this->doTick(0, this->isBackward(), false);

	this->flags.setByBool(tfPaused, paused);
}



} // namespace cpgf

