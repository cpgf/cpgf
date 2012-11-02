#ifndef __GMETADEBUG_H
#define __GMETADEBUG_H


namespace cpgf {

class GMetaDebug
{
public:
	static void debugBreak();
	static void trace(const char * message);
	static void traceError(const char * message);
	static void traceLine(const char * message);
	static void traceErrorLine(const char * message);
};


} // namespace cpgf


#endif

