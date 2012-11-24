#include "test_variant_common.h"

#include "cpgf/gvariant.h"
#include "cpgf/gexception.h"
#include "cpgf/gmetaapi.h"

#include <iostream>

#define CAN_FROM(to, value) GCHECK(canFromVariant<to>(value)); fromVariant<to>(value)
#define CAN_FROM_CAST(to, from, value) GCHECK(canFromVariant<to>(static_cast<from>(value))); fromVariant<to>(static_cast<from>(value))
#define NOT_FROM(to, value) GCHECK(! canFromVariant<to>(value)); \
	GBEGIN_EXCEPTION fromVariant<to>(value); GEND_EXCEPTION(const GException &)
#define NOT_FROM_CAST(to, from, value) GCHECK(! canFromVariant<to>(static_cast<from>(value))); \
	GBEGIN_EXCEPTION fromVariant<to>(static_cast<from>(value)); GEND_EXCEPTION(const GException &)

#define CLASS TestClass_Variant

using namespace std;
using namespace cpgf;

namespace Test_Variant_Cast { namespace {

class CLASS
{
public:
	CLASS() {}
	CLASS(const CLASS &) {}
	CLASS(const string &) {}

	CLASS(int) {}
};

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

	CAN_FROM(void *, n);
	CAN_FROM(CLASS *, 0);
	NOT_FROM(CLASS *, 1);

	CAN_FROM_CAST(CLASS &, CLASS &, obj);
	NOT_FROM_CAST(int &, int &, n);

	CLASS * pobj = NULL;
	CLASS * & rpobj = pobj;
	CAN_FROM_CAST(CLASS *, CLASS * &, rpobj);
	CAN_FROM_CAST(CLASS * &, CLASS *, NULL);
	
	CAN_FROM_CAST(CLASS &, CLASS *, NULL);
	CAN_FROM_CAST(int &, void *, NULL);

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


GTEST(TestVariant_CastFromVariant)
{
	GVariant value;
	GVariant casted;
	
	value = 1999;

	casted = fromVariant<GVariant>(value);
	GEQUAL(1999, fromVariant<int>(casted));

	casted = fromVariant<GVariant &>(value);
	GEQUAL(1999, fromVariant<int>(casted));

	casted = fromVariant<const GVariant &>(value);
	GEQUAL(1999, fromVariant<int>(casted));
}


GTEST(TestVariant_CastFromFloat)
{
	GVariant value;
	float casted;

	float a = 5.0f;
	value = &a;
	value.refData().typeData.vt = byReference | vtFloat;

	GCHECK(value.getType() == (byReference | vtFloat));

	casted = (float)fromVariant<double>(value);
	GCHECK(casted > 4.9f && casted < 5.1f);

	int b = 3;
	value = &b;
	value.refData().typeData.vt = byReference | vtSignedInt;
	CAN_FROM(void *, value);
	CAN_FROM(const void *, value);
	CAN_FROM(const void * const, value);
}



} }
