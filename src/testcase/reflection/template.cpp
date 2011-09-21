#include "test_common.h"



namespace {

template <typename T>
class TestBase
{
public:
	virtual ~TestBase() {
	}

	int getDouble() const {
		return sizeof(T) * 2;
	}

	int value;
};

template <typename T, typename P>
class TestObject : public TestBase<T>
{
public:
	int getSize() const {
		return sizeof(T);
	}

	int n;
};

GMETA_DEFINE_TEMPLATE(
		TestObject,
		TestObject,
		typename T GPP_COMMA() typename P,
		T GPP_COMMA() P,
		"",
		TestBase<T> ) {

	using namespace cpgf;

	GMETA_METHOD(getSize);
	GMETA_METHOD(getDouble);
}

GMETA_INSTANTIATE_TEMPLATE("template::TestObject_char_int", TestObject, char GPP_COMMA() int);



void doTest()
{
	using namespace cpgf;

	const GMetaClass * metaClass;
	const GMetaMethod * method;

	metaClass = findMetaClass("template::TestObject_char_int");
	testCheckAssert(metaClass != NULL);
	std::cout << metaClass->getName() << std::endl;

	{
		void * obj = metaClass->createInstance();
		method = metaClass->getMethodInHierarchy("getSize", &obj); testCheckAssert(method != NULL);
		testCheckEqual(fromVariant<unsigned int>(method->invoke(obj)), sizeof(char));
		metaClass->destroyInstance(obj);
	}

	{
		void * obj = metaClass->createInstance();
		method = metaClass->getMethodInHierarchy("getDouble", &obj); testCheckAssert(method != NULL);
		testCheckEqual(fromVariant<unsigned int>(method->invoke(obj)), 2 * sizeof(char));
		metaClass->destroyInstance(obj);
	}

}


gTestCase(doTest);


}
