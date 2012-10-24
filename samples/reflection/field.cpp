#include "cpgf/gcallback.h"
#include "cpgf/gcallbacklist.h"
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

	int operator () (const std::string & s) const {
		return this->width + (int)s.length();
	}

private:
	int width;
	std::string name;
	TestData data;
	static int stats;

	TestNoncopyable nocopy;

	cpgf::GCallback<int (const std::string &)> callback;

	friend void doTestLib();
	friend void doTestAPI();
	friend void fieldDefineClass();
};

int TestObject::stats = 5;

void fieldDefineClass()
{
	using namespace cpgf;

	GDefineMetaClass<TestObject>
		::define("field::TestObject")

		._field("width", &TestObject::width)
		._field("name", &TestObject::name)
		._field("data", &TestObject::data)
		._field("callback", &TestObject::callback)
		._field("stats", &TestObject::stats)
		._field("nocopy", &TestObject::nocopy, GMetaPolicyNoncopyable())
	;
}

G_RUN_BEFORE_MAIN(fieldDefineClass())

void doTestLib()
{
	using namespace cpgf;

	const GMetaClass * metaClass;
	const GMetaField * field;

	metaClass = findMetaClass("field::TestObject");
	testCheckAssert(metaClass != NULL);
	std::cout << "Lib: " << metaClass->getName() << std::endl;

	{
		field = metaClass->getField("width"); testCheckAssert(field != NULL);
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		field->set(obj, 123);
		testCheckEqual(pobj->width, 123);
		testCheckEqual(pobj->width, fromVariant<int>(field->get(obj)));
		metaClass->destroyInstance(obj);
	}

	{
		field = metaClass->getField("name"); testCheckAssert(field != NULL);
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		field->set(obj, "new name");
		testCheckStringEqual(pobj->name, "new name");
		std::string s = fromVariant<std::string>(field->get(obj));
		testCheckStringEqual(pobj->name, s);
		metaClass->destroyInstance(obj);
	}

	{
		field = metaClass->getField("data"); testCheckAssert(field != NULL);
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		TestData data(3, "Fake dummy data");
		field->set(obj, data);
		testCheckEqual(pobj->data, data);
		testCheckEqual(pobj->data, fromVariant<TestData>(field->get(obj)));
		metaClass->destroyInstance(obj);
	}

	{
		field = metaClass->getField("nocopy"); testCheckAssert(field != NULL);
		void * obj = metaClass->createInstance();
		
		testBeginException;
			field->set(obj, 1);
		testEndException(GException);

		testBeginException;
			field->get(obj);
		testEndException(GException);

		TestNoncopyable * p = static_cast<TestNoncopyable *>(field->getAddress(obj));
		testCheckEqual(p->tag, TestObject().nocopy.tag);

		metaClass->destroyInstance(obj);
	}

	{
		field = metaClass->getField("stats"); testCheckAssert(field != NULL);
		field->set(NULL, 567);
		testCheckEqual(TestObject::stats, 567);
		testCheckEqual(TestObject::stats, fromVariant<int>(field->get(NULL)));
	}

	{
		field = metaClass->getField("callback"); testCheckAssert(field != NULL);
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;

		pobj->width = 5;

		// can't use this because GVariant doesn't know how to convert TestObject to callback
		// field->set(obj, *pobj);

		// this will work
		field->set(obj, GCallback<int (const std::string &)>(*pobj));

		GCallback<int (const std::string &)> * pcb = static_cast<GCallback<int (const std::string &)> *>(field->getAddress(obj));
		testCheckEqual((*pcb)("abc"), pobj->callback("abc"));

		GCallback<int (const std::string &)> cb = fromVariant<GCallback<int (const std::string &)> >(field->get(obj));
		testCheckEqual(cb("abc"), pobj->callback("abc"));

		metaClass->destroyInstance(obj);
	}


}

void doTestAPI()
{
	using namespace cpgf;

	GScopedInterface<IMetaService> service(createDefaultMetaService());
	testCheckAssert(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("field::TestObject"));
	testCheckAssert(metaClass);
	std::cout << "API: " << metaClass->getName() << std::endl;

	GScopedInterface<IMetaField> field;

	{
		field.reset(metaClass->getField("width")); testCheckAssert(field);
		metaCheckError(field.get());
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		metaSetValue(field.get(), obj, 123);
		testCheckEqual(pobj->width, 123);
		testCheckEqual(pobj->width, fromVariant<int>(metaGetValue(field.get(), obj)));
		metaClass->destroyInstance(obj);
	}

	{
		field.reset(metaClass->getField("name")); testCheckAssert(field);
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		metaSetValue(field.get(), obj, "new name");
		testCheckStringEqual(pobj->name, "new name");
		std::string s = fromVariant<std::string>(metaGetValue(field.get(), obj));
		testCheckStringEqual(pobj->name, s);
		metaClass->destroyInstance(obj);
	}

	{
		field.reset(metaClass->getField("data")); testCheckAssert(field);
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		TestData data(3, "Fake dummy data");
		metaSetValue(field.get(), obj, data);
		testCheckEqual(pobj->data, data);
		testCheckEqual(pobj->data, fromVariant<TestData>(metaGetValue(field.get(), obj)));
		metaClass->destroyInstance(obj);
	}

	{
		field.reset(metaClass->getField("nocopy")); testCheckAssert(field);
		void * obj = metaClass->createInstance();
		
		testBeginException;
			metaSetValue(field.get(), obj, 1);
		testEndException(GException);

		testBeginException;
			metaGetValue(field.get(), obj);
		testEndException(GException);

		TestNoncopyable * p = static_cast<TestNoncopyable *>(field->getAddress(obj));
		testCheckEqual(p->tag, TestObject().nocopy.tag);

		metaClass->destroyInstance(obj);
	}

	{
		field.reset(metaClass->getField("stats")); testCheckAssert(field);
		metaSetValue(field.get(), NULL, 567);
		testCheckEqual(TestObject::stats, 567);
		testCheckEqual(TestObject::stats, fromVariant<int>(metaGetValue(field.get(), NULL)));
	}

	{
		field.reset(metaClass->getField("callback")); testCheckAssert(field);
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;

		pobj->width = 5;

		metaSetValue(field.get(), obj, GCallback<int (const std::string &)>(*pobj));

		GCallback<int (const std::string &)> * pcb = static_cast<GCallback<int (const std::string &)> *>(field->getAddress(obj));
		testCheckEqual((*pcb)("abc"), pobj->callback("abc"));

		GCallback<int (const std::string &)> cb = fromVariant<GCallback<int (const std::string &)> >(metaGetValue(field.get(), obj));
		testCheckEqual(cb("abc"), pobj->callback("abc"));

		metaClass->destroyInstance(obj);
	}


}

void doTest()
{
	doTestLib();
	doTestAPI();
}

gTestCase(doTest);



}
