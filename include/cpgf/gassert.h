#ifndef __GASSERT_H
#define __GASSERT_H

#include "cpgf/gpp.h"

#include <stdexcept>

#ifndef NDEBUG
#define GASSERT_ENABLED
#endif


namespace cpgf {

template <bool expr>
struct GStaticAssertCheck;

template<>
struct GStaticAssertCheck <true> { enum { a = 1 }; };

#define GASSERT_STATIC(expr)  enum GPP_CONCAT(gassert_static_, __LINE__) { GPP_CONCAT(gassert_static_v_, __LINE__) = sizeof(cpgf::GStaticAssertCheck<(bool)(expr)>) }

#ifdef GASSERT_ENABLED
	#define GASSERT(expr) gassert((bool)(expr), "assert failure", __FILE__, __LINE__);
	#define GASSERT_MSG(expr, msg) gassert((bool)(expr), msg, __FILE__, __LINE__);

	inline void gassert(bool expr, const char * msg, const char * file, int line) {
		(void)file;
		(void)line;
		if(!expr) {
			throw std::runtime_error(msg);
		}
	}
#else
	#define GASSERT(expr)
	#define GASSERT_MSG(expr, msg)
#endif


} // namespace cpgf


#endif
