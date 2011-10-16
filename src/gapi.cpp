#include "cpgf/gapi.h"
#include "cpgf/gerrorcode.h"

#include "pinclude/gapiimpl.h"

#include <stdlib.h>


namespace cpgf {



ImplRoot::ImplRoot()
	: referenceCount(1)
{
}

ImplRoot::~ImplRoot()
{
}

uint32_t ImplRoot::doQueryInterface(void *, void *)
{
	return 0;
}

uint32_t ImplRoot::doAddReference()
{
	++this->referenceCount;

	return this->referenceCount;
}

uint32_t ImplRoot::doReleaseReference()
{
	if(this->referenceCount > 0) {
		--this->referenceCount;
	}

	unsigned int refCount = this->referenceCount;

	if(this->referenceCount == 0) {
		delete this;
	}

	return refCount;
}



ImplBaseObject::ImplBaseObject()
	: errorInfo()
{
}

ImplBaseObject::~ImplBaseObject()
{
}

void ImplBaseObject::clearError()
{
	if(this->errorInfo) {
		this->errorInfo->errorCode = Error_None;
	}
}

void ImplBaseObject::handleError(int errorCode, const char * message)
{
	if(! this->errorInfo) {
		this->errorInfo.reset(new ErrorInfo);
	}

	this->errorInfo->errorCode = errorCode;
	this->errorInfo->message = message;
}

int32_t ImplBaseObject::doGetErrorCode()
{
	if(this->errorInfo) {
		return this->errorInfo->errorCode;
	}
	else {
		return Error_None;
	}
}

const char * ImplBaseObject::doGetErrorMessage()
{
	if(this->errorInfo) {
		return this->errorInfo->message.c_str();
	}
	else {
		return NULL;
	}
}


void * G_API_CC ImplMemoryAllocator::allocate(uint32_t size)
{
	return malloc(size);
}

void G_API_CC ImplMemoryAllocator::free(void * p)
{
	::free(p);
}

void * G_API_CC ImplMemoryAllocator::reallocate(void * p, uint32_t size)
{
	return realloc(p, size);
}


} // namespace cpgf

