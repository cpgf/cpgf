#include "operator_common.h"


namespace {

class TestObject
{
	DEF_TEST_OBJECT(TestObject)

public:
	TestObject & operator &= (int n) {
		this->value &= n;

		return *this;
	}

	TestObject & operator |= (int n) {
		this->value |= n;

		return *this;
	}

	TestObject & operator ^= (int n) {
		this->value ^= n;

		return *this;
	}

	TestObject & operator >>= (int n) {
		this->value &= n;

		return *this;
	}

	TestObject & operator <<= (int n) {
		this->value &= n;

		return *this;
	}

};

GMETA_DEFINE_CLASS(TestObject, TestObject, "operator_bitwise_assign::TestObject") {
	using namespace cpgf;

	reflectOperator<TestObject & (GMetaSelf, int)>(mopHolder &= mopHolder);

	reflectOperator<TestObject & (GMetaSelf, int)>(mopHolder |= mopHolder);

	reflectOperator<TestObject & (GMetaSelf, int)>(mopHolder ^= mopHolder);

	reflectOperator<TestObject & (GMetaSelf, int)>(mopHolder >>= mopHolder);

	reflectOperator<TestObject & (GMetaSelf, int)>(mopHolder <<= mopHolder);
}

void doTestLib()
{
	using namespace cpgf;

	const GMetaClass * metaClass;
	const GMetaOperator * metaOperator;

	metaClass = findMetaClass("operator_bitwise_assign::TestObject");
	testCheckAssert(metaClass != NULL);
	std::cout << "Lib: " << metaClass->getName() << std::endl;

	{
		metaOperator = getOperator(mopHolder &= mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back &= 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder |= mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back |= 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder ^= mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back ^= 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder >>= mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back >>= 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder <<= mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeBinary(obj, 3));
		testCheckEqual(res, back <<= 3);
		metaClass->destroyInstance(obj);
	}

}

void doTestAPI()
{
	using namespace cpgf;

	GApiScopedPointer<IMetaService> service(createMetaService());
	testCheckAssert(service);

	GApiScopedPointer<IMetaClass> metaClass(service->findClassByName("operator_bitwise_assign::TestObject"));
	testCheckAssert(metaClass);

	std::cout << "API: " << metaClass->getName() << std::endl;

	GApiScopedPointer<IMetaOperator> metaOperator;

	{
		metaOperator.reset(getOperator(mopHolder &= mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator, obj, 3));
		testCheckEqual(res, back &= 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder |= mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator, obj, 3));
		testCheckEqual(res, back |= 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder ^= mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator, obj, 3));
		testCheckEqual(res, back ^= 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder >>= mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator, obj, 3));
		testCheckEqual(res, back >>= 3);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder <<= mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorBinary(metaOperator, obj, 3));
		testCheckEqual(res, back <<= 3);
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
