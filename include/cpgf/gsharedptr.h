#ifndef CPGF_GSHAREDPTR_H
#define CPGF_GSHAREDPTR_H

#include "cpgf/gassert.h"
#include "cpgf/genableif.h"
#include "cpgf/gtypetraits.h"

#include <algorithm>


namespace cpgf {

template <typename T>
class GSharedPointer;
template <typename T>
class GShareFromThis;
class GShareFromBase;


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


template <typename T, typename U>
void enableShareFromThis(const GSharedPointer<T> * sp, const U * self, typename GEnableIf<IsConvertible<U, GShareFromThis<U> >::Result >::Result * = 0)
{
	if(self != nullptr) {
		self->enableShareFromThis(sp);
	}
}

template <typename T, typename U>
void enableShareFromThis(const GSharedPointer<T> * sp, const U * self, typename GEnableIf<IsConvertible<U, GShareFromBase >::Result >::Result * = 0)
{
	if(self != nullptr) {
		self->enableShareFromThis(sp);
	}
}

template <typename T, typename U>
void enableShareFromThis(const GSharedPointer<T> *, const U *, typename GDisableIf<IsConvertible<U, GShareFromThis<U> >::Result || IsConvertible<U, GShareFromBase >::Result >::Result * = 0)
{
}


} // namespace sharedpointer_internal


template <typename T>
class GWeakPointer;

template <typename T>
class GSharedPointer
{
private:
	typedef GSharedPointer<T> ThisType;

public:
	GSharedPointer() : data(nullptr), counter(nullptr) {
	}

	explicit GSharedPointer(T * p) : data(p), counter(new sharedpointer_internal::GSharedCounter()) {
		sharedpointer_internal::enableShareFromThis(this, p);
	}

	template <typename U>
	explicit GSharedPointer(U * p) : data(p), counter(new sharedpointer_internal::GSharedCounter()) {
		sharedpointer_internal::enableShareFromThis(this, p);
	}

	template <typename U>
	GSharedPointer(U * p, bool freeData) : data(p), counter(new sharedpointer_internal::GSharedCounter(freeData)) {
		sharedpointer_internal::enableShareFromThis(this, p);
	}

	explicit GSharedPointer(const GWeakPointer<T> & weakPointer);

	~GSharedPointer() {
		if(this->counter != nullptr) {
			if(this->counter->release()) {
				delete this->data;
			}
		}
	}

	GSharedPointer(const GSharedPointer & other) : data(other.data), counter(other.counter) {
		if(this->counter != nullptr) {
			this->counter->retain();
		}
	}

	template <typename U>
	GSharedPointer(const GSharedPointer<U> & other) : data(other.data), counter(other.counter) {
		if(this->counter != nullptr) {
			this->counter->retain();
		}
	}

	template <typename U>
	GSharedPointer(const GSharedPointer<U> & other, sharedpointer_internal::StaticCastTag) : data(static_cast<T *>(other.data)), counter(other.counter) {
		if(this->counter != nullptr) {
			this->counter->retain();
		}
	}

	template <typename U>
	GSharedPointer(const GSharedPointer<U> & other, sharedpointer_internal::ConstCastTag) : data(const_cast<T *>(other.data)), counter(other.counter) {
		if(this->counter != nullptr) {
			this->counter->retain();
		}
	}

