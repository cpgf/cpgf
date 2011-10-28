#include "operator_common.h"


namespace {

class TestObject
{
	DEF_TEST_OBJECT(TestObject)

public:
	TestObject operator & (int n) const {
		return TestObject(this->value & n);
	}

	TestObject operator | (int n) const {
		return TestObject(this->value | n);
	}

	TestObject operator ^ (int n) const {
		return TestObject(this->value ^ n);
	}

	TestObject operator >> (int n) const {
		return TestObject(this->value >> n);
	}

	TestObject operator << (int n) const {
		return TestObject(this->value << n);
	}

};

GMETA_DEFINE_CLASS(TestObject, TestObject, "operator_bitwise::TestObject") {
	using namespace cpgf;

	reflectOperator<TestObject (const GMetaSelf &, int)>(mopHolder & mopHolder);

	reflectOperator<TestObject (const GMetaSelf &, int)>(mopHolder | mopHolder);

	reflectOperator<TestObject (const GMetaSelf &, int)>(mopHolder ^ mopHolder);

	reflectOperator<TestObject (const GMetaSelf &, int)>(mopHolder >> mopHolder);

	reflectOperator<TestObject (const GMetaSelf &, int)>(mopHolder << mopHolder);
}

void doTestLib()
{
	using namespace cpgf;

	const GMetaClass * metaClass;
	const GMetaOperator * metaOperator;

	metaClass = findMetaClass("operator_bitwise::TestObject");
	testCheckAssert(metaClass != NULL);
	std::cout << "Lib: " << metaClass->getName() << std::endl;

	{
		metaOperator = getOperator(mopHolder & mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back & 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder | mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back | 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder ^ mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back ^ 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder >> mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back >> 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder << mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		const TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back << 3);
		metaClass->destroyInstance(obj);
	}

}

void doTestAPI()
{
	using namespace cpgf;

	GScopedInterface<IMetaService> service(createDefaultMetaService());
	testCheckAssert(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("operator_bitwise::TestObject"));
	testCheckAssert(metaClass);

	std::cout << "API: " << metaClass->getName() << std::endl;

	GScopedInterface<IMetaOperator> metaOperator;

	{
		metaOperator.reset(getOperator(mopHolder & mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator, obj, 3));
		testCheckEqual(res, back & 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder | mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator, obj, 3));
		testCheckEqual(res, back | 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder ^ mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator, obj, 3));
		testCheckEqual(res, back ^ 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder >> mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator, obj, 3));
		testCheckEqual(res, back >> 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder << mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		const TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator, obj, 3));
		testCheckEqual(res, back << 3);
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