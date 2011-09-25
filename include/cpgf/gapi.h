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




} // namespace cpgf



#endif
