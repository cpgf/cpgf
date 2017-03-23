#ifndef CPGF_GMETADEBUG_H
#define CPGF_GMETADEBUG_H

#include "cpgf/gvariant.h"

#include <string>

namespace cpgf {

struct GMetaVariadicParam;

class GMetaDebug
{
public:
	static void debugBreak(const GMetaVariadicParam * params = 0);

	static void trace(const char * message);
	static void traceError(const char * message);
	static void traceLine(const char * message);
	static void traceErrorLine(const char * message);

	static GVariant inspect(const GVariant & value);
};


} // namespace cpgf


#endif

