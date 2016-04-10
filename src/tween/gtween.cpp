#include "cpgf/tween/gtween.h"
#include "cpgf/tween/easing/linear.h"

#include <cmath>

namespace cpgf {


namespace tween_internal {

void GTweenItem::deleteSelf()
{
	this->virtualFunctions->deleteSelf(this);
}

void GTweenItem::tick(GTweenEaseParam * param, const GTweenEaseType & ease)
{
	this->virtualFunctions->tick(this, param, ease);
}

void GTweenItem::init()
{
	this->virtualFunctions->init(this);
}

const void * GTweenItem::getInstance()
{
	return this->virtualFunctions->getInstance(this);
}

} // namespace tween_internal

	
GTween::GTween()
	: easeCallback(LinearEase::ease()), durationTime(0)
{
}

GTween::~GTween()
{
	for(ListType::iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
		(*it)->deleteSelf();
	}
}

GTweenNumber GTween::getDuration() const
{
	return this->durationTime;
}

void GTween::removeForInstance(const void * instance)
{
	for(ListType::iterator it = this->itemList.begin(); it != this->itemList.end();) {
		if((*it)->getInstance() == instance) {
			(*it)->deleteSelf();
			it = this->itemList.erase(it);
		}
		else {
			++it;
		}
	}
	if(this->itemList.empty()) {
		this->doComplete(false);
	}
}

void GTween::performTime(GTweenNumber elapsed, GTweenNumber /*frameDuration*/, bool forceReversed, bool /*forceUseFrames*/)
{
	bool shouldFinish = false;
	bool shouldSetValue = true;
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
			GTweenNumber remains = t - times * cycleDuration;
			if(remains > this->durationTime) {
				t = this->durationTime;
			}
			else {
				if(remains <= 0) {
					--times;
					t = this->durationTime;
				}
				else {
					t = remains;
				}
				if(times > this->cycleCount) {
					this->cycleCount = times;
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

	if(forceReversed || this->isBackward()) {
		t = this->durationTime - t;
	}

	if(shouldSetValue && t != this->previousAppliedTime) {
		this->previousAppliedTime = t;

		GTweenEaseParam param;
		param.current = t;
		param.total = this->durationTime;
		if(this->durationTime == 0) {
			param.current = 1.0f;
			param.total = 1.0f;
		}
		for(ListType::iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
			(*it)->tick(&param, this->easeCallback);
		}
		
		if(this->callbackOnUpdate) {
			this->callbackOnUpdate();
		}
	}
	
	if(shouldFinish) {
		this->doComplete(true);
	}
}

GTween & GTween::ease(const GTweenEaseType & ease)
{
	this->easeCallback = ease;
	if(this->easeCallback.empty()) {
		this->easeCallback = LinearEase::ease();
	}
	return *this;
}

GTween & GTween::duration(GTweenNumber durationTime)
{
	this->durationTime = durationTime;
	return *this;
}

void GTween::initialize()
{
	for(ListType::iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
		(*it)->init();
	}
}


} // namespace cpgf

