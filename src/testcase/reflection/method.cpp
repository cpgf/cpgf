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

	bool operator == (const TestObject & other) const {
		return this->width == other.width
			&& this->name == other.name
			&& this->data == other.data;
	}

	int getWidth() const {
		return this->width;
	}

	void incWidth(int d, std::string & s) {
		this->width += d + static_cast<int>(s.length());
		s = "append";
	}

	TestData G_API_CC calcData(int a, long b, const std::string & s) {
		TestData result(this->data);

		result.x += a + b;
		result.tag = result.tag + s;

		return result;
	}

	std::string & refName() {
		return this->name;
	}

	int sum(const cpgf::GMetaVariadicParam * params) const {
		int total = 0;
		for(size_t i = 0; i < params->paramCount; ++i) {
			total += cpgf::fromVariant<int>(*(params->params[i]));
		}

		return total;
	}

public:
	int width;
	std::string name;
	TestData data;
};

GMETA_DEFINE_CLASS(TestObject, TestObject, "method::TestObject") {
	using namespace cpgf;

	reflectMethod("getWidth", &TestObject::getWidth);
	GMETA_METHOD(incWidth);
	GMETA_METHOD(calcData);
	GMETA_METHOD(refName);
	GMETA_METHOD(sum);
}

void doTestLib()
{
	using namespace cpgf;

	const GMetaClass * metaClass;
	const GMetaMethod * method;

	metaClass = findMetaClass("method::TestObject");
	testCheckAssert(metaClass != NULL);
	std::cout << "Lib: " << metaClass->getName() << std::endl;

	{
		method = metaClass->getMethod("getWidth"); testCheckAssert(method != NULL);
		testCheckAssert(method->getItemType().isConstFunction());

		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		pobj->width = 123;
		testCheckEqual(pobj->width, fromVariant<int>(method->invoke(obj)));
		metaClass->destroyInstance(obj);
	}

	{
		method = metaClass->getMethod("incWidth"); testCheckAssert(method != NULL);
		testCheckAssert(! method->getItemType().isConstFunction());

		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		pobj->width = 567;
		TestObject back(*(TestObject *)obj);
		std::string s = "abcde";
		method->invoke(obj, 3, s);
		std::string ns = "abcde";
		back.incWidth(3, ns);
		testCheckStringEqual(s, ns);
		testCheckEqual(back, *pobj);
		metaClass->destroyInstance(obj);
	}

	{
		method = metaClass->getMethod("calcData"); testCheckAssert(method != NULL);
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		pobj->data = TestData(168, "Test stdcall");
		TestObject back(*(TestObject *)obj);
		TestData data1 = fromVariant<TestData>(method->invoke(obj, 3, 5, "abc"));
		TestData data2 = back.calcData(3, 5, "abc");
		testCheckEqual(data1, data2);
		metaClass->destroyInstance(obj);
	}

	{
		method = metaClass->getMethod("refName"); testCheckAssert(method != NULL);
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		pobj->name = "";
		testCheckAssert(pobj->name != "abc");
		fromVariant<std::string &>(method->invoke(obj)) = "abc";
		testCheckAssert(pobj->name == "abc");
		*static_cast<std::string *>(referenceAddressFromVariant(method->invoke(obj))) = "def";
		testCheckAssert(pobj->name == "def");
		metaClass->destroyInstance(obj);
	}

	{
		method = metaClass->getMethod("sum"); testCheckAssert(method != NULL);
		testCheckAssert(method->getItemType().isConstFunction());

		void * obj = metaClass->createInstance();
		int n = fromVariant<int>(method->invoke(obj, 1, 3, 5, 7, 9));
		testCheckEqual(n, 1 + 3 + 5 + 7 + 9);
		metaClass->destroyInstance(obj);
	}

}

void doTestAPI()
{
	using namespace cpgf;

	GMetaScopedPointer<IMetaService> service(createMetaService());
	testCheckAssert(service);

	GMetaScopedPointer<IMetaClass> metaClass(service->findClassByName("method::TestObject"));
	testCheckAssert(metaClass);

	std::cout << "API: " << metaClass->getName() << std::endl;

	GMetaScopedPointer<IMetaMethod> method;

	{
		method.reset(metaClass->getMethod("getWidth")); testCheckAssert(method);
		testCheckAssert(metaGetItemType(method).isConstFunction());
		
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		pobj->width = 123;
		testCheckEqual(pobj->width, fromVariant<int>(metaCallMethod(method, obj)));
		metaClass->destroyInstance(obj);
	}

	{
		method.reset(metaClass->getMethod("incWidth")); testCheckAssert(method);
		testCheckAssert(! metaGetItemType(method).isConstFunction());
		
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		pobj->width = 567;
		TestObject back(*(TestObject *)obj);
		std::string s = "abcde";
		metaCallMethod(method, obj, 3, s);
		std::string ns = "abcde";
		back.incWidth(3, ns);
		testCheckStringEqual(s, ns);
		testCheckEqual(back, *pobj);
		metaClass->destroyInstance(obj);
	}

	{
		method.reset(metaClass->getMethod("calcData")); testCheckAssert(method);
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		pobj->data = TestData(168, "Test stdcall");
		TestObject back(*(TestObject *)obj);
		TestData data1 = fromVariant<TestData>(metaCallMethod(method, obj, 3, 5, "abc"));
		TestData data2 = back.calcData(3, 5, "abc");
		testCheckEqual(data1, data2);
		metaClass->destroyInstance(obj);
	}

	{
		method.reset(metaClass->getMethod("refName")); testCheckAssert(method);
		void * obj = metaClass->createInstance();
		TestObject * pobj = (TestObject *)obj;
		pobj->name = "";
		testCheckAssert(pobj->name != "abc");
		fromVariant<std::string &>(metaCallMethod(method, obj)) = "abc";
		testCheckAssert(pobj->name == "abc");
		*static_cast<std::string *>(referenceAddressFromVariant(metaCallMethod(method, obj))) = "def";
		testCheckAssert(pobj->name == "def");
		metaClass->destroyInstance(obj);
	}

	{
		method.reset(metaClass->getMethod("sum")); testCheckAssert(method);
		testCheckAssert(metaGetItemType(method).isConstFunction());

		void * obj = metaClass->createInstance();
		int n = fromVariant<int>(metaCallMethod(method, obj, 1, 3, 5, 7, 9));
		testCheckEqual(n, 1 + 3 + 5 + 7 + 9);
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
