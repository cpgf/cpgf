#include "cpgf/tween/gtween.h"
#include "cpgf/tween/easing/linear.h"

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

bool GTween::removeOf(const void * instance)
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

	return this->isCompleted();
}

GTweenNumber GTween::getDuration()
{
	return this->durationTime;
}

void GTween::performTime(GTweenNumber /*frameTime*/, bool forceReversed, bool /*forceUseFrames*/)
{
	if(! this->flags.has(tfInited)) {
		this->flags.set(tfInited);
		this->init();
	}

	bool shouldFinish = false;
	bool shouldSetValue = true;
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
		}
	}

	if(forceReversed || this->flags.has(tfBackward)) {
		t = this->durationTime - t;
	}

	if(shouldSetValue) {
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

GTween & GTween::backward(bool value)
{
	this->setBackward(value);
	return *this;
}

GTween & GTween::useFrames(bool value)
{
	this->setUseFrames(value);
	return *this;
}

GTween & GTween::delay(GTweenNumber value)
{
	this->setDelay(value);
	return *this;
}

GTween & GTween::timeScale(GTweenNumber value)
{
	this->setTimeScale(value);
	return *this;
}

GTween & GTween::immediateTick()
{
	this->doImmediateTick(this->isBackward());
	return *this;
}

GTween & GTween::repeat(int repeatCount)
{
	this->setRepeatCount(repeatCount);
	return *this;
}

GTween & GTween::repeatDelay(GTweenNumber value)
{
	this->setRepeatDelay(value);
	return *this;
}

GTween & GTween::yoyo(bool value)
{
	this->setYoyo(value);
	return *this;
}

GTween & GTween::onComplete(const GTweenCallback & value)
{
	this->setOnComplete(value);
	return *this;
}

GTween & GTween::onDestroy(const GTweenCallback & value)
{
	this->setOnDestroy(value);
	return *this;
}

void GTween::init()
{
	for(ListType::iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
		(*it)->init();
	}
}


} // namespace cpgf

