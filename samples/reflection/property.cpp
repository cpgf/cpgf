#include "reflect_common.h"



namespace {

class TestObject
{
public:
	TestObject()
		:	width(10),
			name("what's the name"),
			data()
	{
	}

	TestObject(const TestObject & other)
		:	width(other.width),
			name(other.name),
			data(other.data)
	{
	}

	TestObject & operator = (const TestObject & other) {
		this->width = other.width;
		this->name = other.name;
		this->data = other.data;

		return *this;
	}
	
	int getWidth() const {
		return this->width;
	}
	
	void setWidth(int width) {
		this->width = width;
	}
	
	const std::string & getName() const {
		return this->name;
	}
	
	TestData getData() const {
		return this->data;
	}

private:
	int width;
	std::string name;

public:
	TestData data;
	TestNoncopyable nocopy;
};

G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<TestObject>
		::define("property::TestObject")

		._property("width", &TestObject::getWidth, &TestObject::setWidth)
		._property("name", &TestObject::getName, 0)
		._property("data", &TestObject::getData, &TestObject::data)
		._property("nocopy", &TestObject::nocopy, 0, GMetaPolicyNoncopyable())
	;
}

void doTest()
{
	using namespace cpgf;

	const GMetaClass * metaClass;
	const GMetaProperty * prop;

	metaClass = findMetaClass("property::TestObject");
	testCheckAssert(metaClass != NULL);
	std::cout << metaClass->getName() << std::endl;

	{
		prop = metaClass->getProperty("width"); testCheckAssert(prop != NULL);
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		prop->set(obj, 123);
		testCheckEqual(pobj->getWidth(), 123);
		testCheckEqual(pobj->getWidth(), fromVariant<int>(prop->get(obj)));
		metaClass->destroyInstance(obj);
	}

	{
		prop = metaClass->getProperty("name"); testCheckAssert(prop != NULL);
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
//		prop->set(obj, "new name");
//		testCheckStringEqual(pobj->getName(), "new name");
		std::string s = fromVariant<std::string>(prop->get(obj));
		testCheckStringEqual(pobj->getName(), s);
		metaClass->destroyInstance(obj);
	}

	{
		prop = metaClass->getProperty("data"); testCheckAssert(prop != NULL);
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		TestData data(3, "Fake dummy data");
		prop->set(obj, data);
		testCheckEqual(pobj->getData(), data);
		testCheckEqual(pobj->getData(), fromVariant<TestData>(prop->get(obj)));
		metaClass->destroyInstance(obj);
	}

}


gTestCase(doTest);


}
