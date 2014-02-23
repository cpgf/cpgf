#ifndef CPGF_GTIMELINE_H
#define CPGF_GTIMELINE_H

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
	virtual void removeForInstance(const void * instance);
	virtual GTweenNumber getDuration() const;
	
	GTweenNumber append(const GTweenable & tweenable);
	void prepend(const GTweenable & tweenable);
	void insert(GTweenNumber time, const GTweenable & tweenable);
	void setAt(GTweenNumber time, const GTweenable & tweenable);
	
	GTweenNumber getStartTime(const GTweenable & tweenable);

public:
	GTimeline & backward(bool value) { return static_cast<GTimeline &>(super::backward(value)); }
	GTimeline & useFrames(bool value) { return static_cast<GTimeline &>(super::useFrames(value)); }
	GTimeline & delay(GTweenNumber value) { return static_cast<GTimeline &>(super::delay(value)); }
	GTimeline & timeScale(GTweenNumber value) { return static_cast<GTimeline &>(super::timeScale(value)); }

	GTimeline & repeat(int repeatCount) { return static_cast<GTimeline &>(super::repeat(repeatCount)); }
	GTimeline & repeatDelay(GTweenNumber value) { return static_cast<GTimeline &>(super::repeatDelay(value)); }
	GTimeline & yoyo(bool value) { return static_cast<GTimeline &>(super::yoyo(value)); }

	GTimeline & onComplete(const GTweenCallback & value) { return static_cast<GTimeline &>(super::onComplete(value)); }
	GTimeline & onDestroy(const GTweenCallback & value) { return static_cast<GTimeline &>(super::onDestroy(value)); }
	GTimeline & onUpdate(const GTweenCallback & value) { return static_cast<GTimeline &>(super::onUpdate(value)); }
	GTimeline & onRepeat(const GTweenCallback & value) { return static_cast<GTimeline &>(super::onRepeat(value)); }

protected:
	virtual void performTime(GTweenNumber elapsed, GTweenNumber frameDuration, bool forceReversed, bool forceUseFrames);

private:
	void invalidDurationTime();

private:
	mutable GTweenNumber durationTime;
};


} // namespace cpgf



#endif
