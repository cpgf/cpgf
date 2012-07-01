#ifndef __GDEBUG_H
#define __GDEBUG_H

#include "cpgf/gcompiler.h"

#ifdef G_FORCE_DEBUG
	#ifndef G_DEBUG
		#define G_DEBUG
	#endif
#endif


#ifdef G_DEBUG

	#define debugTrace(message, ...) cpgf::_internal::DebugTrace(__FILE__, __LINE__, message, ##__VA_ARGS__)
	#define debugAssert(expression, message, ...) (void)((expression) || cpgf::_internal::DebugAssert(__FILE__, __LINE__, # expression, message, ##__VA_ARGS__))
	#define debugPrintMatrix(m) cpgf::_internal::DebugOutputMatrixColumnMajor(m)
	#define debugCheckGL() cpgf::_internal::DebugCheckGL()

#else

	#define debugTrace(msg, ...)
	#define debugAssert(expression, message, ...)
	#define debugPrintMatrix(m)
	#define debugCheckGL()

#endif

#define debugPlaceholder() debugTrace(NULL)

namespace cpgf {


//#ifdef G_DEBUG

namespace _internal {
	void DebugTrace(const char* sourceFile, int lineNumber, const char *message, ...);
	bool DebugAssert(const char* sourceFile, int lineNumber, const char *expression, const char *message, ...);
	void DebugOutputMatrixColumnMajor(float *matrix);
	void DebugCheckGL();
}

//#endif


} // namespace cpgf

#endif

