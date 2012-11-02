#include "cpgf/metautility/gmetadebug.h"
#include "cpgf/gcompiler.h"

#include <stdio.h>


namespace cpgf {


void GMetaDebug::debugBreak()
{
#if defined(__i386) || defined(_M_IX86)
	#if defined(G_COMPILER_VC)
		__asm int 3;
	#elif defined(G_COMPILER_GCC)
		__asm__("int $0x03");
	#endif
#else
#endif
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


