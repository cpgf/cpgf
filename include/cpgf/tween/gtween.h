#ifndef CPGF_GTWEEN_H
#define CPGF_GTWEEN_H

#include "cpgf/tween/gtweencommon.h"
#include "cpgf/private/gtween_p.h"

#include <algorithm>
#include <vector>

namespace cpgf {


class GTween : public GTweenable
{
private:
	typedef GTweenable super;
	typedef std::vector<tween_internal::GTweenItem *> ListType;

public:
	GTween();
	~GTween();

	virtual GTweenNumber getDuration() const;
	virtual void removeForInstance(const void * instance);

	template <typename AccessorType>
	GTween & target(const AccessorType & accessor, const typename AccessorType::ValueType & targetValue)
	{
		this->itemList.push_back(new tween_internal::GTweenTargetItem<AccessorType>(accessor, accessor(), targetValue, 0));
		return *this;
	}

	template <typename AccessorType>
	GTween & target(const AccessorType & accessor, const typename AccessorType::ValueType & from, const typename AccessorType::ValueType & targetValue)
	{
		this->itemList.push_back(new tween_internal::GTweenTargetItem<AccessorType>(accessor, from, targetValue, tween_internal::ttifHasFrom));
		return *this;
	}

	template <typename AccessorType>
	GTween & relative(const AccessorType & accessor, const typename AccessorType::ValueType & relativeValue)
	{
		this->itemList.push_back(new tween_internal::GTweenTargetItem<AccessorType>(accessor, accessor(), relativeValue, tween_internal::ttifRelative));
		return *this;
	}

	template <typename AccessorType>
	GTween & relative(const AccessorType & accessor, const typename AccessorType::ValueType & from, const typename AccessorType::ValueType & relativeValue)
	{
		this->itemList.push_back(new tween_internal::GTweenTargetItem<AccessorType>(accessor, from, relativeValue, tween_internal::ttifRelative | tween_internal::ttifHasFrom));
		return *this;
	}

	template <typename AccessorType, typename TargetGetterType>
	GTween & follow(const AccessorType & accessor, const TargetGetterType & targetGetter)
	{
		this->itemList.push_back(new tween_internal::GTweenFollowItem<AccessorType, TargetGetterType>(accessor, accessor(), targetGetter));
		return *this;
	}

	template <typename AccessorType, typename TargetGetterType>
	GTween & follow(const AccessorType & accessor, const typename AccessorType::ValueType & from, const TargetGetterType & targetGetter)
	{
		this->itemList.push_back(new tween_internal::GTweenFollowItem<AccessorType, TargetGetterType>(accessor, from, targetGetter));
		return *this;
	}

	GTween & ease(const GTweenEaseType & ease);
	GTween & duration(GTweenNumber durationTime);

	GTween & backward(bool value) { return static_cast<GTween &>(super::backward(value)); }
	GTween & useFrames(bool value) { return static_cast<GTween &>(super::useFrames(value)); }
	GTween & delay(GTweenNumber value) { return static_cast<GTween &>(super::delay(value)); }
	GTween & timeScale(GTweenNumber value) { return static_cast<GTween &>(super::timeScale(value)); }

	GTween & repeat(int repeatCount) { return static_cast<GTween &>(super::repeat(repeatCount)); }
	GTween & repeatDelay(GTweenNumber value) { return static_cast<GTween &>(super::repeatDelay(value)); }
	GTween & yoyo(bool value) { return static_cast<GTween &>(super::yoyo(value)); }

	GTween & onComplete(const GTweenCallback & value) { return static_cast<GTween &>(super::onComplete(value)); }
	GTween & onDestroy(const GTweenCallback & value) { return static_cast<GTween &>(super::onDestroy(value)); }
	GTween & onUpdate(const GTweenCallback & value) { return static_cast<GTween &>(super::onUpdate(value)); }
	GTween & onRepeat(const GTweenCallback & value) { return static_cast<GTween &>(super::onRepeat(value)); }

protected:
	virtual void performTime(GTweenNumber elapsed, GTweenNumber frameDuration, bool forceReversed, bool forceUseFrames);
	virtual void initialize();

private:
	GTweenEaseType easeCallback;
	GTweenNumber durationTime;

	ListType itemList;
};


} // namespace cpgf



#endif
