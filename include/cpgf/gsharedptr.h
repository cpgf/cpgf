#ifndef __GSHAREDPTR_H
#define __GSHAREDPTR_H


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
		if(this->freeData) {
			delete this->data;
		}
	}

	void retain() {
		++this->referenceCount;
	}

	void release() {
		--this->referenceCount;
		if(this->referenceCount <= 0) {
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
	GSharedPointer() : holder(new sharedpointer_internal::GSharedDataHolder<T>(NULL)) {
	}

	explicit GSharedPointer(T * p) : holder(new sharedpointer_internal::GSharedDataHolder<T>(p)) {
	}

	explicit GSharedPointer(T * p, bool freeData) : holder(new sharedpointer_internal::GSharedDataHolder<T>(p, freeData)) {
	}

	~GSharedPointer() {
		this->holder->release();
	}

	GSharedPointer(const GSharedPointer & other) : holder(other.holder) {
		this->holder->retain();
	}

	GSharedPointer & operator = (GSharedPointer other) {
		other.swap(*this);
		return *this;
	}

	void swap(GSharedPointer & other) {
		std::swap(this->holder, other.holder);
	}

	T * operator -> () const {
		return this->holder->get();
	}

	T * get() const {
		return this->holder->get();
	}

	T * take() {
		return this->holder->take();
	}

	void reset(T * p = NULL) {
		ThisType(p).swap(*this);
	}

private:
	sharedpointer_internal::GSharedDataHolder<T> * holder;
};



} // namespace cpgf



#endif
