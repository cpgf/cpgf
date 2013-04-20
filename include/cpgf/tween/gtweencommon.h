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

	virtual GTweenNumber getDuration() = 0;

	// return true if the tween is completed
	bool tick(GTweenNumber frameTime) {
		return this->doTick(frameTime, false);
	}

	GTweenNumber getTotalDuration();

public:
	void pause();
	void resume();
	virtual void restart();

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

protected:
	bool doTick(GTweenNumber frameTime, bool forceReversed);
	virtual void performTime(GTweenNumber frameTime, bool forceReversed) = 0;

protected:
	void doImmediateTick();
	void doComplete(bool emitEvent);
	void setOnComplete(const GTweenCallback & value);
	void setBackward(bool value);
	void setUseFrames(bool value);
	void setDelay(GTweenNumber value);
	void setRepeatCount(int repeatCount);
	void setRepeatDelay(GTweenNumber value);
	void setYoyo(bool value);
	void setTimeScale(GTweenNumber value);

protected:
	GTweenNumber currentTime;
	GTweenNumber delayTime;
	GTweenNumber repeatDelayTime;
	int repeatCount;
	int cycleCount;
	GTweenNumber timeScaleTime;
	GFlags<GTweenFlags> flags;

	GTweenCallback callbackOnComplete;
	
private:
	friend class GTimeline;
};


} // namespace cpgf


#endif
