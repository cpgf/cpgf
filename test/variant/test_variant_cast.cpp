#include "test_variant_common.h"

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




} }
