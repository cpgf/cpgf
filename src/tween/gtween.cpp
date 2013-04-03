#include "cpgf/tween/gtween.h"

namespace cpgf {

namespace {

GTweenNumber defaultEase(const GTweenEaseParam * param)
{
	return param->current / param->total;
}

} // unnamed namespace

namespace tween_internal {

void GTweenItemBase::deleteSelf()
{
	this->virtualFunctions->deleteSelf(this);
}

void GTweenItemBase::tick(GTweenEaseParam * param, const GTweenEaseType & ease)
{
	this->virtualFunctions->tick(this, param, ease);
}

void GTweenItemBase::reverse()
{
	this->virtualFunctions->reverse(this);
}

const void * GTweenItemBase::getInstance()
{
	return this->virtualFunctions->getInstance(this);
}

} // namespace tween_internal

	
GTween::GTween()
	: easeCallback(&defaultEase), current(0), total(0), delayTime(0), repeatDelayTime(0), repeatCount(0), flags()
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

	if(this->flags.has(tfUseFrames)) {
		frameTime = 1;
	}

	if(this->delayTime > 0) {
		if(this->delayTime > frameTime) {
			this->delayTime -= frameTime;
			return;
		}
		else {
			frameTime -= this->delayTime;
			this->delayTime = 0;
			if(frameTime <= 0) {
				return;
			}
		}
	}

	if(! this->flags.has(tfInited)) {
		this->flags.set(tfInited);
		this->init();
	}

	if(this->flags.has(tfWaitForStart)) {
		this->flags.clear(tfWaitForStart);
		return;
	}

	bool finished = false;
	this->current += frameTime;
	if(this->current >= this->total) {
		this->current = this->total;
		finished = true;
	}

	GTweenEaseParam param;
	param.current = this->current;
	param.total = this->total;
	for(ListType::iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
		(*it)->tick(&param, this->easeCallback);
	}

	if(finished) {
		if(this->repeatCount >= 0) {
			--this->repeatCount;
			if(this->repeatCount <= 0) {
				this->repeatCount = 0;
				this->flags.set(tfCompleted);
			}
			else {
				this->current = 0;
				if(this->flags.has(tfReverseWhenRepeat)) {
					this->reverseAll();
				}
				if(this->repeatDelayTime > 0) {
					this->delayTime = this->repeatDelayTime;
				}
				if(! this->flags.has(tfImmediateTick)) {
					this->flags.set(tfWaitForStart);
				}
			}
		}
	}

	if(this->isCompleted()) {
	}
}

void GTween::init()
{
	if(! this->flags.has(tfImmediateTick)) {
		this->flags.set(tfWaitForStart);
	}

	if(this->flags.has(tfBackward)) {
		this->reverseAll();
	}
}

void GTween::reverseAll()
{
	for(ListType::iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
		(*it)->reverse();
	}
}


GTween & GTween::ease(const GTweenEaseType & ease)
{
	this->easeCallback = ease;
	if(this->easeCallback.empty()) {
		this->easeCallback = &defaultEase;
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

GTween & GTween::immediateTick(bool value)
{
	this->flags.setByBool(tfImmediateTick, value);
	return *this;
}

GTween & GTween::repeat(int repeatCount)
{
	this->repeatCount = repeatCount;
	return *this;
}

GTween & GTween::yoyo(bool value)
{
	this->flags.setByBool(tfReverseWhenRepeat, value);
	return *this;
}

GTween & GTween::repeatDelay(GTweenNumber d)
{
	this->repeatDelayTime = d;
	return *this;
}

GTween & GTween::immediateYoyo(bool value)
{
	this->flags.setByBool(tfImmediateYoyo, value);
	return *this;
}

bool GTween::isCompleted() const
{
	return this->flags.has(tfCompleted);
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


} // namespace cpgf

