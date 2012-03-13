#ifndef __GAPI_H
#define __GAPI_H


#include "cpgf/ginterface.h"
#include "cpgf/gscopedptr.h"


namespace cpgf {

struct IExtendObject : public IObject
{
	virtual int32_t G_API_CC getErrorCode() = 0;
	virtual const char * G_API_CC getErrorMessage() = 0;
};

struct IMemoryAllocator : public IObject
{
	virtual void * G_API_CC allocate(uint32_t size) = 0;
	virtual void G_API_CC free(void * p) = 0;
	virtual void * G_API_CC reallocate(void * p, uint32_t size) = 0;
};


template <typename T>
struct GScopedInterfaceDeleter
{
	static inline void Delete(T * p) {
		if(p) {
			p->releaseReference();
		}
	}
};

template <typename T>
struct GScopedInterfaceResetPredict
{
	// dont restrict that p != this->rawPointer
	// even if the pointer is the same, it's necessary to reset to release one reference count.
	static inline bool CanReset(T * pointerOfMine, T * pointerToReset) {
		(void)pointerOfMine;
		(void)pointerToReset;

		return true;
	}
};

template <typename T>
class GScopedInterface : public GScopedPointer<T, GScopedInterfaceDeleter<T>, GScopedInterfaceResetPredict<T> >
{
private:
	typedef GScopedPointer<T, GScopedInterfaceDeleter<T>, GScopedInterfaceResetPredict<T> > super;

public:
	GScopedInterface(): super() {
	}

	explicit GScopedInterface(T * p) : super(p) {
	}

};


template <typename T>
class GSharedInterface
{
private:
    typedef GSharedInterface<T> ThisType;

public:
	GSharedInterface(): rawPointer(NULL) {
	}

	explicit GSharedInterface(T * p) : rawPointer(p) {
		if(this->rawPointer != NULL) {
			this->rawPointer->addReference();
		}
	}

	GSharedInterface(const GSharedInterface & other) : rawPointer(other.rawPointer) {
		if(this->rawPointer != NULL) {
			this->rawPointer->addReference();
		}
	}
	
	GSharedInterface & operator = (GSharedInterface other) {
		this->swap(other);
	}
	
	~GSharedInterface() {
		if(this->rawPointer != NULL) {
			this->rawPointer->releaseReference();
		}
	}

	inline void reset(T * p = NULL) {
		ThisType(p).swap(*this);
	}

	inline T & operator * () const {
		return *this->rawPointer;
	}

	inline T * operator -> () const {
		return this->rawPointer;
	}

	inline bool operator ! () const {
		return this->rawPointer == NULL;
	}

	inline T * get() const {
		return this->rawPointer;
	}

	inline T * take() {
		T * p = this->rawPointer;
		
		this->rawPointer = NULL;
		
		return p;
	}

	inline operator bool() {
		return this->rawPointer != NULL;
	}

	inline operator bool() const {
		return this->rawPointer != NULL;
	}

	inline void swap(GSharedInterface & b) {
		T * temp = b.rawPointer;
		b.rawPointer = this->rawPointer;
		this->rawPointer = temp;
	}

private:
	void operator == (const GSharedInterface &) const;
	void operator != (const GSharedInterface &) const;

protected:
	T * rawPointer;
};




} // namespace cpgf



#endif
