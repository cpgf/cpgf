#include "cpgf/tween/gtweenlist.h"
#include "cpgf/tween/gtimeline.h"
#include "cpgf/gscopedptr.h"


namespace cpgf {

const int TweenDataFlag_isTimeline = 1 << 0;
const int TweenDataFlag_hasAddedToTimeline = 1 << 1;

GTweenList::TweenableData::TweenableData(bool isTimeline)
{
	this->flags.setByBool(TweenDataFlag_isTimeline, isTimeline);
}

bool GTweenList::TweenableData::isTimeline() const
{
	return this->flags.has(TweenDataFlag_isTimeline);
}

bool GTweenList::TweenableData::hasAddedToTimeline() const
{
	return this->flags.has(TweenDataFlag_hasAddedToTimeline);
}

void GTweenList::TweenableData::addToTimeline()
{
	this->flags.set(TweenDataFlag_hasAddedToTimeline);
}


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

GTween & GTweenList::tween()
{
	GTween * tweenable = this->tweenPool.allocate();
	TweenableData data(false);
	data.startTime = 0;
	data.tweenable = tweenable;
	tweenable->useFrames(this->isUseFrames());
	this->tweenList.push_back(data);
	return *tweenable;
}

GTimeline & GTweenList::timeline()
{
	if(! this->timelinePool) {
		this->timelinePool.reset(new GObjectPool<GTimeline>());
	}
	GTimeline * newTimeline = this->timelinePool->allocate();
	TweenableData data(true);
	data.startTime = 0;
	data.tweenable = newTimeline;
	newTimeline->useFrames(this->isUseFrames());
	this->tweenList.push_back(data);
	return *newTimeline;
}

void GTweenList::performTime(GTweenNumber /*elapsed*/, GTweenNumber frameDuration, bool /*forceReversed*/, bool /*forceUseFrames*/)
{
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end();) {
		it->tweenable->tick(frameDuration);
		if(it->tweenable->isCompleted()) {
			this->freeTween(it->tweenable, it->isTimeline());
			it = this->tweenList.erase(it);
		}
		else {
			++it;
		}
	}
}

void GTweenList::remove(const GTweenable & tweenable)
{
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end();) {
		if(it->tweenable == &tweenable) {
			this->freeTween(it->tweenable, it->isTimeline());
			it = this->tweenList.erase(it);
		}
		else {
			++it;
		}
	}
}

GTweenNumber GTweenList::getDuration() const
{
	return 0;
}

void GTweenList::removeForInstance(const void * instance)
{
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end();) {
		it->tweenable->removeForInstance(instance);
		if(it->tweenable->isCompleted()) {
			this->freeTween(it->tweenable, it->isTimeline());
			it = this->tweenList.erase(it);
		}
		else {
			++it;
		}
	}
}

void GTweenList::doRestartChildren()
{
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
		it->tweenable->restart();
	}
}

void GTweenList::doRestartChildrenWithDelay()
{
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
		it->tweenable->restartWithDelay();
	}
}

size_t GTweenList::getTweenableCount() const
{
	return this->tweenList.size();
}

void GTweenList::clear()
{
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
		this->freeTween(it->tweenable, it->isTimeline());
	}
	this->tweenList.clear();
}

void GTweenList::freeTween(GTweenable * tweenable, bool isTimeline)
{
	if(isTimeline) {
		this->timelinePool->free(static_cast<GTimeline *>(tweenable));
	}
	else {
		this->tweenPool.free(static_cast<GTween *>(tweenable));
	}
}


} // namespace cpgf

