#ifndef CPGF_GSHAREDINTERFACE_H
#define CPGF_GSHAREDINTERFACE_H

#include <cstddef>
#include <algorithm>


namespace cpgf {


template <typename T>
class GSharedInterface
{
private:
    typedef GSharedInterface<T> ThisType;

public:
	GSharedInterface(): rawPointer(nullptr) {
	}

	explicit GSharedInterface(T * p) : rawPointer(p) {
		if(this->rawPointer != nullptr) {
			this->rawPointer->addReference();
		}
	}

	GSharedInterface(const GSharedInterface & other) : rawPointer(other.rawPointer) {
		if(this->rawPointer != nullptr) {
			this->rawPointer->addReference();
		}
	}
	
	GSharedInterface & operator = (GSharedInterface other) {
		this->swap(other);
		
		return *this;
	}
	
	~GSharedInterface() {
		if(this->rawPointer != nullptr) {
			this->rawPointer->releaseReference();
		}
	}

	inline void reset(T * p = nullptr) {
		ThisType(p).swap(*this);
	}

	inline T & operator * () const {
		return *this->rawPointer;
	}

	inline T * operator -> () const {
		return this->rawPointer;
	}

	inline bool operator ! () const {
		return this->rawPointer == nullptr;
	}

	inline T * get() const {
		return this->rawPointer;
	}

	inline T * take() {
		T * p = this->rawPointer;
		
		this->rawPointer = nullptr;
		
		return p;
	}

	inline operator bool() {
		return this->rawPointer != nullptr;
	}

	inline operator bool() const {
		return this->rawPointer != nullptr;
	}

	inline void swap(GSharedInterface & b) {
		using std::swap;

		swap(this->rawPointer, b.rawPointer);
	}

private:
	void operator == (const GSharedInterface &) const;
	void operator != (const GSharedInterface &) const;

protected:
	T * rawPointer;
};

template <typename T>
void swap(GSharedInterface<T> & a, GSharedInterface<T> & b)
{
	a.swap(b);
}


} // namespace cpgf


#endif
