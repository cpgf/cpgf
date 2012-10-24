#include "operator_common.h"


namespace {

class TestObject
{
	DEF_TEST_OBJECT(TestObject)

public:
	TestObject operator + (int n) const {
		return TestObject(this->value + n);
	}

	TestObject operator - (int n) const {
		return TestObject(this->value - n);
	}

	TestObject operator * (int n) const {
		return TestObject(this->value * n);
	}

	TestObject operator / (int n) const {
		return TestObject(this->value / n);
	}

	TestObject operator % (int n) const {
		return TestObject(this->value % n);
	}

	TestObject operator % (int n) {
		return TestObject(this->value % (n + 1));
	}

};

int operator + (int n, const TestObject & obj) {
	return n * 2 + obj.value;
}


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<TestObject>
		::define("operator_arithmetic::TestObject")

		._operator<TestObject & (GMetaSelf, const TestObject &)>(mopHolder = mopHolder)
		._operator<TestObject (const GMetaSelf &, int)>(mopHolder + mopHolder)
		._operator<int (int, const TestObject &)>(mopHolder + mopHolder)
		._operator<TestObject (const GMetaSelf &, int)>(mopHolder - mopHolder)
		._operator<TestObject (const GMetaSelf &, int)>(mopHolder * mopHolder)
		._operator<TestObject (const GMetaSelf &, int)>(mopHolder / mopHolder)

		// test operator function constness correct
		._operator<TestObject (const GMetaSelf &, int)>(mopHolder % mopHolder)
		._operator<TestObject (GMetaSelf, int)>(mopHolder % mopHolder)
	;
}

void doTestLib()
{
	using namespace cpgf;

	const GMetaClass * metaClass;
	const GMetaOperator * metaOperator;

	metaClass = findMetaClass("operator_arithmetic::TestObject");
	testCheckAssert(metaClass != NULL);
	std::cout << "Lib: " << metaClass->getName() << std::endl;

	{
		metaOperator = getOperator(mopHolder = mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		((TestObject *)obj)->value *= 2;
		testCheckAssert(back != *(TestObject *)obj);
		fromVariant<TestObject>(metaOperator->invokeBinary(obj, back));
		testCheckAssert(back == *(TestObject *)obj);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder + mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back + 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder + mopHolder, metaClass, 1); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		int n = fromVariant<int>(metaOperator->invokeBinary(3, pointerToRefVariant((TestObject *)obj)));
		testCheckEqual(n, 3 + back);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder - mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back - 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder * mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back * 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder / mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back / 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder % mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		const TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back % 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder % mopHolder, metaClass, 1); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back % 3);
		metaClass->destroyInstance(obj);
	}

}

void doTestAPI()
{
	using namespace cpgf;

	GScopedInterface<IMetaService> service(createDefaultMetaService());
	testCheckAssert(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("operator_arithmetic::TestObject"));
	testCheckAssert(metaClass);

	std::cout << "API: " << metaClass->getName() << std::endl;

	GScopedInterface<IMetaOperator> metaOperator;

	{
		metaOperator.reset(getOperator(mopHolder = mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		((TestObject *)obj)->value *= 2;
		testCheckAssert(back != *(TestObject *)obj);
		fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator.get(), obj, back));
		testCheckAssert(back == *(TestObject *)obj);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder + mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator.get(), obj, 3));
		testCheckEqual(res, back + 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder + mopHolder, metaClass, 1)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		int n = fromVariant<int>(metaInvokeOperatorBinary(metaOperator.get(), 3, pointerToRefVariant((TestObject *)obj)));
		testCheckEqual(n, 3 + back);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder - mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator.get(), obj, 3));
		testCheckEqual(res, back - 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder * mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator.get(), obj, 3));
		testCheckEqual(res, back * 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder / mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator.get(), obj, 3));
		testCheckEqual(res, back / 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder % mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		const TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator.get(), obj, 3));
		testCheckEqual(res, back % 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder % mopHolder, metaClass, 1)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator.get(), obj, 3));
		testCheckEqual(res, back % 3);
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
