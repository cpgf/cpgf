#include "cpgf/tween/gtimeline.h"
#include "cpgf/gexception.h"
#include "cpgf/gerrorcode.h"

#include <cmath>


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

void GTimeline::removeForInstance(const void * instance)
{
	super::removeForInstance(instance);

	if(this->getTweenableCount() == 0) {
		this->doComplete(false);
	}
}

GTweenNumber GTimeline::getDuration() const
{
	if(this->durationTime < 0) {
		this->durationTime = 0;
		for(ListType::const_iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
			if(it->hasAddedToTimeline()) {
				GTweenNumber t = it->startTime + it->tweenable->getTotalDuration() + it->tweenable->getDelay();
				if(t > this->durationTime) {
					this->durationTime = t;
				}
			}
		}
	}

	return this->durationTime;
}

GTweenNumber GTimeline::append(const GTweenable & tweenable)
{
	GTweenNumber duration = 0;
	TweenableData * data = nullptr;
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
		if(it->tweenable == &tweenable) {
			data = &*it;
		}
		else {
			if(it->hasAddedToTimeline()) {
				GTweenNumber t = it->startTime + it->tweenable->getTotalDuration() + it->tweenable->getDelay();
				if(t > duration) {
					duration = t;
				}
			}
		}
	}

	if(data == nullptr) {
		raiseCoreException(Error_Tween_TweenableNotOwnedByTimeline);
	}

	data->startTime = duration;
	data->addToTimeline();

	this->invalidDurationTime();

	return duration;
}

void GTimeline::prepend(const GTweenable & tweenable)
{
	TweenableData * data = nullptr;
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
		if(it->tweenable == &tweenable) {
			data = &*it;
			break;
		}
	}

	if(data == nullptr) {
		raiseCoreException(Error_Tween_TweenableNotOwnedByTimeline);
	}

	GTweenNumber duration = tweenable.getTotalDuration() + tweenable.getDelay();
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
	TweenableData * data = nullptr;
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
		if(it->tweenable == &tweenable) {
			data = &*it;
			break;
		}
	}

	if(data == nullptr) {
		raiseCoreException(Error_Tween_TweenableNotOwnedByTimeline);
	}

	GTweenNumber minStartTime = -1.0f;
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
		if(it->tweenable != &tweenable && it->hasAddedToTimeline()) {
			if(it->startTime >= time) {
				if(minStartTime < 0 || it->startTime < minStartTime) {
					minStartTime = it->startTime;
				}
			}
		}
	}
	if(minStartTime >= 0) {
		GTweenNumber duration = tweenable.getTotalDuration() + tweenable.getDelay();
		GTweenNumber deltaTime = duration - (minStartTime - time);
		for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
			if(it->tweenable != &tweenable && it->hasAddedToTimeline()) {
				if(it->startTime >= time) {
					it->startTime += deltaTime;
				}
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

GTweenNumber GTimeline::getStartTime(const GTweenable & tweenable)
{
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
		if(it->tweenable == &tweenable) {
			return it->startTime;
		}
	}

	raiseCoreException(Error_Tween_TweenableNotOwnedByTimeline);
	
	return 0;
}

void GTimeline::performTime(GTweenNumber elapsed, GTweenNumber frameDuration, bool forceReversed, bool forceUseFrames)
{
	this->getDuration();

	bool shouldFinish = false;
	bool shouldSetValue = true;
	bool shouldRestart = false;
	GTweenNumber t = elapsed;

	if(this->repeatCount == 0) {
		if(t >= this->durationTime) {
			shouldFinish = true;
			t = this->durationTime;
		}
	}
	else {
		GTweenNumber cycleDuration = this->durationTime + this->repeatDelayTime;
		if(cycleDuration > 0) {
			int times = (int)(floor(t / cycleDuration));
			int ctimes = times;
			t -= times * cycleDuration;
			if(t > this->durationTime) {
				t = this->durationTime;
			}
			else {
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
					if(this->isYoyo()) {
						this->toggleBackward();
					}
					
					if(this->callbackOnRepeat) {
						this->callbackOnRepeat();
					}
				}
			}
		}
		else {
			shouldSetValue = false;
			shouldFinish = true;
		}
	}

	bool reversed = forceReversed || this->isBackward();
	if(reversed) {
		t = this->durationTime - t;
	}

	if(shouldSetValue && t != this->previousAppliedTime) {
		this->previousAppliedTime = t;

		bool useFrames = forceUseFrames || this->isUseFrames();
		if(useFrames && frameDuration > 0) {
			frameDuration = 1.0f;
		}
		for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
			if(shouldRestart) {
				it->tweenable->restart();
			}
			if(frameDuration == 0
				|| (!reversed && t >= it->startTime)
				|| (reversed && t >= it->startTime && t <= it->startTime + it->tweenable->getTotalDuration() + it->tweenable->getDelay())
			) {
				if(reversed) {
					it->tweenable->elapsedTime = (it->startTime + it->tweenable->getTotalDuration() + it->tweenable->getDelay() - t) - frameDuration;
				}
				else {
					it->tweenable->elapsedTime = t - it->startTime - frameDuration;
				}
				it->tweenable->doTick(frameDuration, reversed, useFrames);
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
