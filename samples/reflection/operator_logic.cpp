#include "operator_common.h"


namespace {

class TestObject
{
	DEF_TEST_OBJECT(TestObject)

public:
	bool operator > (const TestObject & other) const {
		return this->value > other.value;
	}

	bool operator >= (const TestObject & other) const {
		return this->value >= other.value;
	}

	bool operator < (const TestObject & other) const {
		return this->value < other.value;
	}

	bool operator <= (const TestObject & other) const {
		return this->value <= other.value;
	}

	bool operator && (const TestObject & other) const {
		return this->value && other.value;
	}

	bool operator || (const TestObject & other) const {
		return this->value || other.value;
	}

};

G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<TestObject>
		::define("operator_logic::TestObject")

		._operator<bool (const GMetaSelf &, const TestObject &)>(mopHolder == mopHolder)
		._operator<bool (const GMetaSelf &, const TestObject &)>(mopHolder != mopHolder)

		._operator<bool (const GMetaSelf &, const TestObject &)>(mopHolder > mopHolder)
		._operator<bool (const GMetaSelf &, const TestObject &)>(mopHolder >= mopHolder)

		._operator<bool (const GMetaSelf &, const TestObject &)>(mopHolder < mopHolder)
		._operator<bool (const GMetaSelf &, const TestObject &)>(mopHolder <= mopHolder)

		._operator<bool (const GMetaSelf &, const TestObject &)>(mopHolder && mopHolder)
		._operator<bool (const GMetaSelf &, const TestObject &)>(mopHolder || mopHolder)
	;
}


void doTestLib()
{
	using namespace cpgf;

	const GMetaClass * metaClass;
	const GMetaOperator * metaOperator;

	metaClass = findMetaClass("operator_logic::TestObject");
	testCheckAssert(metaClass != NULL);
	std::cout << "Lib: " << metaClass->getName() << std::endl;

	{
		metaOperator = getOperator(mopHolder == mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		testCheckAssert(fromVariant<bool>(metaOperator->invokeBinary(obj, back)));
		++back.value;
		testCheckAssert(!fromVariant<bool>(metaOperator->invokeBinary(obj, back)));
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder != mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		testCheckAssert(!fromVariant<bool>(metaOperator->invokeBinary(obj, back)));
		++back.value;
		testCheckAssert(fromVariant<bool>(metaOperator->invokeBinary(obj, back)));
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder > mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		testCheckAssert(!fromVariant<bool>(metaOperator->invokeBinary(obj, back)));
		--back.value;
		testCheckAssert(fromVariant<bool>(metaOperator->invokeBinary(obj, back)));
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder >= mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		testCheckAssert(fromVariant<bool>(metaOperator->invokeBinary(obj, back)));
		++back.value;
		testCheckAssert(!fromVariant<bool>(metaOperator->invokeBinary(obj, back)));
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder < mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		testCheckAssert(!fromVariant<bool>(metaOperator->invokeBinary(obj, back)));
		++back.value;
		testCheckAssert(fromVariant<bool>(metaOperator->invokeBinary(obj, back)));
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder <= mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		testCheckAssert(fromVariant<bool>(metaOperator->invokeBinary(obj, back)));
		--back.value;
		testCheckAssert(!fromVariant<bool>(metaOperator->invokeBinary(obj, back)));
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder && mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		testCheckAssert(fromVariant<bool>(metaOperator->invokeBinary(obj, back)));
		back.value = 0;
		testCheckAssert(!fromVariant<bool>(metaOperator->invokeBinary(obj, back)));
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder || mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		testCheckAssert(fromVariant<bool>(metaOperator->invokeBinary(obj, back)));
		back.value = 0;
		testCheckAssert(fromVariant<bool>(metaOperator->invokeBinary(obj, back)));
		((TestObject *)obj)->value = 0;
		testCheckAssert(!fromVariant<bool>(metaOperator->invokeBinary(obj, back)));
		metaClass->destroyInstance(obj);
	}

}

void doTestAPI()
{
	using namespace cpgf;

	GScopedInterface<IMetaService> service(createDefaultMetaService());
	testCheckAssert(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("operator_logic::TestObject"));
	testCheckAssert(metaClass);

	std::cout << "API: " << metaClass->getName() << std::endl;

	GScopedInterface<IMetaOperator> metaOperator;

	{
		metaOperator.reset(getOperator(mopHolder == mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		testCheckAssert(fromVariant<bool>(metaInvokeOperatorBinary(metaOperator.get(), obj, back)));
		++back.value;
		testCheckAssert(!fromVariant<bool>(metaInvokeOperatorBinary(metaOperator.get(), obj, back)));
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder != mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		testCheckAssert(!fromVariant<bool>(metaInvokeOperatorBinary(metaOperator.get(), obj, back)));
		++back.value;
		testCheckAssert(fromVariant<bool>(metaInvokeOperatorBinary(metaOperator.get(), obj, back)));
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder > mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		testCheckAssert(!fromVariant<bool>(metaInvokeOperatorBinary(metaOperator.get(), obj, back)));
		--back.value;
		testCheckAssert(fromVariant<bool>(metaInvokeOperatorBinary(metaOperator.get(), obj, back)));
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder >= mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		testCheckAssert(fromVariant<bool>(metaInvokeOperatorBinary(metaOperator.get(), obj, back)));
		++back.value;
		testCheckAssert(!fromVariant<bool>(metaInvokeOperatorBinary(metaOperator.get(), obj, back)));
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder < mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		testCheckAssert(!fromVariant<bool>(metaInvokeOperatorBinary(metaOperator.get(), obj, back)));
		++back.value;
		testCheckAssert(fromVariant<bool>(metaInvokeOperatorBinary(metaOperator.get(), obj, back)));
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder <= mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		testCheckAssert(fromVariant<bool>(metaInvokeOperatorBinary(metaOperator.get(), obj, back)));
		--back.value;
		testCheckAssert(!fromVariant<bool>(metaInvokeOperatorBinary(metaOperator.get(), obj, back)));
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder && mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		testCheckAssert(fromVariant<bool>(metaInvokeOperatorBinary(metaOperator.get(), obj, back)));
		back.value = 0;
		testCheckAssert(!fromVariant<bool>(metaInvokeOperatorBinary(metaOperator.get(), obj, back)));
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder || mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		testCheckAssert(fromVariant<bool>(metaInvokeOperatorBinary(metaOperator.get(), obj, back)));
		back.value = 0;
		testCheckAssert(fromVariant<bool>(metaInvokeOperatorBinary(metaOperator.get(), obj, back)));
		((TestObject *)obj)->value = 0;
		testCheckAssert(!fromVariant<bool>(metaInvokeOperatorBinary(metaOperator.get(), obj, back)));
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
