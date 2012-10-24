#include "test_reflection_common.h"

#define CLASS TestClass_Enum
#define NAME_CLASS GPP_STRINGIZE(CLASS)

#define ENUM(f) pointerAssign(en, metaClass->getEnum(# f))

using namespace std;
using namespace cpgf;

namespace Test_Enum { namespace {

class CLASS {
public:
	enum EnumFirst {
		ws1, ws2, ws3, ws4
	};

	enum EnumSecond {
		bs1 = 1, bs2 = 3, bs3 = 5, bs4 = 7, bs5 = 0x1fffffff,
	};


}; // class CLASS


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<CLASS>
		::define(NAME_CLASS)

		._enum<CLASS::EnumFirst>("EnumFirst")
			._element("ws1", CLASS::ws1)
			._element("ws2", CLASS::ws2)
			._element("ws3", CLASS::ws3)
			._element("ws4", CLASS::ws4)

		._enum<CLASS::EnumSecond>("EnumSecond")
			._element("bs1", CLASS::bs1)
			._element("bs2", CLASS::bs2)
			._element("bs3", CLASS::bs3)
			._element("bs4", CLASS::bs4)
			._element("bs5", CLASS::bs5)
	;
}


GTEST(Lib_Exists)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
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

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaEnum> en;

	ENUM(EnumFirst);
	GCHECK(en);

	ENUM(EnumSecond);
	GCHECK(en);
}


GTEST(Lib_GetCount)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
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

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaEnum> en;

	ENUM(EnumFirst);
	GEQUAL(en->getCount(), 4);

	ENUM(EnumSecond);
	GEQUAL(en->getCount(), 5);
}


GTEST(Lib_GetKey)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
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

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
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
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);

	const GMetaEnum * en;

	ENUM(EnumFirst);
	GEQUAL(fromVariant<int>(en->getValue(0)), CLASS::ws1);
	GEQUAL(fromVariant<int>(en->getValue(1)), CLASS::ws2);
	GEQUAL(fromVariant<int>(en->getValue(2)), CLASS::ws3);
	GEQUAL(fromVariant<int>(en->getValue(3)), CLASS::ws4);

	ENUM(EnumSecond);
	GEQUAL(fromVariant<int>(en->getValue(0)), CLASS::bs1);
	GEQUAL(fromVariant<int>(en->getValue(1)), CLASS::bs2);
	GEQUAL(fromVariant<int>(en->getValue(2)), CLASS::bs3);
	GEQUAL(fromVariant<int>(en->getValue(3)), CLASS::bs4);
	GEQUAL(fromVariant<long long>(en->getValue(4)), CLASS::bs5);
}


GTEST(API_GetValue)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaEnum> en;

	ENUM(EnumFirst);
	GEQUAL(fromVariant<int>(metaGetEnumValue(en.get(), 0)), CLASS::ws1);
	GEQUAL(fromVariant<int>(metaGetEnumValue(en.get(), 1)), CLASS::ws2);
	GEQUAL(fromVariant<int>(metaGetEnumValue(en.get(), 2)), CLASS::ws3);
	GEQUAL(fromVariant<int>(metaGetEnumValue(en.get(), 3)), CLASS::ws4);

	ENUM(EnumSecond);
	GEQUAL(fromVariant<int>(metaGetEnumValue(en.get(), 0)), CLASS::bs1);
	GEQUAL(fromVariant<int>(metaGetEnumValue(en.get(), 1)), CLASS::bs2);
	GEQUAL(fromVariant<int>(metaGetEnumValue(en.get(), 2)), CLASS::bs3);
	GEQUAL(fromVariant<int>(metaGetEnumValue(en.get(), 3)), CLASS::bs4);
	GEQUAL(fromVariant<long long>(metaGetEnumValue(en.get(), 4)), CLASS::bs5);
}


GTEST(Lib_FindKey)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
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

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
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

