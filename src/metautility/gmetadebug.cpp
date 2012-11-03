#include "cpgf/metautility/gmetadebug.h"
#include "cpgf/gcompiler.h"
#include "cpgf/gmetacommon.h"

#include <stdio.h>


namespace cpgf {


void GMetaDebug::debugBreak(const GMetaVariadicParam * params)
{
#if defined(__i386) || defined(_M_IX86)
	#if defined(G_COMPILER_VC)
		__asm int 3;
	#elif defined(G_COMPILER_GCC)
		__asm__("int $0x03");
	#endif
#else
#endif

	if(params != NULL) {
		// Nothing to do. Inspect params in the debugger if you need.
		GVariant value;
		for(size_t i = 0; i < params->paramCount; i++) {
			value = *(params->params[i]);
		}
	}
}

void GMetaDebug::trace(const char * message)
{
	fprintf(stdout, "%s", message);
}

void GMetaDebug::traceError(const char * message)
{
	fprintf(stderr, "%s", message);
}

void GMetaDebug::traceLine(const char * message)
{
	fprintf(stdout, "%s\n", message);
}

void GMetaDebug::traceErrorLine(const char * message)
{
	fprintf(stderr, "%s\n", message);
}


} // namespace cpgf


