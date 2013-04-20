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

GTween & GTweenList::createTween()
{
	GTween * tweenable = this->tweenPool.allocate();
	TweenableData data(false);
	data.startTime = 0;
	data.tweenable = tweenable;
	tweenable->useFrames(this->isUseFrames());
	this->tweenList.push_back(data);
	return *tweenable;
}

GTimeline & GTweenList::createTimeline()
{
	if(! this->timelinePool) {
		this->timelinePool.reset(new GObjectPool<GTimeline>());
	}
	GTimeline * timeline = this->timelinePool->allocate();
	TweenableData data(true);
	data.startTime = 0;
	data.tweenable = timeline;
	timeline->useFrames(this->isUseFrames());
	this->tweenList.push_back(data);
	return *timeline;
}

GTween & GTweenList::to(GTweenNumber duration)
{
	GTween & tweenable = this->createTween();
	tweenable.duration(duration);
	return tweenable;
}

GTween & GTweenList::from(GTweenNumber duration)
{
	GTween & tweenable = this->createTween();
	tweenable.duration(duration).backward(true);
	return tweenable;
}

void GTweenList::performTime(GTweenNumber frameTime, bool /*forceReversed*/)
{
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end();) {
		if(it->tweenable->tick(frameTime)) {
			this->freeTween(it->tweenable, it->isTimeline());
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
		if(it->tweenable->removeOf(instance)) {
			this->freeTween(it->tweenable, it->isTimeline());
			it = this->tweenList.erase(it);
		}
		else {
			++it;
		}
	}

	return false;
}

GTweenNumber GTweenList::getDuration()
{
	return 0;
}

void GTweenList::restart()
{
	super::restart();

	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
		it->tweenable->restart();
	}
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

