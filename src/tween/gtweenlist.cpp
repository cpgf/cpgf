#include "cpgf/tween/gtweenlist.h"
#include "cpgf/gscopedptr.h"


namespace cpgf {


GTweenList * GTweenList::getInstance()
{
	static GScopedPointer<GTweenList> instance;

	if(! instance) {
		instance.reset(new GTweenList);
	}

	return instance.get();
}

GTweenList::GTweenList()
{
}

GTweenList::~GTweenList()
{
	this->clear();
}

GTween & GTweenList::createTween()
{
	GTween * tween = this->tweenPool.allocate();
	TweenData data;
	data.startTime = 0;
	data.tween = tween;
	tween->useFrames(this->isUseFrames());
	this->tweenList.push_back(data);
	return *tween;
}

GTween & GTweenList::to(GTweenNumber duration)
{
	GTween & tween = this->createTween();
	tween.duration(duration);
	return tween;
}

GTween & GTweenList::from(GTweenNumber duration)
{
	GTween & tween = this->createTween();
	tween.duration(duration).backward(true);
	return tween;
}

void GTweenList::performTime(GTweenNumber frameTime)
{
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end();) {
		if(this->current >= it->startTime && it->tween->tick(frameTime)) {
			this->freeTween(it->tween);
			it = this->tweenList.erase(it);
		}
		else {
			++it;
		}
	}
}

bool GTweenList::removeOf(const void * instance)
{
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end();) {
		if(it->tween->removeOf(instance)) {
			this->freeTween(it->tween);
			it = this->tweenList.erase(it);
		}
		else {
			++it;
		}
	}

	return false;
}

void GTweenList::clear()
{
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
		this->freeTween(it->tween);
	}
	this->tweenList.clear();
}

void GTweenList::freeTween(GTweenable * tween)
{
	this->tweenPool.free(static_cast<GTween *>(tween));
}


} // namespace cpgf

