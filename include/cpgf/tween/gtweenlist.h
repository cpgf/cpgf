#ifndef CPGF_GTWEENLIST_H
#define CPGF_GTWEENLIST_H

#include "cpgf/tween/gtween.h"
#include "cpgf/gmemorypool.h"
#include "cpgf/gscopedptr.h"

#include <list>


namespace cpgf {


class GTimeline;

class GTweenList : public GTweenable
{
private:
	typedef GTweenable super;

protected:
	class TweenableData
	{
	public:
		explicit TweenableData(bool isTimeline);
		bool isTimeline() const;
		bool hasAddedToTimeline() const;
		void addToTimeline();
	public:
		GTweenNumber startTime;
		GTweenable * tweenable;
	private:
		GFlags<int> flags;
	};

	typedef std::list<TweenableData> ListType;


public:
	static GTweenList * getInstance();

public:
	GTweenList();
	~GTweenList();

	GTween & tween();
	GTimeline & timeline();

	size_t getTweenableCount() const;
	void clear();

public:
	void remove(const GTweenable & tweenable);
	virtual GTweenNumber getDuration() const;
	virtual void removeForInstance(const void * instance);

protected:
	virtual void performTime(GTweenNumber elapsed, GTweenNumber frameDuration, bool forceReversed, bool forceUseFrames);
	virtual void doRestartChildren();
	virtual void doRestartChildrenWithDelay();

protected:
	void freeTween(GTweenable * tween, bool isTimeline);

protected:
	GTweenList::ListType tweenList;
	GObjectPool<GTween> tweenPool;
	GScopedPointer<GObjectPool<GTimeline> > timelinePool;
};


} // namespace cpgf


#endif
