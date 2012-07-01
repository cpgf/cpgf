#ifndef __GUTILITY_H
#define __GUTILITY_H

#include <map>

#include "cpgf/game/gcore.h"
#include "cpgf/gscopedptr.h"


namespace cpgf {


extern gmillisecond getMillisecond();

inline gmillisecond getCurrentMillisecond()
{
	return getMillisecond();
}


template <typename T>
class GArray
{
public:
	GArray(int capacity) : capacity(capacity), data(new T[capacity]) {
		this->fillZero();
	}

	void fillZero() {
		this->fill(T());
	}

	void fill(const T & value) {
		for(int i = 0; i < this->capacity; ++i) {
			this->data[i] = value;
		}
	}

	bool anyEqual(const T & value) const {
		for(int i = 0; i < this->capacity; ++i) {
			if(this->data[i] == value) {
				return true;
			}
		}

		return false;
	}

	bool anyNotEqual(const T & value) const {
		for(int i = 0; i < this->capacity; ++i) {
			if(! (this->data[i] == value)) {
				return true;
			}
		}

		return false;
	}

	T & operator [] (int index) {
		return this->data[index];
	}

	inline int getCapacity() const {
		return this->capacity;
	}

private:
	GArray(const GArray &);
	GArray & operator = (const GArray &);

protected:
	int capacity;
	GScopedArray<T> data;
};


template <typename T>
class GIndexedArray : public GArray<T>
{
private:
	typedef GArray<T> super;

public:
	GIndexedArray(int capacity) : super(capacity), currentIndex(0) {
	}

	T & requireNext() {
		return (*this)[this->currentIndex++];
	}

	bool isFull() const {
		return this->currentIndex >= this->getCapacity();
	}

	void reset() {
		this->currentIndex = 0;
	}

	int getCount() const {
		return this->currentIndex;
	}

protected:
	int currentIndex;
};

template <typename ClassType>
ClassType * allocateNewObject()
{
	return new ClassType;
}

template <typename ClassType, typename ParamType1>
ClassType * allocateNewObject(ParamType1 p1)
{
	return new ClassType(p1);
}

template <typename ClassType, typename ParamType1, typename ParamType2>
ClassType * allocateNewObject(ParamType1 p1, ParamType2 p2)
{
	return new ClassType(p1, p2);
}

template <typename ReturnType, typename ClassType>
ReturnType * allocateNewBasedObject()
{
	return new ClassType();
}

template <typename ReturnType, typename ClassType, typename ParamType1>
ReturnType * allocateNewBasedObject(ParamType1 p1)
{
	return new ClassType(p1);
}

template <typename ReturnType, typename ClassType, typename ParamType1, typename ParamType2>
ReturnType * allocateNewBasedObject(ParamType1 p1, ParamType2 p2)
{
	return new ClassType(p1, p2);
}


// inspired by boost::static_assert
#define GStaticAsser(b) typedef GStaticAssertTest<sizeof(GStaticAssertFail<(bool)b>)> _StaticAssert_;

template <bool b>
struct GStaticAssertFail;

template <>
struct GStaticAssertFail <true> { typedef bool AssertFailure; };

template <int n>
struct GStaticAssertTest {};

template <typename T1, typename T2>
struct GTypeCompare { enum { same = false }; };

template <typename T>
struct GTypeCompare<T, T> { enum { same = true }; };



} // namespace cpgf


#endif

