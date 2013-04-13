#include "cpgf/tween/gtween.h"
#include "cpgf/tween/easing/linear.h"

namespace cpgf {


namespace tween_internal {

void GTweenItemBase::deleteSelf()
{
	this->virtualFunctions->deleteSelf(this);
}

void GTweenItemBase::tick(GTweenEaseParam * param, const GTweenEaseType & ease)
{
	this->virtualFunctions->tick(this, param, ease);
}

void GTweenItemBase::init()
{
	this->virtualFunctions->init(this);
}

void GTweenItemBase::reverse()
{
	this->virtualFunctions->reverse(this);
}

void GTweenItemBase::rewind()
{
	this->virtualFunctions->rewind(this);
}

const void * GTweenItemBase::getInstance()
{
	return this->virtualFunctions->getInstance(this);
}

} // namespace tween_internal

	
GTween::GTween()
	: easeCallback(LinearEase::ease()), current(0), total(0), delayTime(0), repeatDelayTime(0), repeatCount(0), cycleCount(0), flags()
{
}

GTween::~GTween()
{
	for(ListType::iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
		(*it)->deleteSelf();
	}
}

void GTween::tick(GTweenNumber frameTime)
{
	if(this->isCompleted()) {
		return;
	}

	if(this->flags.has(tfPaused)) {
		return;
	}

	this->current += frameTime;
	if(this->current <= this->delayTime) {
		return;
	}

	if(! this->flags.has(tfInited)) {
		this->flags.set(tfInited);
		this->init();
	}

	bool shouldFinish = false;
	bool shouldReverse = false;
	bool shouldSetValue = true;
	GTweenNumber t = this->current - this->delayTime;

	if(this->repeatCount == 0) {
		if(t > this->total) {
			shouldFinish = true;
			t = this->total;
			this->current = t + this->delayTime;
		}
	}
	else {
		GTweenNumber cycleExtra = 0.0f;
		if(this->cycleCount >= 0) {
			cycleExtra = this->repeatDelayTime;
		}
		GTweenNumber cycleDuration = this->total + cycleExtra;
		int times = (int)(t / cycleDuration);
		int ctiimes = times;
		GTweenNumber remains = t - times * cycleDuration;
		if(remains > this->total) {
			return;
		}
		if(remains <= 0) {
			--times;
			t = this->total;
		}
		else {
			t = remains;
		}
		if(times > this->cycleCount) {
			this->cycleCount = times;
			if(this->repeatCount < 0) {
			}
			else {
				if(ctiimes > this->repeatCount) {
					shouldFinish = true;
					shouldSetValue = false;
				}
			}
			if(this->flags.has(tfReverseWhenRepeat)) {
				shouldReverse = true;
			}
		}
	}
	if(shouldReverse) {
		this->flags.toggle(tfBackward);
	}
	
	if(this->flags.has(tfBackward)) {
		t = this->total - t;
	}
	
	if(shouldSetValue) {
		GTweenEaseParam param;
		param.current = t;
		param.total = this->total;
		for(ListType::iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
			(*it)->tick(&param, this->easeCallback);
		}
	}
	
	if(shouldFinish) {
		this->flags.set(tfCompleted);
		if(this->callbackOnComplete) {
			this->callbackOnComplete();
		}
	}
}

void GTween::reverseAll()
{
	for(ListType::iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
		(*it)->reverse();
	}
}

void GTween::rewindAll()
{
	for(ListType::iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
		(*it)->rewind();
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

GTween & GTween::duration(GTweenNumber total)
{
	this->total = total;
	return *this;
}

GTween & GTween::backward(bool value)
{
	this->flags.setByBool(tfBackward, value);
	return *this;
}

GTween & GTween::useFrames(bool value)
{
	this->flags.setByBool(tfUseFrames, value);
	return *this;
}

GTween & GTween::delay(GTweenNumber d)
{
	this->delayTime = d;
	return *this;
}

GTween & GTween::repeat(int repeatCount)
{
	this->repeatCount = repeatCount;
	return *this;
}

GTween & GTween::repeatDelay(GTweenNumber d)
{
	this->repeatDelayTime = d;
	return *this;
}

GTween & GTween::yoyo(bool value)
{
	this->flags.setByBool(tfReverseWhenRepeat, value);
	return *this;
}

GTween & GTween::onComplete(const GTweenCallback & value)
{
	this->callbackOnComplete = value;
	return *this;
}

void GTween::pause()
{
	this->flags.set(tfPaused);
}

void GTween::resume()
{
	this->flags.clear(tfPaused);
}

void GTween::removeOf(const void * instance)
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
		this->flags.set(tfCompleted);
	}
}

void GTween::init()
{
	for(ListType::iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
		(*it)->init();
	}
}


} // namespace cpgf

