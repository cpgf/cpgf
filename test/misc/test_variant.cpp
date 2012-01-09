#include "test_misc_common.h"

#include "cpgf/gvariant.h"
#include "cpgf/gexception.h"
#include "cpgf/gmetaapi.h"

#define CAN_FROM(to, value) GCHECK(canFromVariant<to>(value)); fromVariant<to>(value)
#define CAN_FROM_CAST(to, from, value) GCHECK(canFromVariant<to>(static_cast<from>(value))); fromVariant<to>(static_cast<from>(value))
#define NOT_FROM(to, value) GCHECK(! canFromVariant<to>(value)); \
	GBEGIN_EXCEPTION fromVariant<to>(value); GEND_EXCEPTION(const GException &)
#define NOT_FROM_CAST(to, from, value) GCHECK(! canFromVariant<to>(static_cast<from>(value))); \
	GBEGIN_EXCEPTION fromVariant<to>(static_cast<from>(value)); GEND_EXCEPTION(const GException &)

#define CLASS TestClass_Variant

using namespace std;
using namespace cpgf;

namespace Test_Variant { namespace {

class CLASS
{
public:
	CLASS() {}
	CLASS(const CLASS &) {}
	CLASS(const string &) {}

	CLASS(int) {}
};

class ClassB
{
public:
	static ClassB * lastB;

public:
	ClassB() { lastB = this; }

	ClassB(const ClassB &) {
	}

	ClassB & operator = (const ClassB &) {
		return *this;
	}
};
ClassB * ClassB::lastB = NULL;

GTEST(TestVariant_Cast)
{
	CAN_FROM(int, 0);
	CAN_FROM(string, "abc");
	CAN_FROM(double, 1999ul);

	NOT_FROM(string, 0);
	NOT_FROM(int, "a");

	CAN_FROM(CLASS, 5);
	CAN_FROM(CLASS, string(""));
	NOT_FROM(CLASS, "");

	CAN_FROM_CAST(void *, int *, NULL);
	CAN_FROM_CAST(void *, int **, NULL);
	CAN_FROM_CAST(void *, CLASS *, NULL);
	CAN_FROM_CAST(int *, void *, NULL);
	CAN_FROM_CAST(int **, void *, NULL);
	CAN_FROM_CAST(CLASS *, void *, NULL);

	GVariant v;

	v = createStringVariant("abc");
	CAN_FROM(string, v);
	CAN_FROM(char *, v);
	CAN_FROM(const char *, v);

	CLASS obj;
	int n = 0;

	CAN_FROM_CAST(CLASS &, CLASS &, obj);
	NOT_FROM_CAST(int &, int &, n);

	CLASS * pobj = NULL;
	CLASS * & rpobj = pobj;
	CAN_FROM_CAST(CLASS *, CLASS * &, rpobj);
	CAN_FROM_CAST(CLASS * &, CLASS *, NULL);

	GVarTypeData typeData = GVarTypeData();
	deduceVariantType<CLASS * & >(typeData, true);
	GEQUAL(typeData.vt, (vtPointer | byReference));

#if G_SUPPORT_RVALUE_REFERENCE
	CAN_FROM_CAST(CLASS &, CLASS &&, n);
	CAN_FROM_CAST(CLASS &&, CLASS &, obj);
	CAN_FROM_CAST(int &&, int, 0);
	CAN_FROM_CAST(int, int &&, 0);
#endif
}

GTEST(TestVariant_ConstReference)
{
	const ClassB & a = fromVariant<const ClassB &, VarantCastCopyConstRef>(createVariant<false>(ClassB()));
	GDIFF(&a, ClassB::lastB);

	const ClassB & b = fromVariant<const ClassB &, VarantCastKeepConstRef>(createVariant<false>(ClassB()));
	GEQUAL(&b, ClassB::lastB);
}

GTEST(TestVariant_ObjectPointer)
{
	GEQUAL(vtGetBaseType(GVariant((CLASS *)0).data.typeData), vtObject);
	GEQUAL(vtGetPointers(GVariant((CLASS *)0).data.typeData), 1);

	GEQUAL(vtGetBaseType(GVariant((CLASS const *)0).data.typeData), vtObject);
	GEQUAL(vtGetPointers(GVariant((CLASS const *)0).data.typeData), 1);

	GEQUAL(vtGetBaseType(GVariant((CLASS volatile *)0).data.typeData), vtObject);
	GEQUAL(vtGetPointers(GVariant((CLASS volatile *)0).data.typeData), 1);

	GEQUAL(vtGetBaseType(GVariant((CLASS const volatile *)0).data.typeData), vtObject);
	GEQUAL(vtGetPointers(GVariant((CLASS const volatile *)0).data.typeData), 1);

	GEQUAL(vtGetBaseType(GVariant((CLASS **)0).data.typeData), vtObject);
	GEQUAL(vtGetPointers(GVariant((CLASS **)0).data.typeData), 2);

	GEQUAL(vtGetBaseType(GVariant((IMetaClass *)0).data.typeData), vtInterface);
	GEQUAL(vtGetPointers(GVariant((IMetaClass *)0).data.typeData), 1);

	GEQUAL(vtGetBaseType(GVariant((const IMetaClass * const volatile)0).data.typeData), vtInterface);
	GEQUAL(vtGetPointers(GVariant((const IMetaClass * const volatile)0).data.typeData), 1);

	GEQUAL(vtGetBaseType(GVariant((const volatile IMetaClass *)0).data.typeData), vtInterface);
	GEQUAL(vtGetPointers(GVariant((const volatile IMetaClass *)0).data.typeData), 1);
}



} }
