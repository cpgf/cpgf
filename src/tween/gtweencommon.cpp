#include "cpgf/tween/gtweencommon.h"

namespace cpgf {


GTweenable::GTweenable()
	: currentTime(0), delayTime(0), repeatDelayTime(0), repeatCount(0), cycleCount(0), timeScaleTime(1.0f), flags()
{
}

GTweenable::~GTweenable()
{
}

bool GTweenable::doTick(GTweenNumber frameTime, bool forceReversed)
{
	if(this->isCompleted()) {
		return true;
	}

	if(this->isPaused()) {
		return false;
	}

	if(frameTime > 0) {
		this->currentTime += (this->flags.has(tfUseFrames) ? 1.0f : frameTime) * this->timeScaleTime;

		if(this->currentTime <= 0) {
			return false;
		}
	}

	this->performTime(frameTime, forceReversed);

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
	this->repeatCount = 0;
	this->cycleCount = 0;
	this->flags.clear(tfCompleted);
}

void GTweenable::doImmediateTick()
{
	this->tick(0);
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
	this->delayTime = value;
	this->currentTime = -value;
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

