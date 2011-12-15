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


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<TestObject>
		::define("enum::TestObject")

		._enum<TestObject::WindowStyle>("WindowStyle")
			._element("ws0", TestObject::ws0)
			._element("ws1", TestObject::ws1)
			._element("ws2", TestObject::ws2)
			._element("ws3", TestObject::ws3)

		._enum<TestObject::BorderStyle>("BorderStyle")
			._element("bs0", TestObject::bs0)
			._element("bs1", TestObject::bs1)
			._element("bs2", TestObject::bs2)
			._element("bs3", TestObject::bs3)
			._element("bs4", TestObject::bs4)

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

	GScopedInterface<IMetaService> service(createDefaultMetaService());
	testCheckAssert(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("enum::TestObject"));
	testCheckAssert(metaClass);

	std::cout << "API: " << metaClass->getName() << std::endl;

	GScopedInterface<IMetaEnum> metaEnum;

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
