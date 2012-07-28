#ifndef __GSHAREDPTR_H
#define __GSHAREDPTR_H

#include "cpgf/gassert.h"

namespace cpgf {

namespace sharedpointer_internal {

template <typename T>
class GSharedDataHolder
{
public:
	explicit GSharedDataHolder(T * data)
		: data(data), referenceCount(1), weakReferenceCount(0), freeData(true) {
	}

	GSharedDataHolder(T * data, bool freeData)
		: data(data), referenceCount(1), weakReferenceCount(0), freeData(freeData) {
	}

	~GSharedDataHolder() {
	}

	void retain() {
		++this->referenceCount;
	}

	void release() {
		GASSERT(this->referenceCount > 0);

		--this->referenceCount;

		if(this->referenceCount == 0) {
			// Prefetch weak reference count because "this" maybe freed in doFreeData if RC is 0 and weak RC is 1.
			// If weak RC is already 0 before doFreeData, doFreeData will not free "this", so it's safe to free "this" if weakRC is 0.
			unsigned int weakRC = this->weakReferenceCount;

			this->doFreeData();

			if(weakRC == 0) {
				delete this;
			}
		}
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

	T * get() const {
		return this->data;
	}

	T * take() {
		this->freeData = false;
		return this->data;
	}

private:
	void doFreeData() {
		T * p = this->data;
		this->data = NULL;
		if(this->freeData) {
			delete p;
		}
	}

private:
	T * data;
	unsigned int referenceCount;
	unsigned int weakReferenceCount;
	bool freeData;
};

template <typename T>
GSharedDataHolder<T> * createSharedDataHolder(T * p)
{
	return new GSharedDataHolder<T>(p);
}

template <typename T>
GSharedDataHolder<T> * createSharedDataHolder(T * p, bool freeData)
{
	return new GSharedDataHolder<T>(p, freeData);
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
	GSharedPointer() : rawPointer(NULL) {
	}

	explicit GSharedPointer(T * p) : rawPointer(sharedpointer_internal::createSharedDataHolder(p)) {
	}

	GSharedPointer(T * p, bool freeData) : rawPointer(sharedpointer_internal::createSharedDataHolder(p, freeData)) {
	}

	explicit GSharedPointer(const GWeakPointer<T> & weakPointer);

	~GSharedPointer() {
		if(this->rawPointer != NULL) {
			this->rawPointer->release();
		}
	}

	GSharedPointer(const GSharedPointer & other) : rawPointer(other.rawPointer) {
		if(this->rawPointer != NULL) {
			this->rawPointer->retain();
		}
	}

	GSharedPointer & operator = (GSharedPointer other) {
		other.swap(*this);
		return *this;
	}

	void swap(GSharedPointer & other) {
		std::swap(this->rawPointer, other.rawPointer);
	}

	T * operator -> () const {
		return this->rawPointer->get();
	}

	T * get() const {
		return this->rawPointer->get();
	}

	T * take() {
		return this->rawPointer->take();
	}

	inline operator bool() {
		return this->rawPointer != NULL && this->rawPointer->get() != NULL;
	}

	inline operator bool() const {
		return this->rawPointer != NULL && this->rawPointer->get() != NULL;
	}

	void reset(T * p = NULL) {
		ThisType(p).swap(*this);
	}

private:
	sharedpointer_internal::GSharedDataHolder<T> * rawPointer;

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
	GWeakPointer() : rawPointer(NULL) {
	}

	explicit GWeakPointer(StrongType p) : rawPointer(p.rawPointer) {
		this->rawPointer->weakRetain();
	}

	~GWeakPointer() {
		if(this->rawPointer != NULL) {
			this->rawPointer->weakRelease();
		}
	}

	GWeakPointer(const GWeakPointer & other) : rawPointer(other.rawPointer) {
		this->rawPointer->weakRetain();
	}

	GWeakPointer & operator = (GWeakPointer other) {
		other.swap(*this);
		return *this;
	}

	void swap(GWeakPointer & other) {
		std::swap(this->rawPointer, other.rawPointer);
	}

	StrongType get() const {
		if(this->rawPointer->hasStrongReference()) {
			return StrongType(*this);
		}
		else {
			return StrongType();
		}
	}

	inline operator bool() {
		return this->rawPointer->get() != NULL;
	}

	inline operator bool() const {
		return this->rawPointer->get() != NULL;
	}

	void reset(StrongType p) {
		ThisType(p).swap(*this);
	}

private:
	sharedpointer_internal::GSharedDataHolder<T> * rawPointer;

private:
	template <typename U>
	friend class GSharedPointer;
};

template<typename T>
GSharedPointer<T>::GSharedPointer(const GWeakPointer<T> & weakPointer)
	: rawPointer(weakPointer.rawPointer)
{
	if(this->rawPointer != NULL) {
		this->rawPointer->retain();
	}
}


} // namespace cpgf



#endif
