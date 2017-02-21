#ifndef CPGF_UNITTESTBASE_H
#define CPGF_UNITTESTBASE_H


#include "UnitTest++.h"

#include "cpgf/gpp.h"
#include "cpgf/gtypetraits.h"
#include "cpgf/genableif.h"

#include <string>


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4800) // forcing value to bool 'true' or 'false' (performance warning)
#endif


#define GTEST TEST

#define GFAIL(...) CHECK(false)

#define GCHECK(...) CHECK(bool(__VA_ARGS__))

#define GEQUAL(a, b) GCHECK(a == b)

#define GEQUAL_NOTNULL(a, b) GCHECK(a); GCHECK(a == b)

#define GDIFF(a, b) GCHECK(!(a == b))

#define GNEAR(a, b, t) CHECK_CLOSE(a, b, t)

#define GBEGIN_EXCEPTION try {
#define GEND_EXCEPTION(e) GCHECK(false); } catch(e) {}

#define FEQUAL(a, b) (fabs((a) - (b)) < 0.001)
#define GFEQUAL(a, b) GNEAR(a, b, 0.001)

namespace cpgf {


template <typename T, typename V>
void pointerAssign(T & p, const V & value, typename GEnableIf<IsPointer<T>::Result>::Result * = 0)
{
	p = value;
}

template <typename T, typename V>
void pointerAssign(T & p, const V & value, typename GDisableIf<IsPointer<T>::Result>::Result * = 0)
{
	p.reset(value);
}



} // namespace cpgf


#endif


