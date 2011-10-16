#include "cpgf/gexception.h"

#include <stdio.h>
#include <stdarg.h>


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif


namespace cpgf {

GException::GException(int errorCode, const std::string & message)
	: super(message), errorCode(errorCode)
{
}

int GException::getCode() const
{
	return this->errorCode;
}

const char * GException::getMessage() const
{
	return this->what();
}

void raiseException(int errorCode, const std::string & message)
{
	throw GException(errorCode, message);
}

void raiseFormatException(int errorCode, const char * message, ...)
{
	char buffer[4096];

	va_list args;
	va_start(args, message);

	vsprintf(buffer, message, args);

	va_end (args);

	raiseException(errorCode, std::string(buffer));
}



} // namespace cpgf


#if defined(_MSC_VER)
#pragma warning(pop)
#endif

