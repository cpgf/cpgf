// Auto generated test code
// Don't modify manually.

#define AUTOGEN_TEST
#include "test_reflection_common.h"

#define CLASS void
#define NAME_CLASS GPP_STRINGIZE(CLASS)

#define ENUM(f) pointerAssign(en, metaClass->getEnum(# f))

using namespace std;
using namespace cpgf;

namespace Test_GlobalEnum { namespace {



	enum EnumFirst {
		ws1, ws2, ws3, ws4
	};

	enum EnumSecond {
		bs1 = 1, bs2 = 3, bs3 = 5, bs4 = 7, bs5 = 0x1fffffff,
	};





G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaGlobal()
		

		._enum<EnumFirst>("EnumFirst")
			._element("ws1", ws1)
			._element("ws2", ws2)
			._element("ws3", ws3)
			._element("ws4", ws4)

		._enum<EnumSecond>("EnumSecond")
			._element("bs1", bs1)
			._element("bs2", bs2)
			._element("bs3", bs3)
			._element("bs4", bs4)
			._element("bs5", bs5)
	;
}


GTEST(Lib_Exists)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);

	const GMetaEnum * en;

	ENUM(EnumFirst);
	GCHECK(en);

	ENUM(EnumSecond);
	GCHECK(en);
}


GTEST(API_Exists)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	GCHECK(metaClass);

	GScopedInterface<IMetaEnum> en;

	ENUM(EnumFirst);
	GCHECK(en);

	ENUM(EnumSecond);
	GCHECK(en);
}


GTEST(Lib_GetCount)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);

	const GMetaEnum * en;

	ENUM(EnumFirst);
	GEQUAL(en->getCount(), 4);

	ENUM(EnumSecond);
	GEQUAL(en->getCount(), 5);
}


GTEST(API_GetCount)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	GCHECK(metaClass);

	GScopedInterface<IMetaEnum> en;

	ENUM(EnumFirst);
	GEQUAL(en->getCount(), 4);

	ENUM(EnumSecond);
	GEQUAL(en->getCount(), 5);
}


GTEST(Lib_GetKey)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);

	const GMetaEnum * en;

	ENUM(EnumFirst);
	GEQUAL(en->getKey(0), string("ws1"));
	GEQUAL(en->getKey(1), string("ws2"));
	GEQUAL(en->getKey(2), string("ws3"));
	GEQUAL(en->getKey(3), string("ws4"));
	GEQUAL(en->getKey(5), NULL);

	ENUM(EnumSecond);
	GEQUAL(en->getKey(0), string("bs1"));
	GEQUAL(en->getKey(1), string("bs2"));
	GEQUAL(en->getKey(2), string("bs3"));
	GEQUAL(en->getKey(3), string("bs4"));
	GEQUAL(en->getKey(4), string("bs5"));
	GEQUAL(en->getKey(5), NULL);
}


GTEST(API_GetKey)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	GCHECK(metaClass);

	GScopedInterface<IMetaEnum> en;

	ENUM(EnumFirst);
	GEQUAL(en->getKey(0), string("ws1"));
	GEQUAL(en->getKey(1), string("ws2"));
	GEQUAL(en->getKey(2), string("ws3"));
	GEQUAL(en->getKey(3), string("ws4"));
	GEQUAL(en->getKey(5), NULL);

	ENUM(EnumSecond);
	GEQUAL(en->getKey(0), string("bs1"));
	GEQUAL(en->getKey(1), string("bs2"));
	GEQUAL(en->getKey(2), string("bs3"));
	GEQUAL(en->getKey(3), string("bs4"));
	GEQUAL(en->getKey(4), string("bs5"));
	GEQUAL(en->getKey(5), NULL);
}


GTEST(Lib_GetValue)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);

	const GMetaEnum * en;

	ENUM(EnumFirst);
	GEQUAL(fromVariant<int>(en->getValue(0)), ws1);
	GEQUAL(fromVariant<int>(en->getValue(1)), ws2);
	GEQUAL(fromVariant<int>(en->getValue(2)), ws3);
	GEQUAL(fromVariant<int>(en->getValue(3)), ws4);

	ENUM(EnumSecond);
	GEQUAL(fromVariant<int>(en->getValue(0)), bs1);
	GEQUAL(fromVariant<int>(en->getValue(1)), bs2);
	GEQUAL(fromVariant<int>(en->getValue(2)), bs3);
	GEQUAL(fromVariant<int>(en->getValue(3)), bs4);
	GEQUAL(fromVariant<long long>(en->getValue(4)), bs5);
}


GTEST(API_GetValue)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	GCHECK(metaClass);

	GScopedInterface<IMetaEnum> en;

	ENUM(EnumFirst);
	GEQUAL(fromVariant<int>(metaGetEnumValue(en.get(), 0)), ws1);
	GEQUAL(fromVariant<int>(metaGetEnumValue(en.get(), 1)), ws2);
	GEQUAL(fromVariant<int>(metaGetEnumValue(en.get(), 2)), ws3);
	GEQUAL(fromVariant<int>(metaGetEnumValue(en.get(), 3)), ws4);

	ENUM(EnumSecond);
	GEQUAL(fromVariant<int>(metaGetEnumValue(en.get(), 0)), bs1);
	GEQUAL(fromVariant<int>(metaGetEnumValue(en.get(), 1)), bs2);
	GEQUAL(fromVariant<int>(metaGetEnumValue(en.get(), 2)), bs3);
	GEQUAL(fromVariant<int>(metaGetEnumValue(en.get(), 3)), bs4);
	GEQUAL(fromVariant<long long>(metaGetEnumValue(en.get(), 4)), bs5);
}


GTEST(Lib_FindKey)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);

	const GMetaEnum * en;

	ENUM(EnumFirst);
	GEQUAL(en->findKey("ws1"), 0);
	GEQUAL(en->findKey("ws2"), 1);
	GEQUAL(en->findKey("ws3"), 2);
	GEQUAL(en->findKey("ws4"), 3);
	GEQUAL(en->findKey("no"), -1);

	ENUM(EnumSecond);
	GEQUAL(en->findKey("bs1"), 0);
	GEQUAL(en->findKey("bs2"), 1);
	GEQUAL(en->findKey("bs3"), 2);
	GEQUAL(en->findKey("bs4"), 3);
	GEQUAL(en->findKey("bs5"), 4);
	GEQUAL(en->findKey("no"), -1);
}


GTEST(API_FindKey)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	GCHECK(metaClass);

	GScopedInterface<IMetaEnum> en;

	ENUM(EnumFirst);
	GEQUAL(en->findKey("ws1"), 0);
	GEQUAL(en->findKey("ws2"), 1);
	GEQUAL(en->findKey("ws3"), 2);
	GEQUAL(en->findKey("ws4"), 3);
	GEQUAL(en->findKey("no"), -1);

	ENUM(EnumSecond);
	GEQUAL(en->findKey("bs1"), 0);
	GEQUAL(en->findKey("bs2"), 1);
	GEQUAL(en->findKey("bs3"), 2);
	GEQUAL(en->findKey("bs4"), 3);
	GEQUAL(en->findKey("bs5"), 4);
	GEQUAL(en->findKey("no"), -1);
}





} }

