#include "cpgf/tween/gtweencommon.h"

#include <cmath>
#include <algorithm>

using namespace std;


namespace cpgf {


GTweenable::GTweenable()
	: elapsedTime(0), delayTime(0), elapsedDelayTime(0), repeatDelayTime(0), repeatCount(0), cycleCount(0), timeScaleTime(1.0f), flags()
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

	if(frameDuration > 0) {
		frameDuration *= this->timeScaleTime;
		if(this->elapsedDelayTime > 0) {
			this->elapsedDelayTime -= frameDuration;
			if(this->elapsedDelayTime >= 0) {
				return;
			}
			this->elapsedTime = -this->elapsedDelayTime;
		}
		else {
			this->elapsedTime += frameDuration;
		}
		if(this->repeatCount >= 0) {
			GTweenNumber total = this->getTotalDuration();
			if(this->elapsedTime > total) {
				this->elapsedTime = total;
			}
		}
	}

	if(! this->flags.has(tfInitialized)) {
		this->flags.set(tfInitialized);

		if(this->callbackOnInitialize) {
			this->callbackOnInitialize();
		}

		this->initialize();
	}

	this->performTime(frameDuration, forceReversed, forceUseFrames);
}

void GTweenable::doComplete(bool emitEvent)
{
	if(this->isCompleted()) {
		return;
	}

	this->flags.set(tfCompleted);
	this->elapsedTime = this->getTotalDuration();

	if(emitEvent && this->callbackOnComplete) {
		this->callbackOnComplete();
	}
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
		return this->getDuration() + this->repeatDelayTime;
	}
}

GTweenNumber GTweenable::getCurrentTime() const
{
	if(this->isRepeat()) {
		GTweenNumber cycleDuration = this->getDuration() + this->repeatDelayTime;
		return this->elapsedTime - cycleDuration * this->cycleCount;
	}
	else {
		return this->elapsedTime;
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
	if(this->isRepeat()) {
		value += (d + this->repeatDelayTime) * this->cycleCount;
	}
	this->elapsedTime = min(value, this->getTotalDuration());
}

GTweenNumber GTweenable::getTotalTime() const
{
	return this->elapsedTime;
}

void GTweenable::setTotalTime(GTweenNumber value)
{
	if(value < 0.0f) {
		value = 0.0f;
	}
	this->elapsedTime = min(value, this->getTotalDuration());
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
	this->elapsedDelayTime = 0;
	this->elapsedTime = 0;
	this->cycleCount = 0;
	this->flags.clear(tfCompleted);
}

void GTweenable::restartWithDelay()
{
	this->restart();
	this->elapsedDelayTime = this->delayTime;
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
	this->elapsedDelayTime = value;
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

