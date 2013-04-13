#ifndef __GTWEEN_P_H
#define __GTWEEN_P_H


namespace cpgf {

namespace tween_internal {

struct GTweenItemVirtual
{
	void (*deleteSelf)(void * self);
	void (*tick)(void * self, GTweenEaseParam * param, const GTweenEaseType & ease);
	void (*init)(void * self);
	void (*reverse)(void * self);
	void (*rewind)(void * self);
	const void * (*getInstance)(void * self);
};

class GTweenItemBase
{
public:
	void deleteSelf();
	void tick(GTweenEaseParam * param, const GTweenEaseType & ease);
	void init();
	void reverse();
	void rewind();
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

	static void virtualInit(void * self) {
		static_cast<ThisType *>(self)->doInit();
	}

	static void virtualReverse(void * self) {
		static_cast<ThisType *>(self)->doReverse();
	}

	static void virtualRewind(void * self) {
		static_cast<ThisType *>(self)->doRewind();
	}

	static const void * virtualGetInstance(void * self) {
		return static_cast<ThisType *>(self)->doGetInstance();
	}

public:
	GTweenItem(const AccessorType & accessor, const ValueType & from, const ValueType & to)
		: super(), accessor(accessor), from(from), to(to), change(to - from)
	{
		static GTweenItemVirtual thisFunctions = {
			&virtualDeleteSelf,
			&virtualTick,
			&virtualInit,
			&virtualReverse,
			&virtualRewind,
			&virtualGetInstance
		};
		this->virtualFunctions = &thisFunctions;
	}

protected:
	void doTick(GTweenEaseParam * param, const GTweenEaseType & ease) {
		GTweenNumber ratio = ease(param);
		ValueType value = (ValueType)(this->from + this->change * ratio);
		this->accessor(value);
	}

	void doInit() {
		if(this->accessor.canRead()) {
			this->from = this->accessor();
			this->change = this->to - this->from;
		}
	}

	void doReverse() {
		using std::swap;
		swap(this->from, this->to);
		this->change = -this->change;
		this->accessor(this->from);
	}

	void doRewind() {
		this->accessor(this->from);
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


template <typename AccessorType, typename TargetGetterType>
class GTweenFollowItem : public GTweenItemBase
{
private:
	typedef GTweenItemBase super;
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

	static void virtualReverse(void * self) {
		static_cast<ThisType *>(self)->doReverse();
	}

	static void virtualRewind(void * self) {
		static_cast<ThisType *>(self)->doRewind();
	}

	static const void * virtualGetInstance(void * self) {
		return static_cast<ThisType *>(self)->doGetInstance();
	}

public:
	GTweenFollowItem(const AccessorType & accessor, const ValueType & from, const TargetGetterType & TargetGetter)
		: super(), accessor(accessor), from(from), TargetGetter(TargetGetter), reversed(false)
	{
		static GTweenItemVirtual thisFunctions = {
			&virtualDeleteSelf,
			&virtualTick,
			&virtualInit,
			&virtualReverse,
			&virtualRewind,
			&virtualGetInstance
		};
		this->virtualFunctions = &thisFunctions;
	}

protected:
	void doTick(GTweenEaseParam * param, const GTweenEaseType & ease) {
		GTweenNumber ratio = ease(param);
		ValueType value;
		if(this->reversed) {
			value = (ValueType)((ValueType)(this->TargetGetter()) + (this->from - (ValueType)(this->TargetGetter())) * ratio);
		}
		else {
			value = (ValueType)(this->from + ((ValueType)(this->TargetGetter()) - this->from) * ratio);
		}
		this->accessor(value);
	}

	void doInit() {
		if(this->accessor.canRead()) {
			this->from = this->accessor();
		}
	}

	void doReverse() {
		this->reversed = ! this->reversed;
		this->doRewind();
	}

	void doRewind() {
		if(this->reversed) {
			this->accessor(this->TargetGetter());
		}
		else {
			this->accessor(this->from);
		}
	}

	const void * doGetInstance() {
		return this->accessor.getInstance();
	}

private:
	AccessorType accessor;
	ValueType from;
	TargetGetterType TargetGetter;
	bool reversed;
};


} // namespace tween_internal


} //namespace cpgf


#endif
