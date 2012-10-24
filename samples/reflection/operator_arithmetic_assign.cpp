#include "operator_common.h"


namespace {

class TestObject
{
	DEF_TEST_OBJECT(TestObject)

public:
	TestObject & operator += (int n) {
		this->value += n;

		return *this;
	}

	TestObject & operator -= (int n) {
		this->value -= n;

		return *this;
	}

	TestObject & operator *= (int n) {
		this->value *= n;

		return *this;
	}

	TestObject & operator /= (int n) {
		this->value /= n;

		return *this;
	}

	TestObject & operator %= (int n) {
		this->value %= n;

		return *this;
	}

};


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<TestObject>
		::define("operator_arithmetic_assign::TestObject")

		._operator<TestObject (GMetaSelf, int)>(mopHolder += mopHolder)
		._operator<TestObject (GMetaSelf, int)>(mopHolder -= mopHolder)
		._operator<TestObject (GMetaSelf, int)>(mopHolder *= mopHolder)
		._operator<TestObject (GMetaSelf, int)>(mopHolder /= mopHolder)
		._operator<TestObject (GMetaSelf, int)>(mopHolder %= mopHolder)
	;
}

void doTestLib()
{
	using namespace cpgf;

	const GMetaClass * metaClass;
	const GMetaOperator * metaOperator;

	metaClass = findMetaClass("operator_arithmetic_assign::TestObject");
	testCheckAssert(metaClass != NULL);
	std::cout << "Lib: " << metaClass->getName() << std::endl;

	{
		metaOperator = getOperator(mopHolder += mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back += 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder -= mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back -= 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder *= mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back *= 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder /= mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back /= 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder %= mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back %= 3);
		metaClass->destroyInstance(obj);
	}

}

void doTestAPI()
{
	using namespace cpgf;

	GScopedInterface<IMetaService> service(createDefaultMetaService());
	testCheckAssert(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("operator_arithmetic_assign::TestObject"));
	testCheckAssert(metaClass);

	std::cout << "API: " << metaClass->getName() << std::endl;

	GScopedInterface<IMetaOperator> metaOperator;

	{
		metaOperator.reset(getOperator(mopHolder += mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator.get(), obj, 3));
		testCheckEqual(res, back += 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder -= mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator.get(), obj, 3));
		testCheckEqual(res, back -= 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder *= mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator.get(), obj, 3));
		testCheckEqual(res, back *= 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder /= mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator.get(), obj, 3));
		testCheckEqual(res, back /= 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder %= mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator.get(), obj, 3));
		testCheckEqual(res, back %= 3);
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
