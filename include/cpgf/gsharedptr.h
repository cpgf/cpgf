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

	bool unique() const {
		return this->referenceCount == 1;
	}

private:
	unsigned int referenceCount;
	unsigned int weakReferenceCount;
	bool freeData;
};

struct StaticCastTag {};
struct ConstCastTag {};
struct DynamicCastTag {};


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

	template <typename U>
	explicit GSharedPointer(U * p) : data(p), counter(new sharedpointer_internal::GSharedCounter()) {
	}

	template <typename U>
	GSharedPointer(U * p, bool freeData) : data(p), counter(new sharedpointer_internal::GSharedCounter(freeData)) {
	}

	explicit GSharedPointer(const GWeakPointer<T> & weakPointer);

	~GSharedPointer() {
		if(this->counter != NULL) {
			if(this->counter->release()) {
				delete this->data;
			}
		}
	}

	GSharedPointer(const GSharedPointer & other) : data(other.data), counter(other.counter) {
		if(this->counter != NULL) {
			this->counter->retain();
		}
	}

	template <typename U>
	GSharedPointer(const GSharedPointer<U> & other) : data(other.data), counter(other.counter) {
		if(this->counter != NULL) {
			this->counter->retain();
		}
	}

	template <typename U>
	GSharedPointer(const GSharedPointer<U> & other, sharedpointer_internal::StaticCastTag) : data(static_cast<T *>(other.data)), counter(other.counter) {
		if(this->counter != NULL) {
			this->counter->retain();
		}
	}

	template <typename U>
	GSharedPointer(const GSharedPointer<U> & other, sharedpointer_internal::ConstCastTag) : data(const_cast<T *>(other.data)), counter(other.counter) {
		if(this->counter != NULL) {
			this->counter->retain();
		}
	}

	template <typename U>
	GSharedPointer(const GSharedPointer<U> & other, sharedpointer_internal::DynamicCastTag) : data(dynamic_cast<T *>(other.data)), counter(other.counter) {
		if(this->data != NULL && this->counter != NULL) {
			this->counter->retain();
		}
	}

	GSharedPointer & operator = (const GSharedPointer & other) {
		ThisType(other).swap(*this);
		return *this;
	}

	template <typename U>
	GSharedPointer & operator = (const GSharedPointer<U> & other) {
		ThisType(other).swap(*this);
		return *this;
	}

	template <typename U>
	bool operator == (GSharedPointer<U> other) {
		return this->data == other.data && this->counter == other.counter;
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

	bool unique() const {
		return this->counter->unique();
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
	friend class GSharedPointer;

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

	explicit GWeakPointer(const GSharedPointer<T> & p) : data(p.get()), counter(p.counter) {
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

	template <typename U>
	GWeakPointer(const GWeakPointer<U> & other) : data(other.data), counter(other.counter) {
		this->counter->weakRetain();
	}

	GWeakPointer & operator = (const GWeakPointer & other) {
		ThisType(other).swap(*this);
		return *this;
	}

	template <typename U>
	GWeakPointer & operator = (const GWeakPointer<U> & other) {
		ThisType(other).swap(*this);
		return *this;
	}

	template <typename U>
	bool operator == (GWeakPointer<U> other) {
		return this->data == other.data && this->counter == other.counter;
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

	bool expired() const {
		return ! this->counter->hasStrongReference();
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

	template <typename U>
	friend class GWeakPointer;
};

template<typename T>
GSharedPointer<T>::GSharedPointer(const GWeakPointer<T> & weakPointer)
	: data(weakPointer.data), counter(weakPointer.counter)
{
	if(this->counter != NULL) {
		this->counter->retain();
	}
}

template<typename T, typename U>
GSharedPointer<T> sharedStaticCast(const GSharedPointer<U> & other)
{
	return GSharedPointer<T>(other, sharedpointer_internal::StaticCastTag());
}

template<typename T, typename U>
GSharedPointer<T> sharedConstCast(const GSharedPointer<U> & other)
{
	return GSharedPointer<T>(other, sharedpointer_internal::ConstCastTag());
}

template<typename T, typename U>
GSharedPointer<T> sharedDynamicCast(const GSharedPointer<U> & other)
{
	return GSharedPointer<T>(other, sharedpointer_internal::DynamicCastTag());
}


} // namespace cpgf



#endif
