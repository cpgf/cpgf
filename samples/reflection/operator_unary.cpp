#include "operator_common.h"


namespace {

class TestObject
{
	DEF_TEST_OBJECT(TestObject)

public:
	TestObject operator + () const {
		return TestObject(this->value < 0 ? -this->value : this->value);
	}

	TestObject operator - () const {
		return TestObject(this->value < 0 ? this->value : -this->value);
	}

	TestObject operator ++ () {
		return TestObject(++this->value);
	}

	TestObject operator ++ (int) {
		TestObject t = TestObject(this->value);
		++this->value;
		return t;
	}

	TestObject operator -- () {
		return TestObject(--this->value);
	}

	TestObject operator -- (int) {
		TestObject t = TestObject(this->value);
		--this->value;
		return t;
	}

	TestObject operator ! () const {
		return TestObject(!this->value);
	}

	TestObject operator ~ () const {
		return TestObject(~this->value);
	}

	int * operator & () {
		return &this->value;
	}

	int operator * () const {
		return this->value * 2;
	}

};

G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<TestObject>
		::define("operator_unary::TestObject")

		._operator<TestObject (const GMetaSelf &)> (+ mopHolder)
		._operator<TestObject (const GMetaSelf &)> (- mopHolder)

		._operator<TestObject (GMetaSelf)> (++mopHolder)
		._operator<TestObject (GMetaSelf)> (mopHolder++)
		._operator<TestObject (GMetaSelf)> (--mopHolder)
		._operator<TestObject (GMetaSelf)> (mopHolder--)

		._operator<TestObject (const GMetaSelf &)> (! mopHolder)
		._operator<TestObject (const GMetaSelf &)> (~ mopHolder)

		._operator<int * (GMetaSelf)> (& mopHolder)
		._operator<int (const GMetaSelf &)> (* mopHolder)
	;
}


void doTestLib()
{
	using namespace cpgf;

	const GMetaClass * metaClass;
	const GMetaOperator * metaOperator;

	metaClass = findMetaClass("operator_unary::TestObject");
	testCheckAssert(metaClass != NULL);
	std::cout << "Lib: " << metaClass->getName() << std::endl;

	{
		metaOperator = getOperator(+ mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		((TestObject *)obj)->value = -((TestObject *)obj)->value;
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeUnary(obj));
		testCheckEqual(res, +back);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(- mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		((TestObject *)obj)->value = -((TestObject *)obj)->value;
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeUnary(obj));
		testCheckEqual(res, -back);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(++ mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeUnary(obj));
		testCheckEqual(res, ++back);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder ++, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeUnary(obj));
		testCheckEqual(res, back);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(-- mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeUnary(obj));
		testCheckEqual(res, --back);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(mopHolder --, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeUnary(obj));
		testCheckEqual(res, back);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(! mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeUnary(obj));
		testCheckEqual(res, !back);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(~ mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaOperator->invokeUnary(obj));
		testCheckEqual(res, ~back);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(& mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		int * pn = fromVariant<int *>(metaOperator->invokeUnary(obj));
		testCheckEqual(*pn, *(&back));
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator = getOperator(* mopHolder, metaClass, 0); testCheckAssert(metaOperator != NULL);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		int n = fromVariant<int>(metaOperator->invokeUnary(obj));
		testCheckEqual(n, *back);
		metaClass->destroyInstance(obj);
	}


}

void doTestAPI()
{
	using namespace cpgf;

	GScopedInterface<IMetaService> service(createDefaultMetaService());
	testCheckAssert(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("operator_unary::TestObject"));
	testCheckAssert(metaClass);

	std::cout << "API: " << metaClass->getName() << std::endl;

	GScopedInterface<IMetaOperator> metaOperator;

	{
		metaOperator.reset(getOperator(+ mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		((TestObject *)obj)->value = -((TestObject *)obj)->value;
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorUnary(metaOperator.get(), obj));
		testCheckEqual(res, +back);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(- mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		((TestObject *)obj)->value = -((TestObject *)obj)->value;
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorUnary(metaOperator.get(), obj));
		testCheckEqual(res, -back);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(++ mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorUnary(metaOperator.get(), obj));
		testCheckEqual(res, ++back);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder ++, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorUnary(metaOperator.get(), obj));
		testCheckEqual(res, back);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(-- mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorUnary(metaOperator.get(), obj));
		testCheckEqual(res, --back);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(mopHolder --, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorUnary(metaOperator.get(), obj));
		testCheckEqual(res, back);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(! mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorUnary(metaOperator.get(), obj));
		testCheckEqual(res, !back);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(~ mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		TestObject res = fromVariant<TestObject>(metaInvokeOperatorUnary(metaOperator.get(), obj));
		testCheckEqual(res, ~back);
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(& mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		int * pn = fromVariant<int *>(metaInvokeOperatorUnary(metaOperator.get(), obj));
		testCheckEqual(*pn, *(&back));
		metaClass->destroyInstance(obj);
	}

	{
		metaOperator.reset(getOperator(* mopHolder, metaClass, 0)); testCheckAssert(metaOperator);
		void * obj = metaClass->createInstance();
		TestObject back(*(TestObject *)obj);
		int n = fromVariant<int>(metaInvokeOperatorUnary(metaOperator.get(), obj));
		testCheckEqual(n, *back);
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
