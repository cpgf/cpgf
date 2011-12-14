#include "reflect_common.h"



namespace {

class TestAbstract
{
public:
	virtual void abstractFunction() = 0;
};


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<TestAbstract>
		::define("class::TestAbstract")

		._method("abstractFunction", &TestAbstract::abstractFunction)
	;
}


class TestNoDefaultConstructor
{
public:
	TestNoDefaultConstructor(int) {
	}
};

GMETA_DEFINE_POLICIED_CLASS(TestNoDefaultConstructor, TestNoDefaultConstructor, "class::TestNoDefaultConstructor", cpgf::GMetaPolicyNoDefaultConstructor()) {
	using namespace cpgf;

}

class TestNoCopyConstructor
{
public:
	TestNoCopyConstructor() {
	}

private:
	TestNoCopyConstructor(const TestNoCopyConstructor &);
	TestNoCopyConstructor & operator = (const TestNoCopyConstructor &);
};

GMETA_DEFINE_POLICIED_CLASS(TestNoCopyConstructor, TestNoCopyConstructor, "class::TestNoCopyConstructor", cpgf::GMetaPolicyNoCopyConstructor()) {
	using namespace cpgf;

}

class TestNoDefaultAndCopyConstructor
{
public:
	TestNoDefaultAndCopyConstructor(int) {
	}

private:
	TestNoDefaultAndCopyConstructor(const TestNoDefaultAndCopyConstructor &);
	TestNoDefaultAndCopyConstructor & operator = (const TestNoDefaultAndCopyConstructor &);
};

GMETA_DEFINE_POLICIED_CLASS(TestNoDefaultAndCopyConstructor, TestNoDefaultAndCopyConstructor, "class::TestNoDefaultAndCopyConstructor", cpgf::GMetaPolicyNoDefaultAndCopyConstructor()) {
	using namespace cpgf;

}

void doTestLib()
{
	using namespace cpgf;

	std::cout << "Lib: " << "Test Meta Classes" << std::endl;
	
	const GMetaClass * metaClass;
	char buffer[4096];

	metaClass = findMetaClass("class::TestAbstract");
	testCheckAssert(metaClass != NULL);
	testCheckAssert(metaClass->isAbstract());
	testCheckAssert(! metaClass->canCreateInstance());
	testCheckAssert(! metaClass->canCopyInstance());
	testBeginException;
		metaClass->createInstance();
	testEndException(GException);
	testBeginException;
		metaClass->createInplace(buffer);
	testEndException(GException);

	metaClass = findMetaClass("class::TestNoDefaultConstructor");
	testCheckAssert(metaClass != NULL);
	testCheckAssert(! metaClass->isAbstract());
	testCheckAssert(! metaClass->canCreateInstance());
	testCheckAssert(metaClass->canCopyInstance());
	testBeginException;
		metaClass->createInstance();
	testEndException(GException);
	testBeginException;
		metaClass->createInplace(buffer);
	testEndException(GException);

	metaClass = findMetaClass("class::TestNoCopyConstructor");
	testCheckAssert(metaClass != NULL);
	testCheckAssert(! metaClass->isAbstract());
	testCheckAssert(metaClass->canCreateInstance());
	testCheckAssert(! metaClass->canCopyInstance());
	testBeginException;
		metaClass->cloneInstance(NULL);
	testEndException(GException);
	testBeginException;
		metaClass->cloneInplace(NULL, buffer);
	testEndException(GException);

	metaClass = findMetaClass("class::TestNoDefaultAndCopyConstructor");
	testCheckAssert(metaClass != NULL);
	testCheckAssert(! metaClass->isAbstract());
	testCheckAssert(! metaClass->canCreateInstance());
	testCheckAssert(! metaClass->canCopyInstance());
	testBeginException;
		metaClass->cloneInstance(NULL);
	testEndException(GException);
	testBeginException;
		metaClass->cloneInplace(NULL, buffer);
	testEndException(GException);

}

void doTestAPI()
{
	using namespace cpgf;

	std::cout << "API: " << "Test Meta Classes" << std::endl;
	
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	testCheckAssert(service);

	GScopedInterface<IMetaClass> metaClass;

	char buffer[4096];

	metaClass.reset(service->findClassByName("class::TestAbstract"));
	testCheckAssert(metaClass);
	testCheckAssert(!! metaClass->isAbstract());
	testCheckAssert(! metaClass->canCreateInstance());
	testCheckAssert(! metaClass->canCopyInstance());
	testBeginException;
		metaClass->createInstance();
		metaCheckError(metaClass);
	testEndException(GException);
	testBeginException;
		metaClass->createInplace(buffer);
		metaCheckError(metaClass);
	testEndException(GException);

	metaClass.reset(service->findClassByName("class::TestNoDefaultConstructor"));
	testCheckAssert(metaClass);
	testCheckAssert(! metaClass->isAbstract());
	testCheckAssert(! metaClass->canCreateInstance());
	testCheckAssert(!! metaClass->canCopyInstance());
	testBeginException;
		metaClass->createInstance();
		metaCheckError(metaClass);
	testEndException(GException);
	testBeginException;
		metaClass->createInplace(buffer);
		metaCheckError(metaClass);
	testEndException(GException);

	metaClass.reset(service->findClassByName("class::TestNoCopyConstructor"));
	testCheckAssert(metaClass);
	testCheckAssert(! metaClass->isAbstract());
	testCheckAssert(!! metaClass->canCreateInstance());
	testCheckAssert(! metaClass->canCopyInstance());
	testBeginException;
		metaClass->cloneInstance(NULL);
		metaCheckError(metaClass);
	testEndException(GException);
	testBeginException;
		metaClass->cloneInplace(NULL, buffer);
		metaCheckError(metaClass);
	testEndException(GException);

	metaClass.reset(service->findClassByName("class::TestNoDefaultAndCopyConstructor"));
	testCheckAssert(metaClass);
	testCheckAssert(! metaClass->isAbstract());
	testCheckAssert(! metaClass->canCreateInstance());
	testCheckAssert(! metaClass->canCopyInstance());
	testBeginException;
		metaClass->cloneInstance(NULL);
		metaCheckError(metaClass);
	testEndException(GException);
	testBeginException;
		metaClass->cloneInplace(NULL, buffer);
		metaCheckError(metaClass);
	testEndException(GException);

}


void doTest()
{
	doTestLib();
	doTestAPI();
}


gTestCase(doTest);


}
