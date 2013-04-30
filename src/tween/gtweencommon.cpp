#include "cpgf/tween/gtweencommon.h"

namespace cpgf {


GTweenable::GTweenable()
	: currentTime(0), delayTime(0), currentDelayTime(0), repeatDelayTime(0), repeatCount(0), cycleCount(0), timeScaleTime(1.0f), flags()
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
		if(this->currentDelayTime > 0) {
			this->currentDelayTime -= frameDuration;
			if(this->currentDelayTime >= 0) {
				return;
			}
			this->currentTime = -this->currentDelayTime;
		}
		else {
			this->currentTime += frameDuration;
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
	this->flags.set(tfCompleted);
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
	this->currentDelayTime = 0;
	this->currentTime = 0;
	this->cycleCount = 0;
	this->flags.clear(tfCompleted);
}

void GTweenable::restartWithDelay()
{
	this->restart();
	this->currentDelayTime = this->delayTime;
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
	this->currentDelayTime = value;
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

