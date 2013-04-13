#ifndef __GTWEEN_H
#define __GTWEEN_H

#include "cpgf/tween/gtweeneaseparam.h"
#include "cpgf/private/gtween_p.h"
#include "cpgf/gcontainer.h"
#include "cpgf/gflags.h"
#include "cpgf/gclassutil.h"

#include <algorithm>


namespace cpgf {


typedef GCallback<void ()> GTweenCallback;

class GTween : public GNoncopyable
{
private:
	typedef GWiseList<tween_internal::GTweenItemBase *> ListType;

	enum GTweenFlags {
		tfInited = 1 << 0,
		tfPaused = 1 << 1,
		tfCompleted = 1 << 2,
		tfUseFrames = 1 << 3,
		tfBackward = 1 << 4,
		tfReverseWhenRepeat = 1 << 5,
		tfRewind = 1 << 6,
	};

public:
	GTween();
	~GTween();

	void tick(GTweenNumber frameTime);

	template <typename AccessorType>
	GTween & tween(const AccessorType & accessor, const typename AccessorType::ValueType & target)
	{
		this->itemList.push_back(new tween_internal::GTweenItem<AccessorType>(accessor, accessor(), target));
		return *this;
	}

	template <typename AccessorType>
	GTween & tween(const AccessorType & accessor, const typename AccessorType::ValueType & from, const typename AccessorType::ValueType & target)
	{
		this->itemList.push_back(new tween_internal::GTweenItem<AccessorType>(accessor, from, target));
		return *this;
	}

	template <typename AccessorType, typename TargetGetterType>
	GTween & follow(const AccessorType & accessor, const TargetGetterType & TargetGetter)
	{
		this->itemList.push_back(new tween_internal::GTweenFollowItem<AccessorType, TargetGetterType>(accessor, accessor(), TargetGetter));
		return *this;
	}

	template <typename AccessorType, typename TargetGetterType>
	GTween & follow(const AccessorType & accessor, const typename AccessorType::ValueType & from, const TargetGetterType & TargetGetter)
	{
		this->itemList.push_back(new tween_internal::GTweenFollowItem<AccessorType, TargetGetterType>(accessor, from, TargetGetter));
		return *this;
	}

	GTween & ease(const GTweenEaseType & ease);
	GTween & duration(GTweenNumber total);
	GTween & backward(bool value);
	GTween & useFrames(bool value);
	GTween & delay(GTweenNumber d);

	GTween & repeat(int repeatCount);
	GTween & repeatDelay(GTweenNumber d);
	GTween & yoyo(bool value);

	GTween & onComplete(const GTweenCallback & value);

	void pause();
	void resume();

	bool isRunning() const
	{
		return this->flags.has(tfInited) && ! this->flags.has(tfPaused);
	}

	bool isCompleted() const
	{
		return this->flags.has(tfCompleted);
	}

	bool isUseFrames() const
	{
		return this->flags.has(tfUseFrames);
	}

	void removeOf(const void * instance);

private:
	void init();
	void reverseAll();
	void rewindAll();

private:
	GTweenEaseType easeCallback;
	GTweenNumber current;
	GTweenNumber total;
	GTweenNumber delayTime;
	GTweenNumber repeatDelayTime;
	int repeatCount;
	int cycleCount;

	ListType itemList;
	GFlags<GTweenFlags> flags;

	GTweenCallback callbackOnComplete;
};


} // namespace cpgf



#endif
