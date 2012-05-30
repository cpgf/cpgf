#ifndef __GSHAREDPTR_H
#define __GSHAREDPTR_H

#include "cpgf/gassert.h"

namespace cpgf {

namespace sharedpointer_internal {

template <typename T>
class GSharedDataHolder
{
public:
	explicit GSharedDataHolder(T * data) : data(data), referenceCount(1), freeData(true) {
	}

	GSharedDataHolder(T * data, bool freeData) : data(data), referenceCount(1), freeData(freeData) {
	}

	~GSharedDataHolder() {
		T * p = this->data;
		this->data = NULL;
		if(this->freeData) {
			delete p;
		}
	}

	void retain() {
		++this->referenceCount;
	}

	void release() {
		--this->referenceCount;
		if(this->referenceCount <= 0) {
			GASSERT(this->referenceCount == 0);

			delete this;
		}
	}

	T * get() const {
		return this->data;
	}

	T * take() {
		this->freeData = false;
		return this->data;
	}

private:
	T * data;
	int referenceCount;
	bool freeData;
};

} // namespace sharedpointer_internal

template <typename T>
class GSharedPointer
{
private:
	typedef GSharedPointer<T> ThisType;

public:
	GSharedPointer() : rawPointer(new sharedpointer_internal::GSharedDataHolder<T>(NULL)) {
	}

	explicit GSharedPointer(T * p) : rawPointer(new sharedpointer_internal::GSharedDataHolder<T>(p)) {
	}

	explicit GSharedPointer(T * p, bool freeData) : rawPointer(new sharedpointer_internal::GSharedDataHolder<T>(p, freeData)) {
	}

	~GSharedPointer() {
		this->rawPointer->release();
	}

	GSharedPointer(const GSharedPointer & other) : rawPointer(other.rawPointer) {
		this->rawPointer->retain();
	}

	GSharedPointer & operator = (GSharedPointer other) {
		ThisType(other).swap(*this);
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
		return this->rawPointer->get() != NULL;
	}

	inline operator bool() const {
		return this->rawPointer->get() != NULL;
	}

	void reset(T * p = NULL) {
		ThisType(p).swap(*this);
	}

private:
	sharedpointer_internal::GSharedDataHolder<T> * rawPointer;
};



} // namespace cpgf



#endif
