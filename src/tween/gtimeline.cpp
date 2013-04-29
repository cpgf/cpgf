#include "cpgf/tween/gtimeline.h"
#include "cpgf/gexception.h"
#include "cpgf/gerrorcode.h"

namespace cpgf {


GTimeline::GTimeline()
{
	this->invalidDurationTime();
}

GTimeline::~GTimeline()
{
}

void GTimeline::invalidDurationTime()
{
	this->durationTime = -1;
}

GTweenNumber GTimeline::getDuration() const
{
	if(this->durationTime < 0) {
		this->durationTime = 0;
		for(ListType::const_iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
			if(it->hasAddedToTimeline()) {
				GTweenNumber t = it->startTime + it->tweenable->getTotalDuration();
				if(t > this->durationTime) {
					this->durationTime = t;
				}
			}
		}
	}

	return this->durationTime;
}

bool GTimeline::removeOf(const void * instance)
{
	super::removeOf(instance);

	if(this->getTweenableCount() == 0) {
		this->flags.set(tfCompleted);
	}

	return this->isCompleted();
}

GTweenNumber GTimeline::append(const GTweenable & tweenable)
{
	GTweenNumber duration = 0;
	TweenableData * data = NULL;
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
		if(it->tweenable == &tweenable) {
			data = &*it;
		}
		else {
			if(it->hasAddedToTimeline()) {
				GTweenNumber t = it->startTime + it->tweenable->getTotalDuration();
				if(t > duration) {
					duration = t;
				}
			}
		}
	}

	if(data == NULL) {
		raiseCoreException(Error_Tween_TweenableNotOwnedByTimeline);
	}

	data->startTime = duration;
	data->addToTimeline();

	this->invalidDurationTime();

	return duration;
}

void GTimeline::prepend(const GTweenable & tweenable)
{
	TweenableData * data = NULL;
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
		if(it->tweenable == &tweenable) {
			data = &*it;
			break;
		}
	}

	if(data == NULL) {
		raiseCoreException(Error_Tween_TweenableNotOwnedByTimeline);
	}

	GTweenNumber duration = tweenable.getTotalDuration();
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
		if(it->tweenable != &tweenable && it->hasAddedToTimeline()) {
			it->startTime += duration;
		}
	}

	data->startTime = 0;
	data->addToTimeline();

	this->invalidDurationTime();
}

void GTimeline::insert(GTweenNumber time, const GTweenable & tweenable)
{
	TweenableData * data = NULL;
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
		if(it->tweenable == &tweenable) {
			data = &*it;
			break;
		}
	}

	if(data == NULL) {
		raiseCoreException(Error_Tween_TweenableNotOwnedByTimeline);
	}

	GTweenNumber duration = tweenable.getTotalDuration();
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
		if(it->tweenable != &tweenable && it->hasAddedToTimeline()) {
			if(it->startTime >= time) {
				it->startTime += duration;
			}
		}
	}

	data->startTime = time;
	data->addToTimeline();

	this->invalidDurationTime();
}

void GTimeline::setAt(GTweenNumber time, const GTweenable & tweenable)
{
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
		if(it->tweenable == &tweenable) {
			it->startTime = time;
			it->addToTimeline();
			this->invalidDurationTime();
			return;
		}
	}

	raiseCoreException(Error_Tween_TweenableNotOwnedByTimeline);
}

GTimeline & GTimeline::backward(bool value)
{
	this->setBackward(value);
	return *this;
}

GTimeline & GTimeline::useFrames(bool value)
{
	this->setUseFrames(value);
	return *this;
}

GTimeline & GTimeline::delay(GTweenNumber value)
{
	this->setDelay(value);
	return *this;
}

GTimeline & GTimeline::timeScale(GTweenNumber value)
{
	this->setTimeScale(value);
	return *this;
}

GTimeline & GTimeline::immediateTick()
{
	this->doImmediateTick(this->isBackward());
	return *this;
}

GTimeline & GTimeline::repeat(int repeatCount)
{
	this->setRepeatCount(repeatCount);
	return *this;
}

GTimeline & GTimeline::repeatDelay(GTweenNumber value)
{
	this->setRepeatDelay(value);
	return *this;
}

GTimeline & GTimeline::yoyo(bool value)
{
	this->setYoyo(value);
	return *this;
}

GTimeline & GTimeline::onComplete(const GTweenCallback & value)
{
	this->setOnComplete(value);
	return *this;
}

GTimeline & GTimeline::onDestroy(const GTweenCallback & value)
{
	this->setOnDestroy(value);
	return *this;
}

GTimeline & GTimeline::onUpdate(const GTweenCallback & value)
{
	this->setOnUpdate(value);
	return *this;
}

GTimeline & GTimeline::onCycleComplete(const GTweenCallback & value)
{
	this->setOnCycleComplete(value);
	return *this;
}

void GTimeline::performTime(GTweenNumber frameTime, bool forceReversed, bool forceUseFrames)
{
	this->getDuration();

	bool shouldFinish = false;
	bool shouldSetValue = true;
	bool shouldRestart = false;
	GTweenNumber t = this->currentTime;

	if(this->repeatCount == 0) {
		if(t > this->durationTime) {
			shouldFinish = true;
			t = this->durationTime;
			this->currentTime = t;
		}
	}
	else {
		GTweenNumber cycleExtra = this->repeatDelayTime;
		GTweenNumber cycleDuration = this->durationTime + cycleExtra;
		int times = (int)(t / cycleDuration);
		int ctimes = times;
		GTweenNumber remains = t - times * cycleDuration;
		if(remains > this->durationTime) {
			return;
		}
		if(remains <= 0) {
			--times;
			t = this->durationTime;
		}
		else {
			t = remains;
		}
		if(times > this->cycleCount) {
			this->cycleCount = times;
			shouldRestart = true;
			if(this->repeatCount < 0) {
			}
			else {
				if(ctimes > this->repeatCount) {
					shouldFinish = true;
					shouldSetValue = false;
				}
			}
			if(this->flags.has(tfReverseWhenRepeat)) {
				this->flags.toggle(tfBackward);
			}
			
			if(this->callbackOnCycleComplete) {
				this->callbackOnCycleComplete();
			}
		}
	}

	bool reversed = forceReversed || this->flags.has(tfBackward);
	if(reversed) {
		t = this->durationTime - t;
	}

	if(shouldSetValue) {
		for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
			if(shouldRestart) {
				it->tweenable->restart();
			}
			if(frameTime == 0
				|| (!reversed && t >= it->startTime)
				|| (reversed && t < it->startTime + it->tweenable->getTotalDuration())
			) {
				it->tweenable->doTick(frameTime, reversed, forceUseFrames || this->isUseFrames());
			}
		}
		if(shouldRestart) {
			this->immediateTick();
		}
		
		if(this->callbackOnUpdate) {
			this->callbackOnUpdate();
		}
	}
	
	if(shouldFinish) {
		this->doComplete(true);
	}
}


} // namespace cpgf
