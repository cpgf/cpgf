#ifndef __GAPI_H
#define __GAPI_H


#include "cpgf/gcompiler.h"
#include "cpgf/gstdint.h"
#include "cpgf/gscopedptr.h"

#include <string>


namespace cpgf {

typedef int32_t gapi_bool;

struct IObject
{
   virtual uint32_t G_API_CC unused_queryInterface(void *, void *) = 0;
   virtual uint32_t G_API_CC addReference() = 0;
   virtual uint32_t G_API_CC releaseReference() = 0;
};

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



} // namespace cpgf



#endif
