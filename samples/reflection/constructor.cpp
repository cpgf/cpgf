#include "reflect_common.h"



namespace {

class TestObject
{
public:
	TestObject()
		:	width(10),
			name("default name"),
			data(1, "default tag")
	{
	}

	TestObject(int width)
		:	width(width),
			name("default name"),
			data(1, "default tag")
	{
	}

	TestObject(int width, const std::string & name)
		:	width(width),
			name(name),
			data(1, "default tag")
	{
	}

	TestObject(int width, const std::string & name, const TestData & data)
		:	width(width),
			name(name),
			data(data)
	{
	}

	TestObject(const cpgf::GMetaVariadicParam * params)
		:	name("default name"),
			data(1, "default tag")
	{
		int total = 0;
		for(size_t i = 0; i < params->paramCount; ++i) {
			total += cpgf::fromVariant<int>(*(params->params[i]));
		}

		this->width = total;
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

public:
	int width;
	std::string name;
	TestData data;
};


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<TestObject>
		::define("constructor::TestObject")

		._constructor<void * ()>()
		._constructor<void * (int)>()
		._constructor<void * (int, const std::string &)>(GMetaPolicyCopyAllConstReference())
		._constructor<void * (int, const std::string &, const TestData &)>(GMetaPolicyCopyAllConstReference())
		._constructor<void * (const cpgf::GMetaVariadicParam *)>()
	;
}


void doTestLib()
{
	using namespace cpgf;

	const GMetaClass * metaClass;
	const GMetaConstructor * constructor;

	metaClass = findMetaClass("constructor::TestObject");
	testCheckAssert(metaClass != NULL);
	std::cout << "Lib: " << metaClass->getName() << std::endl;

	{
		constructor = metaClass->getConstructorAt(0); testCheckAssert(constructor != NULL);
		void * obj = constructor->invoke();
		TestObject back;
		testCheckEqual(back, *(TestObject *)obj);
		metaClass->destroyInstance(obj);
	}

	{
		constructor = metaClass->getConstructorAt(1); testCheckAssert(constructor != NULL);
		void * obj = constructor->invoke(5);
		TestObject back(5);
		testCheckEqual(back, *(TestObject *)obj);
		metaClass->destroyInstance(obj);
	}

	{
		constructor = metaClass->getConstructorAt(2); testCheckAssert(constructor != NULL);
		void * obj = constructor->invoke(18, "new name");
		TestObject back(18, "new name");
		testCheckEqual(back, *(TestObject *)obj);
		metaClass->destroyInstance(obj);
	}

	{
		constructor = metaClass->getConstructorAt(3); testCheckAssert(constructor != NULL);
		void * obj = constructor->invoke(9, "another name", TestData(199, "another tag"));
		TestObject back(9, "another name", TestData(199, "another tag"));
		testCheckEqual(back, *(TestObject *)obj);
		metaClass->destroyInstance(obj);
	}

	{
		constructor = metaClass->getConstructorAt(4); testCheckAssert(constructor != NULL);
		void * obj = constructor->invoke(1, 3, 5, 7, 9);
		int n = ((TestObject *)obj)->width;
		testCheckEqual(n, 1 + 3 + 5 + 7 + 9);
		metaClass->destroyInstance(obj);
	}

}

void doTestAPI()
{
	using namespace cpgf;

	GScopedInterface<IMetaService> service(createDefaultMetaService());
	testCheckAssert(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("constructor::TestObject"));
	testCheckAssert(metaClass);

	std::cout << "API: " << metaClass->getName() << std::endl;

	GScopedInterface<IMetaConstructor> constructor;

	{
		constructor.reset(metaClass->getConstructorAt(0)); testCheckAssert(constructor);
		void * obj = metaInvokeConstructor(constructor.get());
		TestObject back;
		testCheckEqual(back, *(TestObject *)obj);
		metaClass->destroyInstance(obj);
	}

	{
		constructor.reset(metaClass->getConstructorAt(1)); testCheckAssert(constructor);
		void * obj = metaInvokeConstructor(constructor.get(), 5);
		TestObject back(5);
		testCheckEqual(back, *(TestObject *)obj);
		metaClass->destroyInstance(obj);
	}

	{
		constructor.reset(metaClass->getConstructorAt(2)); testCheckAssert(constructor);
		void * obj = metaInvokeConstructor(constructor.get(), 18, "new name");
		TestObject back(18, "new name");
		testCheckEqual(back, *(TestObject *)obj);
		metaClass->destroyInstance(obj);
	}

	{
		constructor.reset(metaClass->getConstructorAt(3)); testCheckAssert(constructor);
		void * obj = metaInvokeConstructor(constructor.get(), 9, "another name", TestData(199, "another tag"));
		TestObject back(9, "another name", TestData(199, "another tag"));
		testCheckEqual(back, *(TestObject *)obj);
		metaClass->destroyInstance(obj);
	}

	{
		constructor.reset(metaClass->getConstructorAt(4)); testCheckAssert(constructor);
		void * obj = metaInvokeConstructor(constructor.get(), 1, 3, 5, 7, 9);
		int n = ((TestObject *)obj)->width;
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
