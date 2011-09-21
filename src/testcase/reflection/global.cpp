#include "test_common.h"



namespace {

int width;
std::string name;
TestData data;
int stats;

int addNumber(int n, int delta)
{
	return n + delta;
}

GMETA_DEFINE_GLOBAL() {
	using namespace cpgf;

	GMETA_QUALIFIED_FIELD(width);
	GMETA_QUALIFIED_FIELD(name);
	GMETA_QUALIFIED_FIELD(data);

	GMETA_QUALIFIED_FIELD(stats);

	GMETA_QUALIFIED_METHOD(addNumber);
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

	GMetaScopedPointer<IMetaService> service(createMetaService());
	testCheckAssert(service);

	GMetaScopedPointer<IMetaClass> metaClass(service->getGlobalMetaClass());
	testCheckAssert(metaClass);
	std::cout << "API: " << metaClass->getName() << std::endl;

	GMetaScopedPointer<IMetaField> field;
	GMetaScopedPointer<IMetaMethod> method;

	{
		width = 0;
		field.reset(metaClass->getField("width")); testCheckAssert(field);
		metaSetValue(field, NULL, 123);
		testCheckEqual(width, 123);
		testCheckEqual(width, fromVariant<int>(metaGetValue(field, NULL)));
	}

	{
		name = "";
		field.reset(metaClass->getField("name")); testCheckAssert(field);
		metaSetValue(field, NULL, "new name");
		testCheckStringEqual(name, "new name");
		std::string s = fromVariant<std::string>(metaGetValue(field, NULL));
		testCheckStringEqual(name, s);
	}

	{
		field.reset(metaClass->getField("data")); testCheckAssert(field);
		TestData localdata(3, "Fake dummy data");
		metaSetValue(field, NULL, localdata);
		testCheckEqual(data, localdata);
		testCheckEqual(data, fromVariant<TestData>(metaGetValue(field, NULL)));
	}

	{
		stats = 0;
		field.reset(metaClass->getField("stats")); testCheckAssert(field);
		metaSetValue(field, NULL, 567);
		testCheckEqual(stats, 567);
		testCheckEqual(stats, fromVariant<int>(metaGetValue(field, NULL)));
	}

	{
		method.reset(metaClass->getMethod("addNumber")); testCheckAssert(method);
		int n = fromVariant<int>(metaCallMethod(method, NULL, 38888, 16888));
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
