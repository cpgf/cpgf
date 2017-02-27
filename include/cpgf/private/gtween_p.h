#ifndef CPGF_GTWEEN_P_H
#define CPGF_GTWEEN_P_H

#include "cpgf/gflags.h"

namespace cpgf {

namespace tween_internal {

struct GTweenItemVirtual
{
	void (*deleteSelf)(void * self);
	void (*tick)(void * self, GTweenEaseParam * param, const GTweenEaseType & ease);
	void (*init)(void * self);
	const void * (*getInstance)(void * self);
};

class GTweenItem
{
public:
	void deleteSelf();
	void tick(GTweenEaseParam * param, const GTweenEaseType & ease);
	void init();
	const void * getInstance();

protected:
	GTweenItemVirtual * virtualFunctions;
};

enum GTweenTargetItemFlag
{
	ttifRelative = 1 << 0,
	ttifHasFrom = 1 << 1
};

template <typename AccessorType>
class GTweenTargetItem : public GTweenItem
{
private:
	typedef GTweenItem super;
	typedef typename AccessorType::ValueType ValueType;
	typedef GTweenTargetItem<AccessorType> ThisType;

private:
	static void virtualDeleteSelf(void * self) {
		delete static_cast<ThisType *>(self);
	}
	
	static void virtualTick(void * self, GTweenEaseParam * param, const GTweenEaseType & ease) {
		static_cast<ThisType *>(self)->doTick(param, ease);
	}

	static void virtualInit(void * self) {
		static_cast<ThisType *>(self)->doInit();
	}

	static const void * virtualGetInstance(void * self) {
		return static_cast<ThisType *>(self)->doGetInstance();
	}

public:
	GTweenTargetItem(const AccessorType & accessor, const ValueType & from, const ValueType & to, const cpgf::GFlags<GTweenTargetItemFlag> & flags)
		: super(), accessor(accessor), from(from), to(to), change(to), flags(flags)
	{
		static GTweenItemVirtual thisFunctions = {
			&virtualDeleteSelf,
			&virtualTick,
			&virtualInit,
			&virtualGetInstance
		};
		this->virtualFunctions = &thisFunctions;
	}

protected:
	void doTick(GTweenEaseParam * param, const GTweenEaseType & ease) {
		GTweenNumber ratio = ease(param);
		ValueType value = (ValueType)(this->from + (this->change * ratio));
		this->accessor(value);
	}

	void doInit() {
		if(this->accessor.canRead()) {
			if(! this->flags.has(ttifHasFrom)) {
				this->from = this->accessor();
			}
		}

		if(! this->flags.has(ttifRelative)) {
			this->change = this->to - this->from;
		}
	}

	const void * doGetInstance() {
		return this->accessor.getInstance();
	}

private:
	AccessorType accessor;
	ValueType from;
	ValueType to;
	ValueType change;
	cpgf::GFlags<GTweenTargetItemFlag> flags;
};


template <typename AccessorType, typename TargetGetterType>
class GTweenFollowItem : public GTweenItem
{
private:
	typedef GTweenItem super;
	typedef typename AccessorType::ValueType ValueType;
	typedef typename TargetGetterType::ValueType TargetValueType;
	typedef GTweenFollowItem<AccessorType, TargetGetterType> ThisType;

private:
	static void virtualDeleteSelf(void * self) {
		delete static_cast<ThisType *>(self);
	}
	
	static void virtualTick(void * self, GTweenEaseParam * param, const GTweenEaseType & ease) {
		static_cast<ThisType *>(self)->doTick(param, ease);
	}

	static void virtualInit(void * self) {
		static_cast<ThisType *>(self)->doInit();
	}

	static const void * virtualGetInstance(void * self) {
		return static_cast<ThisType *>(self)->doGetInstance();
	}

public:
	GTweenFollowItem(const AccessorType & accessor, const ValueType & from, const TargetGetterType & TargetGetter)
		: super(), accessor(accessor), from(from), TargetGetter(TargetGetter)
	{
		static GTweenItemVirtual thisFunctions = {
			&virtualDeleteSelf,
			&virtualTick,
			&virtualInit,
			&virtualGetInstance
		};
		this->virtualFunctions = &thisFunctions;
	}

protected:
	void doTick(GTweenEaseParam * param, const GTweenEaseType & ease) {
		GTweenNumber ratio = ease(param);
		ValueType value = (ValueType)(this->from + (((ValueType)(this->TargetGetter()) - this->from) * ratio));
		this->accessor(value);
	}

	void doInit() {
		if(this->accessor.canRead()) {
			this->from = this->accessor();
		}
	}

	const void * doGetInstance() {
		return this->accessor.getInstance();
	}

private:
	AccessorType accessor;
	ValueType from;
	TargetGetterType TargetGetter;
};


} // namespace tween_internal


} //namespace cpgf


#endif
