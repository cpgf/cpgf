#ifndef __GAPIIMPL_H
#define __GAPIIMPL_H


#include "cpgf/gcompiler.h"
#include "cpgf/gstdint.h"
#include "cpgf/gscopedptr.h"

#include <string>


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




} // namespace cpgf



#endif
