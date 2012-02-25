#include "test_reflection_common.h"

#include "cpgf/gmetatype.h"

#include <typeinfo>
#include <iostream>


#define MT(t) createMetaType<t>()
#define MT_EQUAL(a, b) GEQUAL(createMetaType<a>(), createMetaType<b>())
#define MT_DIFF(a, b) GDIFF(createMetaType<a>(), createMetaType<b>())

using namespace std;
using namespace cpgf;

namespace Test_MetaType { namespace {

GTEST(TestMetaType)
{
	MT_EQUAL(int, int);
	MT_EQUAL(int, int &);
	MT_EQUAL(int, const int &);
	MT_EQUAL(int, volatile int &);
	MT_EQUAL(int, const volatile int &);
	
	MT_EQUAL(int *, int *);
	MT_EQUAL(int **, int **);
	MT_EQUAL(int ***, int ***);

	MT_DIFF(int, long);

	MT_DIFF(int, int *);
	MT_DIFF(int *, const int *);
	MT_DIFF(int *, volatile int *);
	MT_DIFF(int *, const volatile int *);
	MT_DIFF(const int *, const volatile int *);
	MT_DIFF(volatile int *, const volatile int *);
	MT_DIFF(const int *, volatile int *);

	GCHECK(! MT(int).isConst());
	GCHECK(! MT(int).isVolatile());
	GCHECK(! MT(int).isConstVolatile());
	GCHECK(MT(const int).isConst());
	GCHECK(MT(volatile int).isVolatile());
	GCHECK(MT(const volatile int).isConstVolatile());

	GCHECK(MT(int *).isPointer());
	GCHECK(MT(const int *).isPointer());
	GCHECK(MT(const volatile int *).isPointer());
	GCHECK(MT(int * const).isPointer());
	GCHECK(MT(const int * const).isPointer());
	GCHECK(MT(const volatile int * volatile).isPointer());

	GCHECK(MT(int &).isReference());
	GCHECK(MT(const int &).isReference());
	GCHECK(MT(const volatile int &).isReference());
	GCHECK(MT(int * &).isReference());
	GCHECK(MT(int * &).isPointer());

	GCHECK(MT(const int *).isPointerToConst());
	GCHECK(MT(volatile int *).isPointerToVolatile());
	GCHECK(MT(const volatile int *).isPointerToConstVolatile());

}





} }

