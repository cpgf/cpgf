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
private:
	enum GTweenFlags {
		tfInitialized = 1 << 0,
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

	virtual void removeForInstance(const void * instance) = 0;

	virtual GTweenNumber getDuration() const = 0;
	GTweenNumber getTotalDuration() const;

	// return true if the tween is completed
	bool tick(GTweenNumber frameTime) {
		return this->doTick(this->isUseFrames() ? 1.0f : frameTime, false, false);
	}

public:
	void pause();
	void resume();
	void immediateTick();

	virtual void restart();
	virtual void restartWithDelay();

	GTweenable & backward(bool value);
	GTweenable & useFrames(bool value);
	GTweenable & delay(GTweenNumber value);
	GTweenable & timeScale(GTweenNumber value);

	GTweenable & repeat(int repeatCount);
	GTweenable & repeatDelay(GTweenNumber value);
	GTweenable & yoyo(bool value);

	GTweenable & onInitialize(const GTweenCallback & value);
	GTweenable & onComplete(const GTweenCallback & value);
	GTweenable & onDestroy(const GTweenCallback & value);
	GTweenable & onUpdate(const GTweenCallback & value);
	GTweenable & onRepeat(const GTweenCallback & value);
	
	bool isPaused() {
		return this->flags.has(tfPaused);
	}

	bool isCompleted() const {
		return this->flags.has(tfCompleted);
	}

	bool isUseFrames() const {
		return this->flags.has(tfUseFrames);
	}

	bool isBackward() const {
		return this->flags.has(tfBackward);
	}

	bool isYoyo() const {
		return this->flags.has(tfReverseWhenRepeat);
	}

	bool isRunning() const {
		return this->flags.has(tfInitialized) && ! this->flags.has(tfPaused);
	}
	
	GTweenNumber getDelay() const {
		return this->delayTime;
	}

	GTweenNumber getTimeScale() const {
		return this->timeScaleTime;
	}

protected:
	bool doTick(GTweenNumber frameTime, bool forceReversed, bool forceUseFrames);
	void doComplete(bool emitEvent);
	
	virtual void performTime(GTweenNumber frameTime, bool forceReversed, bool forceUseFrames) = 0;
	virtual void initialize();

	void toggleBackward() {
		this->flags.toggle(tfBackward);
	}

protected:
	GTweenNumber currentTime;
	GTweenNumber delayTime;
	GTweenNumber currentDelayTime;
	GTweenNumber repeatDelayTime;
	int repeatCount;
	int cycleCount;
	GTweenNumber timeScaleTime;
	GFlags<GTweenFlags> flags;

	GTweenCallback callbackOnInitialize;
	GTweenCallback callbackOnComplete;
	GTweenCallback callbackOnDestroy;
	GTweenCallback callbackOnUpdate;
	GTweenCallback callbackOnRepeat;
	
private:
	friend class GTimeline;
};


} // namespace cpgf


#endif
