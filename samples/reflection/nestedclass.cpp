#include "reflect_common.h"



namespace {

class TestOutterClass
{
public:
	class Inner {
	public:
		int x;

	};

public:
	class AnotherInner {
	public:
		int y;

	};

public:
	TestOutterClass()
		:	width(10)
	{
	}

	TestOutterClass(const TestOutterClass & other)
		:	width(other.width)
	{
	}

	TestOutterClass & operator = (const TestOutterClass & other) {
		this->width = other.width;

		return *this;
	}

private:
	int width;

	GMETA_FRIEND(TestOutterClass);
	friend void doTest();
};


GMETA_DEFINE_CLASS(TestOutterClass, TestOutterClass, "nestedclass::TestOutterClass") {
	using namespace cpgf;

	GMETA_QUALIFIED_CLASS(TestOutterClass::Inner);
	GMETA_QUALIFIED_CLASS(TestOutterClass::AnotherInner);

	GMETA_FIELD(width);
}

GMETA_DEFINE_CLASS(TestOutterClass::Inner, TestOutterClass_Inner, "nestedclass::TestOutterClass::Inner") {
	using namespace cpgf;
		
	GMETA_FIELD(x);
}
	
GMETA_DEFINE_CLASS(TestOutterClass::AnotherInner, TestOutterClass_AnotherInner, "nestedclass::TestOutterClass::AnotherInner") {
	using namespace cpgf;

	GMETA_FIELD(y);
}

void doTest()
{
	using namespace cpgf;

	const GMetaClass * metaClass;
	const GMetaField * field;
	const GMetaClass * innerClass;

	metaClass = findMetaClass("nestedclass::TestOutterClass");
	testCheckAssert(metaClass != NULL);
	std::cout << metaClass->getName() << std::endl;

	{
		field = metaClass->getField("width"); testCheckAssert(field != NULL);
		void * obj = metaClass->createInstance();
		TestOutterClass * pobj = (TestOutterClass *)obj;
		field->set(obj, 123);
		testCheckEqual(pobj->width, 123);
		testCheckEqual(pobj->width, fromVariant<int>(field->get(obj)));
		metaClass->destroyInstance(obj);
	}

	{
		innerClass = metaClass->getClass("nestedclass::TestOutterClass::Inner");
		testCheckAssert(innerClass != NULL);

		field = innerClass->getField("x"); testCheckAssert(field != NULL);
		void * obj = innerClass->createInstance();
		TestOutterClass::Inner * pobj = (TestOutterClass::Inner *)obj;
		field->set(obj, 567);
		testCheckEqual(pobj->x, 567);
		testCheckEqual(pobj->x, fromVariant<int>(field->get(obj)));
		innerClass->destroyInstance(obj);
	}

	{
		innerClass = metaClass->getClass("nestedclass::TestOutterClass::AnotherInner");
		testCheckAssert(innerClass != NULL);

		field = innerClass->getField("y"); testCheckAssert(field != NULL);
		void * obj = innerClass->createInstance();
		TestOutterClass::AnotherInner * pobj = (TestOutterClass::AnotherInner *)obj;
		field->set(obj, 567);
		testCheckEqual(pobj->y, 567);
		testCheckEqual(pobj->y, fromVariant<int>(field->get(obj)));
		innerClass->destroyInstance(obj);
	}


}


gTestCase(doTest);



}
