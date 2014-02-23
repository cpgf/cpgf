#ifndef CPGF_GMETADEBUG_H
#define CPGF_GMETADEBUG_H


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
};


} // namespace cpgf


#endif

