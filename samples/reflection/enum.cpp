#include "reflect_common.h"



namespace {

class TestObject
{
public:
	enum WindowStyle {
		ws0, ws1, ws2, ws3,
	};

	enum BorderStyle {
		bs0 = 1, bs1 = 3, bs2 = 5, bs3 = 7, bs4 = 9,
	};

public:
};

GMETA_DEFINE_CLASS(TestObject, TestObject, "enum::TestObject") {
	using namespace cpgf;

	GMETA_ENUM(WindowStyle, TestObject::ws0, TestObject::ws1, TestObject::ws2, TestObject::ws3);
	reflectEnum<TestObject::BorderStyle>("BorderStyle")
		("bs0", TestObject::bs0)
		("bs1", TestObject::bs1)
		("bs2", TestObject::bs2)
		("bs3", TestObject::bs3)
		("bs4", TestObject::bs4)
	;
}

void doTestLib()
{
	using namespace cpgf;

	const GMetaClass * metaClass;
	const GMetaEnum * metaEnum;

	metaClass = findMetaClass("enum::TestObject");
	testCheckAssert(metaClass != NULL);
	std::cout << "Lib: " << metaClass->getName() << std::endl;

	{
		metaEnum = metaClass->getEnum("WindowStyle"); testCheckAssert(metaEnum != NULL);
		checkEnum(metaEnum, "ws");
	}

	{
		metaEnum = metaClass->getEnum("BorderStyle"); testCheckAssert(metaEnum != NULL);
		checkEnum(metaEnum, "bs");
	}


}

void doTestAPI()
{
	using namespace cpgf;

	GApiScopedPointer<IMetaService> service(createDefaultMetaService());
	testCheckAssert(service);

	GApiScopedPointer<IMetaClass> metaClass(service->findClassByName("enum::TestObject"));
	testCheckAssert(metaClass);

	std::cout << "API: " << metaClass->getName() << std::endl;

	GApiScopedPointer<IMetaEnum> metaEnum;

	{
		metaEnum.reset(metaClass->getEnum("WindowStyle")); testCheckAssert(metaEnum);
		checkEnum(metaEnum, "ws");
	}

	{
		metaEnum.reset(metaClass->getEnum("BorderStyle")); testCheckAssert(metaEnum);
		checkEnum(metaEnum, "bs");
	}


}


void doTest()
{
	doTestLib();
	doTestAPI();
}


gTestCase(doTest);


}
