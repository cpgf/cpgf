#ifndef __GCOMPILER_H
#define __GCOMPILER_H


#if !defined(NDEBUG)
	#define G_DEBUG
#else
	#undef G_DEBUG
#endif


#if defined(_WIN32) || defined(WIN32) || defined(__WIN32__) || defined(__WINDOWS__) || defined(_WIN32_WCE)
	#define G_OS_WIN32
#else
	#define G_OS_IPHONE
#endif

#if defined(_MSC_VER)
	#define G_CPP_VC
#elif defined(__BORLANDC__)
	#define G_CPP_BCB
#elif defined(__GNUC__)
	#define G_CPP_GCC
#endif

#ifdef G_OS_IPHONE
	#define G_OPENGLES
#endif

#if defined(G_OS_WIN32) || defined(G_OS_WIN64)
	#define G_OS_WIN
#endif

#ifdef G_CPP_VC
//	#define override override
#else
	#define override
#endif



#endif
