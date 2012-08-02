#ifndef __GSHAREDPTR_H
#define __GSHAREDPTR_H

#include "cpgf/gassert.h"

namespace cpgf {

namespace sharedpointer_internal {

class GSharedCounter
{
public:
	explicit GSharedCounter()
		: referenceCount(1), weakReferenceCount(0), freeData(true) {
	}

	explicit GSharedCounter(bool freeData)
		: referenceCount(1), weakReferenceCount(0), freeData(freeData) {
	}

	~GSharedCounter() {
	}

	void retain() {
		++this->referenceCount;
	}

	bool release() {
		GASSERT(this->referenceCount > 0);

		--this->referenceCount;

		if(this->referenceCount == 0) {
			bool freeIt = this->freeData;
			if(this->weakReferenceCount == 0) {
				delete this;
			}
			return freeIt;
		}
		return false;
	}

	void weakRetain() {
		++this->weakReferenceCount;
	}

	void weakRelease() {
		GASSERT(this->weakReferenceCount > 0);

		--this->weakReferenceCount;

		if(this->weakReferenceCount == 0 && this->referenceCount == 0) {
			delete this;
		}
	}

	bool hasStrongReference() const {
		return this->referenceCount > 0;
	}

	void take() {
		this->freeData = false;
	}

private:
	unsigned int referenceCount;
	unsigned int weakReferenceCount;
	bool freeData;
};

} // namespace sharedpointer_internal

template <typename T>
class GWeakPointer;

template <typename T>
class GSharedPointer
{
private:
	typedef GSharedPointer<T> ThisType;

public:
	GSharedPointer() : data(NULL), counter(NULL) {
	}

	explicit GSharedPointer(T * p) : data(p), counter(new sharedpointer_internal::GSharedCounter()) {
	}

	GSharedPointer(T * p, bool freeData) : data(p), counter(new sharedpointer_internal::GSharedCounter(freeData)) {
	}

	explicit GSharedPointer(const GWeakPointer<T> & weakPointer);

	~GSharedPointer() {
		if(this->counter != NULL) {
			if(this->counter->release()) {
				delete this->data;
			}
		}
	}

	GSharedPointer(const GSharedPointer<T> & other) : data(other.data), counter(other.counter) {
		if(this->counter != NULL) {
			this->counter->retain();
		}
	}

	GSharedPointer & operator = (GSharedPointer<T> other) {
		ThisType(other).swap(*this);
		return *this;
	}

	void swap(GSharedPointer & other) {
		using std::swap;

		swap(this->data, other.data);
		swap(this->counter, other.counter);
	}

	T * operator -> () const {
		return this->data;
	}

	T * get() const {
		return this->data;
	}

	T * take() {
		this->counter->take();
		return this->data;
	}

	inline operator bool() {
		return this->data != NULL;
	}

	inline operator bool() const {
		return this->data != NULL;
	}

	void reset(T * p) {
		ThisType(p).swap(*this);
	}

	void reset() {
		ThisType().swap(*this);
	}

private:
	T * data;
	sharedpointer_internal::GSharedCounter * counter;

private:
	template <typename U>
	friend class GWeakPointer;
};


template <typename T>
class GWeakPointer
{
private:
	typedef GWeakPointer<T> ThisType;
	typedef GSharedPointer<T> StrongType;

public:
	GWeakPointer() : data(NULL), counter(NULL) {
	}

	explicit GWeakPointer(StrongType p) : data(p.get()), counter(p.counter) {
		this->counter->weakRetain();
	}

	~GWeakPointer() {
		if(this->counter != NULL) {
			this->counter->weakRelease();
		}
	}

	GWeakPointer(const GWeakPointer & other) : data(other.data), counter(other.counter) {
		this->counter->weakRetain();
	}

	GWeakPointer & operator = (const GWeakPointer & other) {
		ThisType(other).swap(*this);
		return *this;
	}

	void swap(GWeakPointer & other) {
		using std::swap;

		swap(this->data, other.data);
		swap(this->counter, other.counter);
	}

	StrongType get() const {
		if(this->counter->hasStrongReference()) {
			return StrongType(*this);
		}
		else {
			return StrongType();
		}
	}

	inline operator bool() {
		return this->counter->hasStrongReference() && this->data != NULL;
	}

	inline operator bool() const {
		return this->counter->hasStrongReference() && this->data != NULL;
	}

	void reset(StrongType p) {
		ThisType(p).swap(*this);
	}

private:
	T * data;
	sharedpointer_internal::GSharedCounter * counter;

private:
	template <typename U>
	friend class GSharedPointer;
};

template<typename T>
GSharedPointer<T>::GSharedPointer(const GWeakPointer<T> & weakPointer)
	: data(weakPointer.data), counter(weakPointer.counter)
{
	if(this->counter != NULL) {
		this->counter->retain();
	}
}


} // namespace cpgf



#endif
