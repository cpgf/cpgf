#ifndef __GTWEEN_H
#define __GTWEEN_H

#include "cpgf/tween/gtweencommon.h"
#include "cpgf/private/gtween_p.h"
#include "cpgf/gcontainer.h"

#include <algorithm>

namespace cpgf {


class GTween : public GTweenable
{
private:
	typedef GTweenable super;
	typedef GWiseList<tween_internal::GTweenItem *> ListType;

public:
	GTween();
	~GTween();

	virtual bool removeOf(const void * instance);
	virtual GTweenNumber getDuration();

	template <typename AccessorType>
	GTween & target(const AccessorType & accessor, const typename AccessorType::ValueType & target)
	{
		this->itemList.push_back(new tween_internal::GTweenTargetItem<AccessorType>(accessor, accessor(), target, false));
		return *this;
	}

	template <typename AccessorType>
	GTween & target(const AccessorType & accessor, const typename AccessorType::ValueType & from, const typename AccessorType::ValueType & target)
	{
		this->itemList.push_back(new tween_internal::GTweenTargetItem<AccessorType>(accessor, from, target, false));
		return *this;
	}

	template <typename AccessorType>
	GTween & relative(const AccessorType & accessor, const typename AccessorType::ValueType & target)
	{
		this->itemList.push_back(new tween_internal::GTweenTargetItem<AccessorType>(accessor, accessor(), target, true));
		return *this;
	}

	template <typename AccessorType>
	GTween & relative(const AccessorType & accessor, const typename AccessorType::ValueType & from, const typename AccessorType::ValueType & target)
	{
		this->itemList.push_back(new tween_internal::GTweenTargetItem<AccessorType>(accessor, from, target, true));
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
	GTween & duration(GTweenNumber durationTime);
	GTween & backward(bool value);
	GTween & useFrames(bool value);
	GTween & delay(GTweenNumber value);
	GTween & timeScale(GTweenNumber value);
	GTween & immediateTick();

	GTween & repeat(int repeatCount);
	GTween & repeatDelay(GTweenNumber value);
	GTween & yoyo(bool value);

	GTween & onComplete(const GTweenCallback & value);
	GTween & onDestroy(const GTweenCallback & value);

	bool isRunning() const
	{
		return this->flags.has(tfInited) && ! this->flags.has(tfPaused);
	}

protected:
	virtual void performTime(GTweenNumber frameTime, bool forceReversed, bool forceUseFrames);

private:
	void init();

private:
	GTweenEaseType easeCallback;
	GTweenNumber durationTime;

	ListType itemList;
};


} // namespace cpgf



#endif
