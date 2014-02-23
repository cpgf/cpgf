#ifndef CPGF_GDEPRECATED_H
#define CPGF_GDEPRECATED_H

#include "cpgf/gcompiler.h"

#if defined(G_COMPILER_VC)
	#define G_DEPRECATED(func, msg) __declspec(deprecated(msg)) func
#elif defined(G_COMPILER_GCC)
	#define G_DEPRECATED(func, msg) func __attribute__ ((deprecated(msg)))
#else
	#define G_DEPRECATED(func, msg) func
#endif



#endif
