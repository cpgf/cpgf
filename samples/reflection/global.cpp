#include "reflect_common.h"



namespace {

int width;
std::string name;
TestData data;
int stats;

int addNumber(int n, int delta)
{
	return n + delta;
}


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaGlobal()
		._field("width", &width)
		._field("name", &name)
		._field("data", &data)
		._field("stats", &stats)
		._method("addNumber", &addNumber)
	;
}


void doTestLib()
{
	using namespace cpgf;

	const GMetaClass * metaClass;
	const GMetaField * field;
	const GMetaMethod * method;

	metaClass = getGlobalMetaClass();
	testCheckAssert(metaClass != nullptr);
	std::cout << "Lib: " << metaClass->getName() << std::endl;

	{
		width = 0;
		field = metaClass->getField("width"); testCheckAssert(field != nullptr);
		field->set(nullptr, 123);
		testCheckEqual(width, 123);
		testCheckEqual(width, fromVariant<int>(field->get(nullptr)));
	}

	{
		name = "";
		field = metaClass->getField("name"); testCheckAssert(field != nullptr);
		field->set(nullptr, "new name");
		testCheckStringEqual(name, "new name");
		std::string s = fromVariant<std::string>(field->get(nullptr));
		testCheckStringEqual(name, s);
	}

	{
		field = metaClass->getField("data"); testCheckAssert(field != nullptr);
		TestData localdata(3, "Fake dummy data");
		field->set(nullptr, localdata);
		testCheckEqual(data, localdata);
		testCheckEqual(data, fromVariant<TestData>(field->get(nullptr)));
	}

	{
		stats = 0;
		field = metaClass->getField("stats"); testCheckAssert(field != nullptr);
		field->set(nullptr, 567);
		testCheckEqual(stats, 567);
		testCheckEqual(stats, fromVariant<int>(field->get(nullptr)));
	}

	{
		method = metaClass->getMethod("addNumber"); testCheckAssert(method != nullptr);
		int n = fromVariant<int>(method->invoke(nullptr, 38888, 16888));
		testCheckEqual(n, addNumber(38888, 16888));
	}


}

void doTestAPI()
{
	using namespace cpgf;

	GScopedInterface<IMetaService> service(createDefaultMetaService());
	testCheckAssert((bool)service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	testCheckAssert((bool)metaClass);
	std::cout << "API: " << metaClass->getName() << std::endl;

	GScopedInterface<IMetaField> field;
	GScopedInterface<IMetaMethod> method;

	{
		width = 0;
		field.reset(metaClass->getField("width")); testCheckAssert((bool)field);
		metaSetValue(field.get(), nullptr, 123);
		testCheckEqual(width, 123);
		testCheckEqual(width, fromVariant<int>(metaGetValue(field.get(), nullptr)));
	}

	{
		name = "";
		field.reset(metaClass->getField("name")); testCheckAssert((bool)field);
		metaSetValue(field.get(), nullptr, "new name");
		testCheckStringEqual(name, "new name");
		std::string s = fromVariant<std::string>(metaGetValue(field.get(), nullptr));
		testCheckStringEqual(name, s);
	}

	{
		field.reset(metaClass->getField("data")); testCheckAssert((bool)field);
		TestData localdata(3, "Fake dummy data");
		metaSetValue(field.get(), nullptr, localdata);
		testCheckEqual(data, localdata);
		testCheckEqual(data, fromVariant<TestData>(metaGetValue(field.get(), nullptr)));
	}

	{
		stats = 0;
		field.reset(metaClass->getField("stats")); testCheckAssert((bool)field);
		metaSetValue(field.get(), nullptr, 567);
		testCheckEqual(stats, 567);
		testCheckEqual(stats, fromVariant<int>(metaGetValue(field.get(), nullptr)));
	}

	{
		method.reset(metaClass->getMethod("addNumber")); testCheckAssert((bool)method);
		int n = fromVariant<int>(metaInvokeMethod(method.get(), nullptr, 38888, 16888));
		testCheckEqual(n, addNumber(38888, 16888));
	}


}



void doTest()
{
	doTestLib();
	doTestAPI();
}

gTestCase(doTest);



}
