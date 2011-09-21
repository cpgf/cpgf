#include "cpgf/gcallback.h"
#include "cpgf/gcallbacklist.h"
#include "test_common.h"


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

	GMETA_FRIEND(TestObject);
	friend void doTestLib();
	friend void doTestAPI();
};

int TestObject::stats = 5;


GMETA_DEFINE_CLASS(TestObject, TestObject, "field::TestObject") {
	using namespace cpgf;

	GMETA_FIELD(width);
	GMETA_FIELD(name);
	GMETA_FIELD(data);
	GMETA_FIELD(callback);

	GMETA_FIELD(stats);

	reflectField("nocopy", &TestObject::nocopy, GMetaPolicyNoncopyable());
}

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
		testEndException(GMetaException);

		testBeginException;
			field->get(obj);
		testEndException(GMetaException);

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

	GMetaScopedPointer<IMetaService> service(createMetaService());
	testCheckAssert(service);

	GMetaScopedPointer<IMetaClass> metaClass(service->findClassByName("field::TestObject"));
	testCheckAssert(metaClass);
	std::cout << "API: " << metaClass->getName() << std::endl;

	GMetaScopedPointer<IMetaField> field;

	{
		field.reset(metaClass->getField("width")); testCheckAssert(field);
		metaCheckError(field);
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		metaSetValue(field, obj, 123);
		testCheckEqual(pobj->width, 123);
		testCheckEqual(pobj->width, fromVariant<int>(metaGetValue(field, obj)));
		metaClass->destroyInstance(obj);
	}

	{
		field.reset(metaClass->getField("name")); testCheckAssert(field);
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		metaSetValue(field, obj, "new name");
		testCheckStringEqual(pobj->name, "new name");
		std::string s = fromVariant<std::string>(metaGetValue(field, obj));
		testCheckStringEqual(pobj->name, s);
		metaClass->destroyInstance(obj);
	}

	{
		field.reset(metaClass->getField("data")); testCheckAssert(field);
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		TestData data(3, "Fake dummy data");
		metaSetValue(field, obj, data);
		testCheckEqual(pobj->data, data);
		testCheckEqual(pobj->data, fromVariant<TestData>(metaGetValue(field, obj)));
		metaClass->destroyInstance(obj);
	}

	{
		field.reset(metaClass->getField("nocopy")); testCheckAssert(field);
		void * obj = metaClass->createInstance();
		
		testBeginException;
			metaSetValue(field, obj, 1);
		testEndException(GMetaException);

		testBeginException;
			metaGetValue(field, obj);
		testEndException(GMetaException);

		TestNoncopyable * p = static_cast<TestNoncopyable *>(field->getAddress(obj));
		testCheckEqual(p->tag, TestObject().nocopy.tag);

		metaClass->destroyInstance(obj);
	}

	{
		field.reset(metaClass->getField("stats")); testCheckAssert(field);
		metaSetValue(field, NULL, 567);
		testCheckEqual(TestObject::stats, 567);
		testCheckEqual(TestObject::stats, fromVariant<int>(metaGetValue(field, NULL)));
	}

	{
		field.reset(metaClass->getField("callback")); testCheckAssert(field);
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;

		pobj->width = 5;

		metaSetValue(field, obj, GCallback<int (const std::string &)>(*pobj));

		GCallback<int (const std::string &)> * pcb = static_cast<GCallback<int (const std::string &)> *>(field->getAddress(obj));
		testCheckEqual((*pcb)("abc"), pobj->callback("abc"));

		GCallback<int (const std::string &)> cb = fromVariant<GCallback<int (const std::string &)> >(metaGetValue(field, obj));
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
