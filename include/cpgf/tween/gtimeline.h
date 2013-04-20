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
	virtual GTweenNumber getDuration();
	
	GTweenNumber append(const GTweenable & tweenable);
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

protected:
	virtual void performTime(GTweenNumber frameTime, bool forceReversed);

private:
	void invalidDurationTime();

private:
	GTweenNumber durationTime;
};


} // namespace cpgf



#endif
