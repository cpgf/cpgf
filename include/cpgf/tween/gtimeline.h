#ifndef __GTIMELINE_H
#define __GTIMELINE_H

#include "cpgf/tween/gtweenlist.h"

#include <algorithm>

namespace cpgf {


class GTimeline : public GTweenList
{
private:
	typedef GTweenList super;

public:
	GTimeline();
	~GTimeline();

public:
	virtual GTweenNumber getDuration() const;
	virtual bool removeOf(const void * instance);
	
	GTweenNumber append(const GTweenable & tweenable);
	void prepend(const GTweenable & tweenable);
	void insert(GTweenNumber time, const GTweenable & tweenable);
	void setAt(GTweenNumber time, const GTweenable & tweenable);

public:
	GTimeline & backward(bool value);
	GTimeline & useFrames(bool value);
	GTimeline & delay(GTweenNumber value);
	GTimeline & timeScale(GTweenNumber value);
	GTimeline & immediateTick();

	GTimeline & repeat(int repeatCount);
	GTimeline & repeatDelay(GTweenNumber value);
	GTimeline & yoyo(bool value);

	GTimeline & onComplete(const GTweenCallback & value);
	GTimeline & onDestroy(const GTweenCallback & value);
	GTimeline & onUpdate(const GTweenCallback & value);
	GTimeline & onCycleComplete(const GTweenCallback & value);

protected:
	virtual void performTime(GTweenNumber frameTime, bool forceReversed, bool forceUseFrames);

private:
	void invalidDurationTime();

private:
	mutable GTweenNumber durationTime;
};


} // namespace cpgf



#endif
