#ifndef CPGF_GCOMPILER_H
#define CPGF_GCOMPILER_H

#include "cpgf/gconfig.h"


#undef G_DEBUG

#if !defined(NDEBUG)
	#define G_DEBUG 1
#else
	#define G_DEBUG 0
#endif

#if defined(_WIN64) || defined(WIN64) || defined(__WIN64_) || defined(_WIN64_WCE)
	#define G_OS_WIN64
#elif defined(_WIN32) || defined(WIN32) || defined(__WIN32__) || defined(_WIN32_WCE)
	#define G_OS_WIN32
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__) 
	#define G_OS_LINUX
#elif defined(__APPLE__)
	#define G_OS_APPLE
#else
	#define G_OS_UNKNOWN
#endif

#if defined(_MSC_VER)
	#define G_COMPILER_VC
#elif defined(__BORLANDC__)
	#define G_COMPILER_CPPBUILDER
#elif defined(__CODEGEARC__)
	#define G_COMPILER_CPPBUILDER
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

    #ifdef G_COMPILER_CPPBUILDER
		#undef G_SUPPORT_STDCALL
		#undef G_SUPPORT_FASTCALL
    #endif

    #ifdef G_OS_WIN64
		#undef G_SUPPORT_STDCALL
		#undef G_SUPPORT_FASTCALL
    #endif
#endif    

#ifndef G_API_CC
	#ifdef G_SUPPORT_STDCALL
		#define G_API_CC __stdcall
	#else
		#define G_API_CC
	#endif
#endif

#ifdef G_COMPILER_CPPBUILDER
	#define G_NO_MEMBER_TEMPLATE_FRIENDS
#endif

#define G_STATIC_CONSTANT(type, assignment) static const type assignment

#ifdef __clang__
    #define G_SUPPORT_NORETURN_ATTRIBUTE
#endif

#ifndef __has_builtin
    #define __has_builtin(x) 0  // Compatibility with non-clang compilers.
#endif

#if __has_builtin(__builtin_trap)
    #define abort_trap __builtin_trap
#else
    #define abort_trap abort
#endif


#endif
