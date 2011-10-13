#ifndef __GCOMPILER_H
#define __GCOMPILER_H

#include "cpgf/gconfig.h"


#if !defined(NDEBUG)
	#define G_DEBUG
#else
	#undef G_DEBUG
#endif


#if defined(_WIN32) || defined(WIN32) || defined(__WIN32__) || defined(__WINDOWS__) || defined(_WIN32_WCE)
	#define G_OS_WIN32
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__) 
	#define G_OS_LINUX
#else
	#define G_OS_UNKNOWN
#endif

#if defined(_MSC_VER)
	#define G_COMPILER_VC
#elif defined(__BORLANDC__)
	#define G_COMPILER_BCB
#elif defined(__GNUC__)
	#define G_COMPILER_GCC
#else
	#define G_COMPILER_UNKNOWN	
#endif

#if defined(G_OS_WIN32) || defined(G_OS_WIN64)
	#define G_OS_WIN
#endif

#ifdef G_OS_WIN
	#define G_SUPPORT_STDCALL
	#define G_SUPPORT_FASTCALL
	
    #ifdef __GNUC__
        #if (__GNUC__ < 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ < 5))
			#undef G_SUPPORT_STDCALL
			#undef G_SUPPORT_FASTCALL
        #endif
    #endif
#endif    

#ifndef G_API_CC
	#ifdef G_SUPPORT_STDCALL
		#define G_API_CC __stdcall
	#else
		#define G_API_CC
	#endif
#endif



#endif
