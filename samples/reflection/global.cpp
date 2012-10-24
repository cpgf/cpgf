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
	testCheckAssert(metaClass != NULL);
	std::cout << "Lib: " << metaClass->getName() << std::endl;

	{
		width = 0;
		field = metaClass->getField("width"); testCheckAssert(field != NULL);
		field->set(NULL, 123);
		testCheckEqual(width, 123);
		testCheckEqual(width, fromVariant<int>(field->get(NULL)));
	}

	{
		name = "";
		field = metaClass->getField("name"); testCheckAssert(field != NULL);
		field->set(NULL, "new name");
		testCheckStringEqual(name, "new name");
		std::string s = fromVariant<std::string>(field->get(NULL));
		testCheckStringEqual(name, s);
	}

	{
		field = metaClass->getField("data"); testCheckAssert(field != NULL);
		TestData localdata(3, "Fake dummy data");
		field->set(NULL, localdata);
		testCheckEqual(data, localdata);
		testCheckEqual(data, fromVariant<TestData>(field->get(NULL)));
	}

	{
		stats = 0;
		field = metaClass->getField("stats"); testCheckAssert(field != NULL);
		field->set(NULL, 567);
		testCheckEqual(stats, 567);
		testCheckEqual(stats, fromVariant<int>(field->get(NULL)));
	}

	{
		method = metaClass->getMethod("addNumber"); testCheckAssert(method != NULL);
		int n = fromVariant<int>(method->invoke(NULL, 38888, 16888));
		testCheckEqual(n, addNumber(38888, 16888));
	}


}

void doTestAPI()
{
	using namespace cpgf;

	GScopedInterface<IMetaService> service(createDefaultMetaService());
	testCheckAssert(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	testCheckAssert(metaClass);
	std::cout << "API: " << metaClass->getName() << std::endl;

	GScopedInterface<IMetaField> field;
	GScopedInterface<IMetaMethod> method;

	{
		width = 0;
		field.reset(metaClass->getField("width")); testCheckAssert(field);
		metaSetValue(field.get(), NULL, 123);
		testCheckEqual(width, 123);
		testCheckEqual(width, fromVariant<int>(metaGetValue(field.get(), NULL)));
	}

	{
		name = "";
		field.reset(metaClass->getField("name")); testCheckAssert(field);
		metaSetValue(field.get(), NULL, "new name");
		testCheckStringEqual(name, "new name");
		std::string s = fromVariant<std::string>(metaGetValue(field.get(), NULL));
		testCheckStringEqual(name, s);
	}

	{
		field.reset(metaClass->getField("data")); testCheckAssert(field);
		TestData localdata(3, "Fake dummy data");
		metaSetValue(field.get(), NULL, localdata);
		testCheckEqual(data, localdata);
		testCheckEqual(data, fromVariant<TestData>(metaGetValue(field.get(), NULL)));
	}

	{
		stats = 0;
		field.reset(metaClass->getField("stats")); testCheckAssert(field);
		metaSetValue(field.get(), NULL, 567);
		testCheckEqual(stats, 567);
		testCheckEqual(stats, fromVariant<int>(metaGetValue(field.get(), NULL)));
	}

	{
		method.reset(metaClass->getMethod("addNumber")); testCheckAssert(method);
		int n = fromVariant<int>(metaInvokeMethod(method.get(), NULL, 38888, 16888));
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
