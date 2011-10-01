#ifndef __GAPI_H
#define __GAPI_H


#include "cpgf/gcompiler.h"
#include "cpgf/gstdint.h"
#include "cpgf/gscopedptr.h"

#include <string>


namespace cpgf {

typedef int32_t gapi_bool;

const int32_t apiError_None = 0;

struct IApiBase
{
   virtual uint32_t G_API_CC unused_queryInterface(void *, void *) = 0;
   virtual uint32_t G_API_CC addReference() = 0;
   virtual uint32_t G_API_CC releaseReference() = 0;
};

struct IApiObject : public IApiBase
{
	virtual int32_t G_API_CC getErrorCode() = 0;
	virtual const char * G_API_CC getErrorMessage() = 0;
};


template <typename T>
struct GApiScopedPointerDeleter
{
	static inline void Delete(T * p) {
		if(p) {
			p->releaseReference();
		}
	}
};

template <typename T>
class GApiScopedPointer : public GScopedPointer<T, GApiScopedPointerDeleter<T> >
{
private:
	typedef GScopedPointer<T, GApiScopedPointerDeleter<T> > super;

public:
	GApiScopedPointer(): super() {
	}

	explicit GApiScopedPointer(T * p) : super(p) {
	}

};



} // namespace cpgf



#endif
