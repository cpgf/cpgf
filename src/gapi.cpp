#include "cpgf/gapi.h"

#include "pinclude/gapiimpl.h"

#include <stdlib.h>


namespace cpgf {



ImplApiBase::ImplApiBase()
	: referenceCount(1)
{
}

ImplApiBase::~ImplApiBase()
{
}

uint32_t ImplApiBase::doQueryInterface(void *, void *)
{
	return 0;
}

uint32_t ImplApiBase::doAddReference()
{
	++this->referenceCount;

	return this->referenceCount;
}

uint32_t ImplApiBase::doReleaseReference()
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



ImplApiObject::ImplApiObject()
	: errorInfo()
{
}

ImplApiObject::~ImplApiObject()
{
}

void ImplApiObject::clearError()
{
	if(this->errorInfo) {
		this->errorInfo->errorCode = apiError_None;
	}
}

void ImplApiObject::handleError(int errorCode, const char * message)
{
	if(! this->errorInfo) {
		this->errorInfo.reset(new ErrorInfo);
	}

	this->errorInfo->errorCode = errorCode;
	this->errorInfo->message = message;
}

int32_t ImplApiObject::doGetErrorCode()
{
	if(this->errorInfo) {
		return this->errorInfo->errorCode;
	}
	else {
		return apiError_None;
	}
}

const char * ImplApiObject::doGetErrorMessage()
{
	if(this->errorInfo) {
		return this->errorInfo->message.c_str();
	}
	else {
		return NULL;
	}
}


void * G_API_CC ImplApiAllocator::allocate(uint32_t size)
{
	return malloc(size);
}

void G_API_CC ImplApiAllocator::free(void * p)
{
	::free(p);
}

void * G_API_CC ImplApiAllocator::reallocate(void * p, uint32_t size)
{
	return realloc(p, size);
}


} // namespace cpgf

