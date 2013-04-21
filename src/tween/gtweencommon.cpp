#include "cpgf/tween/gtweencommon.h"

namespace cpgf {


GTweenable::GTweenable()
	: currentTime(0), delayTime(0), repeatDelayTime(0), repeatCount(0), cycleCount(0), timeScaleTime(1.0f), flags()
{
}

GTweenable::~GTweenable()
{
	if(this->callbackOnDestroy) {
		this->callbackOnDestroy();
	}
}

bool GTweenable::doTick(GTweenNumber frameTime, bool forceReversed, bool forceUseFrames)
{
	if(this->isCompleted()) {
		return true;
	}

	if(this->isPaused()) {
		return false;
	}

	if(frameTime > 0) {
		frameTime *= this->timeScaleTime;
		this->currentTime += (forceUseFrames || this->flags.has(tfUseFrames)) ? this->timeScaleTime : frameTime;

		if(this->currentTime <= 0) {
			return false;
		}
	}

	this->performTime(frameTime, forceReversed, forceUseFrames);

	return this->isCompleted();
}

GTweenNumber GTweenable::getTotalDuration()
{
	if(this->repeatCount >= 0) {
		return (this->getDuration() + this->repeatDelayTime) * (this->repeatCount + 1);
	}
	else {
		return this->getDuration() + this->repeatDelayTime;
	}
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
	this->currentTime = 0;
	this->cycleCount = 0;
	this->flags.clear(tfCompleted);
}

void GTweenable::reset()
{
	this->currentTime = -this->delayTime;
	this->cycleCount = 0;
	this->flags.clear(tfCompleted);
}

void GTweenable::doImmediateTick(bool forceReversed)
{
	bool paused = this->isPaused();
	this->flags.clear(tfPaused);

	this->doTick(0, forceReversed, false);

	this->flags.setByBool(tfPaused, paused);
}

void GTweenable::doComplete(bool emitEvent)
{
	this->flags.set(tfCompleted);
	if(emitEvent && this->callbackOnComplete) {
		this->callbackOnComplete();
	}
}

void GTweenable::setOnComplete(const GTweenCallback & value)
{
	this->callbackOnComplete = value;
}

void GTweenable::setOnDestroy(const GTweenCallback & value)
{
	this->callbackOnDestroy = value;
}

void GTweenable::setBackward(bool value)
{
	this->flags.setByBool(tfBackward, value);
}

void GTweenable::setUseFrames(bool value)
{
	this->flags.setByBool(tfUseFrames, value);
}

void GTweenable::setDelay(GTweenNumber value)
{
	if(this->delayTime != value) {
		this->delayTime = value;
		this->currentTime = -value;
	}
}

void GTweenable::setRepeatCount(int repeatCount)
{
	this->repeatCount = repeatCount;
}

void GTweenable::setRepeatDelay(GTweenNumber value)
{
	this->repeatDelayTime = value;
}

void GTweenable::setYoyo(bool value)
{
	this->flags.setByBool(tfReverseWhenRepeat, value);
}

void GTweenable::setTimeScale(GTweenNumber value)
{
	this->timeScaleTime = value;
}


} // namespace cpgf

