#include "test_variant_common.h"

#include "cpgf/gvariant.h"
#include "cpgf/gexception.h"
#include "cpgf/gmetaapi.h"

#include <string>
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
	GEQUAL(typeData.vt, ((int)GVariantType::vtObject | (int)GVariantType::byPointer | (int)GVariantType::byLvalueReference));

	CAN_FROM_CAST(CLASS &, CLASS &&, n);
	CAN_FROM_CAST(CLASS &&, CLASS &, obj);
	CAN_FROM_CAST(int &&, int, 0);
	CAN_FROM_CAST(int, int &&, 0);
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
	value.refData().typeData.vt = (uint16_t)GVariantType::byLvalueReference | (uint16_t)GVariantType::vtFloat;

	GCHECK((uint16_t)value.getType() == ((uint16_t)GVariantType::byLvalueReference | (uint16_t)GVariantType::vtFloat));

	casted = (float)fromVariant<double>(value);
	GCHECK(casted > 4.9f && casted < 5.1f);

	int b = 3;
	value = &b;
	value.refData().typeData.vt = (uint16_t)GVariantType::byLvalueReference | (uint16_t)GVariantType::vtSignedInt;
	CAN_FROM(void *, value);
	CAN_FROM(const void *, value);
	CAN_FROM(const void * const, value);
}


GTEST(TestVariant_CastFromArray)
{
	GVariant value;
	int a[] = { 1, 2, 3 };

	value = a;
	int * casted = fromVariant<int *>(value);
	GCHECK(casted[0] == 1);
	GCHECK(casted[1] == 2);
	GCHECK(casted[2] == 3);
}

// https://github.com/cpgf/cpgf/issues/42
GTEST(TestVariant_CastVtStringToConstStdStringReference)
{
	GVariant value = createStringVariant("abc");
	const std::string & stringReference = fromVariant<const std::string &>(value);
	const std::string * stringPointer = &stringReference;

	GCHECK(stringReference == "abc");
	GCHECK(*stringPointer == "abc");
}

GTEST(TestVariant_CastVtStringToStdStringReference)
{
	GVariant value = createStringVariant("abc");
	std::string & stringReference = fromVariant<std::string &>(value);
	std::string * stringPointer = &stringReference;

	GCHECK(stringReference == "abc");
	GCHECK(*stringPointer == "abc");
}

GTEST(TestVariant_CastVtStringToConstStdWideStringReference)
{
	GVariant value = createWideStringVariant(L"abc");
	const std::wstring & stringReference = fromVariant<const std::wstring &>(value);
	const std::wstring * stringPointer = &stringReference;

	GCHECK(stringReference == L"abc");
	GCHECK(*stringPointer == L"abc");
}

GTEST(TestVariant_CastVtStringToStdWideStringReference)
{
	GVariant value = createWideStringVariant(L"abc");
	std::wstring & stringReference = fromVariant<std::wstring &>(value);
	std::wstring * stringPointer = &stringReference;

	GCHECK(stringReference == L"abc");
	GCHECK(*stringPointer == L"abc");
}



} }
