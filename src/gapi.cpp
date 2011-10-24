#include "cpgf/gapi.h"
#include "cpgf/gerrorcode.h"

#include "pinclude/gapiimpl.h"

#include <stdlib.h>


namespace cpgf {



ImplObject::ImplObject()
	: referenceCount(1)
{
}

ImplObject::~ImplObject()
{
}

uint32_t ImplObject::doQueryInterface(void *, void *)
{
	return 0;
}

uint32_t ImplObject::doAddReference()
{
	++this->referenceCount;

	return this->referenceCount;
}

uint32_t ImplObject::doReleaseReference()
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



ImplExtendObject::ImplExtendObject()
	: errorInfo()
{
}

ImplExtendObject::~ImplExtendObject()
{
}

void ImplExtendObject::clearError()
{
	if(this->errorInfo) {
		this->errorInfo->errorCode = Error_None;
	}
}

void ImplExtendObject::handleError(int errorCode, const char * message)
{
	if(! this->errorInfo) {
		this->errorInfo.reset(new ErrorInfo);
	}

	this->errorInfo->errorCode = errorCode;
	this->errorInfo->message = message;
}

int32_t ImplExtendObject::doGetErrorCode()
{
	if(this->errorInfo) {
		return this->errorInfo->errorCode;
	}
	else {
		return Error_None;
	}
}

const char * ImplExtendObject::doGetErrorMessage()
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

