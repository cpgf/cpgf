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
	GTweenable::GTweenable()
		: current(0), delayTime(0), repeatDelayTime(0), repeatCount(0), cycleCount(0), flags()
	{
	}

public:
	virtual ~GTweenable() {
	}

	// return true if the tween is completed
	bool tick(GTweenNumber frameTime) {
		if(this->isCompleted()) {
			return true;
		}

		if(this->isPaused()) {
			return false;
		}

		this->current += (this->flags.has(tfUseFrames) ? 1 : frameTime);

		if(this->current <= 0) {
			return false;
		}

		this->performTime(frameTime);

		return this->isCompleted();
	}

	virtual bool removeOf(const void * instance) = 0; // return true if the tween is completed

public:
	void pause() {
		this->flags.set(tfPaused);
	}

	void resume() {
		this->flags.clear(tfPaused);
	}

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
	virtual void performTime(GTweenNumber frameTime) = 0;

protected:
	void doComplete(bool emitEvent) {
		this->flags.set(tfCompleted);
		if(emitEvent && this->callbackOnComplete) {
			this->callbackOnComplete();
		}
	}

	void setOnComplete(const GTweenCallback & value) {
		this->callbackOnComplete = value;
	}

	void setUseFrames(bool value) {
		this->flags.setByBool(tfUseFrames, value);
	}

	void setDelay(GTweenNumber value) {
		this->delayTime = value;
		this->current = -value;
	}
	
	void setRepeatCount(int repeatCount) {
		this->repeatCount = repeatCount;
	}

	void setRepeatDelay(GTweenNumber value) {
		this->repeatDelayTime = value;
	}

protected:
	GTweenNumber current;
	GTweenNumber delayTime;
	GTweenNumber repeatDelayTime;
	int repeatCount;
	int cycleCount;
	GFlags<GTweenFlags> flags;

	GTweenCallback callbackOnComplete;
};


} // namespace cpgf


#endif
