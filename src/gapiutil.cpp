#include "cpgf/gapiutil.h"
#include "cpgf/gerrorcode.h"

#include <stdlib.h>


namespace cpgf {


GImplObject::GImplObject()
	: referenceCount(1)
{
}

GImplObject::~GImplObject()
{
}

uint32_t GImplObject::queryInterface(void *, void *)
{
	return 0;
}

uint32_t GImplObject::addReference()
{
	++this->referenceCount;

	return this->referenceCount;
}


GImplExtendObject::GImplExtendObject()
	: errorInfo()
{
}

void GImplExtendObject::clearError()
{
	if(this->errorInfo) {
		this->errorInfo->errorCode = Error_None;
	}
}

void GImplExtendObject::handleError(int errorCode, const char * message)
{
	if(! this->errorInfo) {
		this->errorInfo.reset(new ExtendObjectErrorInfo);
	}

	this->errorInfo->errorCode = errorCode;
	this->errorInfo->message = message;
}

int32_t GImplExtendObject::getErrorCode()
{
	if(this->errorInfo) {
		return this->errorInfo->errorCode;
	}
	else {
		return Error_None;
	}
}

const char * GImplExtendObject::getErrorMessage()
{
	if(this->errorInfo) {
		return this->errorInfo->message.c_str();
	}
	else {
		return NULL;
	}
}


void * G_API_CC GImplMemoryAllocator::allocate(uint32_t size)
{
	return malloc(size);
}

void G_API_CC GImplMemoryAllocator::free(void * p)
{
	::free(p);
}

void * G_API_CC GImplMemoryAllocator::reallocate(void * p, uint32_t size)
{
	return realloc(p, size);
}


} // namespace cpgf
