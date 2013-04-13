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
	this->tweenList.push_back(tween);
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

void GTweenList::tick(GTweenNumber frameTime)
{
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end();) {
		if((*it)->isUseFrames()) {
			(*it)->tick(1);
		}
		else {
			(*it)->tick(frameTime);
		}
		if((*it)->isCompleted()) {
			this->tweenPool.free(*it);
			it = this->tweenList.erase(it);
		}
		else {
			++it;
		}
	}
}

void GTweenList::removeTweenOf(const void * instance)
{
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end();) {
		(*it)->removeOf(instance);
		if((*it)->isCompleted()) {
			this->tweenPool.free(*it);
			it = this->tweenList.erase(it);
		}
		else {
			++it;
		}
	}
}

void GTweenList::clear()
{
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
		this->tweenPool.free(*it);
	}
	this->tweenList.clear();
}


} // namespace cpgf

