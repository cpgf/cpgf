#ifndef __GTWEENCOMMON_H
#define __GTWEENCOMMON_H

#include "cpgf/gcallback.h"
#include "cpgf/gclassutil.h"
#include "cpgf/gflags.h"


namespace cpgf {

typedef float GTweenNumber;

struct GTweenEaseParam
{
	GTweenNumber current;
	GTweenNumber total;
};

typedef GTweenNumber (*GTweenEaseFunction)(const GTweenEaseParam *);
typedef GCallback<GTweenNumber (*)(const GTweenEaseParam *)> GTweenEaseType;

typedef GCallback<void ()> GTweenCallback;

class GTweenable : public GNoncopyable
{
protected:
	enum GTweenFlags {
		tfInited = 1 << 0,
		tfPaused = 1 << 1,
		tfCompleted = 1 << 2,
		tfUseFrames = 1 << 3,
		tfBackward = 1 << 4,
		tfReverseWhenRepeat = 1 << 5,
	};

protected:
	GTweenable();

public:
	virtual ~GTweenable();

	// return true if the tween is completed
	virtual bool removeOf(const void * instance) = 0;

	virtual GTweenNumber getDuration() const = 0;

	// return true if the tween is completed
	bool tick(GTweenNumber frameTime) {
		return this->doTick(this->isUseFrames() ? 1.0f : frameTime, false, false);
	}

	GTweenNumber getTotalDuration() const;

public:
	void pause();
	void resume();
	virtual void restart();
	virtual void reset();

	bool isPaused() {
		return this->flags.has(tfPaused);
	}

	bool isCompleted() const
	{
		return this->flags.has(tfCompleted);
	}

	bool isUseFrames() const
	{
		return this->flags.has(tfUseFrames);
	}

	bool isBackward() const
	{
		return this->flags.has(tfBackward);
	}
	
	GTweenNumber getTimeScale() const {
		return this->timeScaleTime;
	}

protected:
	bool doTick(GTweenNumber frameTime, bool forceReversed, bool forceUseFrames);
	virtual void performTime(GTweenNumber frameTime, bool forceReversed, bool forceUseFrames) = 0;

protected:
	void doImmediateTick(bool forceReversed);
	void doComplete(bool emitEvent);
	void setBackward(bool value);
	void setUseFrames(bool value);
	void setDelay(GTweenNumber value);
	void setRepeatCount(int repeatCount);
	void setRepeatDelay(GTweenNumber value);
	void setYoyo(bool value);
	void setTimeScale(GTweenNumber value);
	
	void setOnComplete(const GTweenCallback & value);
	void setOnDestroy(const GTweenCallback & value);
	void setOnUpdate(const GTweenCallback & value);
	void setOnCycleComplete(const GTweenCallback & value);

protected:
	GTweenNumber currentTime;
	GTweenNumber delayTime;
	GTweenNumber repeatDelayTime;
	int repeatCount;
	int cycleCount;
	GTweenNumber timeScaleTime;
	GFlags<GTweenFlags> flags;

	GTweenCallback callbackOnComplete;
	GTweenCallback callbackOnDestroy;
	GTweenCallback callbackOnUpdate;
	GTweenCallback callbackOnCycleComplete;
	
private:
	friend class GTimeline;
};


} // namespace cpgf


#endif
