#ifndef __GTWEENLIST_H
#define __GTWEENLIST_H


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

	GTween & createTween();
	GTimeline & createTimeline();

	GTween & to(GTweenNumber duration);
	GTween & from(GTweenNumber duration);

	template <typename AccessorType>
	GTween & to(GTweenNumber duration, const AccessorType & accessor, const typename AccessorType::ValueType & target) {
		return this->to(duration).target(accessor, target);
	}

	template <typename AccessorType>
	GTween & from(GTweenNumber duration, const AccessorType & accessor, const typename AccessorType::ValueType & target) {
		return this->from(duration).target(accessor, target);
	}

	size_t getTweenableCount() const;
	void clear();

public:
	void remove(const GTweenable & tweenable);
	virtual GTweenNumber getDuration() const;
	virtual void removeForInstance(const void * instance);
	virtual void restart();
	virtual void restartWithDelay();

protected:
	virtual void performTime(GTweenNumber frameTime, bool forceReversed, bool forceUseFrames);

protected:
	void freeTween(GTweenable * tween, bool isTimeline);

protected:
	GTweenList::ListType tweenList;
	GObjectPool<GTween> tweenPool;
	GScopedPointer<GObjectPool<GTimeline> > timelinePool;
};


} // namespace cpgf


#endif
