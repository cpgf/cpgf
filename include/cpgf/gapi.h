#ifndef CPGF_GAPI_H
#define CPGF_GAPI_H


#include "cpgf/ginterface.h"


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


} // namespace cpgf



#endif
