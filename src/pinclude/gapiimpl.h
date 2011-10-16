#ifndef __GAPIIMPL_H
#define __GAPIIMPL_H


#include "cpgf/gcompiler.h"
#include "cpgf/gstdint.h"
#include "cpgf/gscopedptr.h"

#include <string>


#define IMPL_ROOT \
	virtual uint32_t G_API_CC unused_queryInterface(void *, void *) { return 0; } \
	virtual uint32_t G_API_CC addReference() { return this->doAddReference(); } \
	virtual uint32_t G_API_CC releaseReference() { return this->doReleaseReference(); }

#define IMPL_BASEOBJECT \
protected: \
	virtual int32_t G_API_CC getErrorCode() { return this->doGetErrorCode(); } \
	virtual const char * G_API_CC getErrorMessage() { return this->doGetErrorMessage(); }



namespace cpgf {


class ImplRoot
{
public:
	ImplRoot();
	virtual ~ImplRoot();

protected:
	uint32_t doQueryInterface(void *, void *);
	uint32_t doAddReference();
	uint32_t doReleaseReference();

private:
	unsigned int referenceCount;
};

class ImplBaseObject : public ImplRoot
{
private:
	struct ErrorInfo {
		int32_t errorCode;
		std::string message;
	};

public:
	ImplBaseObject();
	virtual ~ImplBaseObject();

public:
	void clearError();
	void handleError(int errorCode, const char * message);

protected:
	int32_t doGetErrorCode();
	const char * doGetErrorMessage();

private:
	GScopedPointer<ErrorInfo> errorInfo;
};


class ImplMemoryAllocator : public ImplBaseObject, public IMemoryAllocator
{
protected:
	IMPL_ROOT
	IMPL_BASEOBJECT

	virtual void * G_API_CC allocate(uint32_t size);
	virtual void G_API_CC free(void * p);
	virtual void * G_API_CC reallocate(void * p, uint32_t size);
};



} // namespace cpgf



#endif
