#ifndef __GTWEEN_H
#define __GTWEEN_H

#include "cpgf/tween/gtweeneaseparam.h"
#include "cpgf/gcallback.h"
#include "cpgf/gcontainer.h"
#include "cpgf/gflags.h"

#include <algorithm>


namespace cpgf {

typedef GTweenNumber (*GTweenEaseFunction)(const GTweenEaseParam *);
typedef GCallback<GTweenNumber (*)(const GTweenEaseParam *)> GTweenEaseType;

namespace tween_internal {

struct GTweenItemVirtual
{
	void (*deleteSelf)(void * self);
	void (*tick)(void * self, GTweenEaseParam * param, const GTweenEaseType & ease);
	void (*reverse)(void * self);
	const void * (*getInstance)(void * self);
};

class GTweenItemBase
{
public:
	void deleteSelf();
	void tick(GTweenEaseParam * param, const GTweenEaseType & ease);
	void reverse();
	const void * getInstance();

protected:
	GTweenItemVirtual * virtualFunctions;
};

template <typename AccessorType>
class GTweenItem : public GTweenItemBase
{
private:
	typedef GTweenItemBase super;
	typedef typename AccessorType::ValueType ValueType;
	typedef GTweenItem<AccessorType> ThisType;

private:
	static void virtualDeleteSelf(void * self) {
		delete static_cast<ThisType *>(self);
	}
	
	static void virtualTick(void * self, GTweenEaseParam * param, const GTweenEaseType & ease) {
		static_cast<ThisType *>(self)->doTick(param, ease);
	}

	static void virtualReverse(void * self) {
		static_cast<ThisType *>(self)->doReverse();
	}

	static const void * virtualGetInstance(void * self) {
		return static_cast<ThisType *>(self)->doGetInstance();
	}

public:
	GTweenItem(const AccessorType & accessor, const ValueType & to)
		: super(), accessor(accessor), from(accessor()), to(to), change(to - from)
	{
		static GTweenItemVirtual thisFunctions = {
			&virtualDeleteSelf,
			&virtualTick,
			&virtualReverse,
			&virtualGetInstance
		};
		this->virtualFunctions = &thisFunctions;
	}

protected:
	void doTick(GTweenEaseParam * param, const GTweenEaseType & ease) {
		GTweenNumber ratio = ease(param);
		this->accessor((ValueType)(this->from + this->change * ratio));
	}

	void doReverse() {
		using std::swap;
		swap(this->from, this->to);
		this->change = -this->change;
		this->accessor((ValueType)(this->from));
	}

	const void * doGetInstance() {
		return this->accessor.getInstance();
	}

private:
	AccessorType accessor;
	ValueType from;
	ValueType to;
	ValueType change;
};

} // namespace tween_internal


class GTween
{
private:
	typedef GWiseList<tween_internal::GTweenItemBase *> ListType;

	enum GTweenFlags {
		tfInited = 1 << 0,
		tfCompleted = 1 << 1,
		tfUseFrames = 1 << 2,
		tfBackward = 1 << 3,
		tfReverseWhenRepeat = 1 << 4,
		tfWaitForStart = 1 << 5,
		tfImmediateTick = 1 << 6,
		tfImmediateYoyo = 1 << 7,
	};

public:
	GTween();
	~GTween();

	void tick(GTweenNumber frameTime);

	template <typename AccessorType>
	GTween & tween(const AccessorType & accessor, const typename AccessorType::ValueType & target)
	{
		this->itemList.push_back(new tween_internal::GTweenItem<AccessorType>(accessor, target));
		return *this;
	}

	GTween & ease(const GTweenEaseType & ease);
	GTween & duration(GTweenNumber total);
	GTween & backward(bool value);
	GTween & useFrames(bool value);
	GTween & delay(GTweenNumber d);
	GTween & immediateTick(bool value);

	GTween & repeat(int repeatCount);
	GTween & yoyo(bool value);
	GTween & repeatDelay(GTweenNumber d);
	GTween & immediateYoyo(bool value);

	bool isCompleted() const;

	void removeOf(const void * instance);

private:
	void init();
	void reverseAll();

private:
	GTweenEaseType easeCallback;
	GTweenNumber current;
	GTweenNumber total;
	GTweenNumber delayTime;
	GTweenNumber repeatDelayTime;
	int repeatCount;

	ListType itemList;
	GFlags<GTweenFlags> flags;
};


} // namespace cpgf



#endif