	template <typename U>
	GSharedPointer(const GSharedPointer<U> & other, sharedpointer_internal::DynamicCastTag) : data(dynamic_cast<T *>(other.data)), counter(other.counter) {
		if(this->data != nullptr && this->counter != nullptr) {
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
		return this->data != nullptr;
	}

	inline operator bool() const {
		return this->data != nullptr;
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

	template <typename U>
	bool isBefore(const GSharedPointer<U> & other) const {
		return this->counter < other.counter;
	}

	template <typename U>
	bool isBefore(const GWeakPointer<U> & other) const {
		return this->counter < other.counter;
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
void swap(GSharedPointer<T> & a, GSharedPointer<T> & b)
{
	a.swap(b);
}


template <typename T>
class GWeakPointer
{
private:
	typedef GWeakPointer<T> ThisType;
	typedef GSharedPointer<T> StrongType;

public:
	GWeakPointer() : data(nullptr), counter(nullptr) {
	}

	explicit GWeakPointer(const GSharedPointer<T> & p) : data(p.get()), counter(p.counter) {
		if(this->counter != nullptr) {
			this->counter->weakRetain();
		}
	}

	~GWeakPointer() {
		if(this->counter != nullptr) {
			this->counter->weakRelease();
		}
	}

	GWeakPointer(const GWeakPointer & other) : data(other.data), counter(other.counter) {
		if(this->counter != nullptr) {
			this->counter->weakRetain();
		}
	}

	template <typename U>
	GWeakPointer(const GWeakPointer<U> & other) : data(other.data), counter(other.counter) {
		if(this->counter != nullptr) {
			this->counter->weakRetain();
		}
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

	void swap(GWeakPointer & other) {
		using std::swap;

		swap(this->data, other.data);
		swap(this->counter, other.counter);
	}

	StrongType get() const {
		if(this->counter != nullptr && this->counter->hasStrongReference()) {
			return StrongType(*this);
		}
		else {
			return StrongType();
		}
	}

	inline operator bool() {
		return this->counter != nullptr && this->counter->hasStrongReference() && this->data != nullptr;
	}

	inline operator bool() const {
		return this->counter != nullptr && this->counter->hasStrongReference() && this->data != nullptr;
	}

	bool expired() const {
		return this->counter == nullptr || ! this->counter->hasStrongReference();
	}

	void reset(StrongType p) {
		ThisType(p).swap(*this);
	}

	template <typename U>
	bool isBefore(const GSharedPointer<U> & other) const {
		return this->counter < other.counter;
	}

	template <typename U>
	bool isBefore(const GWeakPointer<U> & other) const {
		return this->counter < other.counter;
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
class GShareFromThis
{
public:
	GSharedPointer<T> shareFromThis() const {
		return GSharedPointer<T>(this->weakThis);
	}

public:
	template <typename U>
	void enableShareFromThis(const GSharedPointer<U> * self) const {
		if(this->weakThis.expired()) {
			this->weakThis = GWeakPointer<T>(GSharedPointer<T>(*self));
		}
	}

private:
	mutable GWeakPointer<T> weakThis;
};

class GShareFromBase
{
};

template <typename T>
GSharedPointer<T>::GSharedPointer(const GWeakPointer<T> & weakPointer)
	: data(weakPointer.data), counter(weakPointer.counter)
{
	if(this->counter != nullptr) {
		this->counter->retain();
	}
}

template <typename T, typename U>
bool operator == (const GSharedPointer<T> & a, const GSharedPointer<U> & b)
{
	return a.get() == b.get();
}

template <typename T, typename U>
bool operator == (const GWeakPointer<T> & a, const GWeakPointer<U> & b)
{
	return a.get() == b.get();
}

template <typename T, typename U>
bool operator != (const GSharedPointer<T> & a, const GSharedPointer<U> & b)
{
	return a.get() != b.get();
}

template <typename T, typename U>
bool operator != (const GWeakPointer<T> & a, const GWeakPointer<U> & b)
{
	return a.get() != b.get();
}

template <typename T, typename U>
bool operator < (const GSharedPointer<T> & a, const GSharedPointer<U> & b)
{
	return a.isBefore(b);
}

template <typename T, typename U>
bool operator < (const GWeakPointer<T> & a, const GWeakPointer<U> & b)
{
	return a.isBefore(b);
}

template <typename T, typename U>
GSharedPointer<T> sharedStaticCast(const GSharedPointer<U> & other)
{
	return GSharedPointer<T>(other, sharedpointer_internal::StaticCastTag());
}

template <typename T, typename U>
GSharedPointer<T> sharedConstCast(const GSharedPointer<U> & other)
{
	return GSharedPointer<T>(other, sharedpointer_internal::ConstCastTag());
}

template <typename T, typename U>
GSharedPointer<T> sharedDynamicCast(const GSharedPointer<U> & other)
{
	return GSharedPointer<T>(other, sharedpointer_internal::DynamicCastTag());
}



} // namespace cpgf



#endif
