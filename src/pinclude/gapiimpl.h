#ifndef __GAPIIMPL_H
#define __GAPIIMPL_H


#include "cpgf/gcompiler.h"
#include "cpgf/gstdint.h"
#include "cpgf/gscopedptr.h"

#include <string>


#define IMPL_BASE \
	virtual uint32_t G_API_CC unused_queryInterface(void *, void *) { return 0; } \
	virtual uint32_t G_API_CC addReference() { return this->doAddReference(); } \
	virtual uint32_t G_API_CC releaseReference() { return this->doReleaseReference(); }

#define IMPL_OBJECT \
protected: \
	virtual int32_t G_API_CC getErrorCode() { return this->doGetErrorCode(); } \
	virtual const char * G_API_CC getErrorMessage() { return this->doGetErrorMessage(); }



namespace cpgf {


class ImplApiBase
{
public:
	ImplApiBase();
	virtual ~ImplApiBase();

protected:
	uint32_t doQueryInterface(void *, void *);
	uint32_t doAddReference();
	uint32_t doReleaseReference();

private:
	unsigned int referenceCount;
};

class ImplApiObject : public ImplApiBase
{
private:
	struct ErrorInfo {
		int32_t errorCode;
		std::string message;
	};

public:
	ImplApiObject();
	virtual ~ImplApiObject();

public:
	void clearError();
	void handleError(int errorCode, const char * message);

protected:
	int32_t doGetErrorCode();
	const char * doGetErrorMessage();

private:
	GScopedPointer<ErrorInfo> errorInfo;
};


class ImplApiAllocator : public ImplApiObject, public IApiAllocator
{
protected:
	IMPL_BASE
	IMPL_OBJECT

	virtual void * G_API_CC allocate(uint32_t size);
	virtual void G_API_CC free(void * p);
	virtual void * G_API_CC reallocate(void * p, uint32_t size);
};



} // namespace cpgf



#endif
