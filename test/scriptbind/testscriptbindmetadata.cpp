#include "cpgf/gmetareflect.h"
#include "cpgf/gscopedptr.h"
#include "cpgf/gexception.h"

#include "testscriptbindmetadata.h"

#include <string>

namespace testscript {

int TestObject::staticValue = 0;

template <typename T>
void bindClass(T * script, cpgf::IMetaService * service, const char * metaName, const char * bindName)
{
	using namespace cpgf;
	
	GScopedInterface<IMetaClass> metaClass(service->findClassByName(metaName));
	
	script->bindClass(bindName, metaClass.get());
}


template <typename T>
void bindMethod(T * script, cpgf::IMetaService * service, const char * metaName, const char * bindName)
{
	using namespace cpgf;
	
	GScopedInterface<IMetaModule> module(service->getModuleAt(0));
	GScopedInterface<IMetaClass> metaClass(module->getGlobalMetaClass());
	GScopedInterface<IMetaMethod> method(metaClass->getMethod(metaName));
	
	script->bindMethod(bindName, NULL, method.get());
}


template <typename T>
void bindEnum(T * script, cpgf::IMetaService * service, const char * metaName, const char * bindName)
{
	using namespace cpgf;
	
	GScopedInterface<IMetaModule> module(service->getModuleAt(0));
	GScopedInterface<IMetaClass> metaClass(module->getGlobalMetaClass());
	GScopedInterface<IMetaEnum> metaEnum(metaClass->getEnum(metaName));
	
	script->bindEnum(bindName, metaEnum.get());
}


namespace {
	TestObject testObject = TestObject(38);
}

template <typename T>
void bindBasicInfo(T * script, cpgf::IMetaService * service)
{
	using namespace cpgf;
	
	bindClass(script, service, REG_NAME_TestObject, "TestObject");
	bindClass(script, service, REG_NAME_TestData, "TestData");
	bindClass(script, service, "testscript::TestBase", "TestBase");
	bindClass(script, service, "testscript::TestOperator", "TestOperator");
	
	bindClass(script, service, "testscript::TestA", "TestA");
	bindClass(script, service, "testscript::TestB", "TestB");
	bindClass(script, service, "testscript::TestC", "TestC");

	bindClass(script, service, "testscript::DeriveA", "DeriveA");
	bindClass(script, service, "testscript::DeriveB", "DeriveB");
	bindClass(script, service, "testscript::DeriveC", "DeriveC");
	bindClass(script, service, "testscript::DeriveD", "DeriveD");
	bindClass(script, service, "testscript::DeriveE", "DeriveE");

	bindClass(script, service, REG_NAME_BasicA, "BasicA");
	
	script->bindString("testString", testString);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(REG_NAME_TestObject));
	TestObject * obj = new TestObject(testObjValue);
	script->bindObject("testObj", obj, metaClass.get(), true);

	GScopedInterface<IMetaModule> module(service->getModuleAt(0));
	GScopedInterface<IMetaClass> global(module->getGlobalMetaClass());
	GScopedInterface<IMetaMethod> method;
	GScopedInterface<IMetaList> metaList(createMetaList());

	method.reset(metaClass->getMethod("add"));
	metaList->add(method.get(), &testObject);
	method.reset(global->getMethod("testAdd2"));
	metaList->add(method.get(), NULL);
	method.reset(global->getMethod("testAddN"));
	metaList->add(method.get(), NULL);
	script->bindMethodList("testAdd", metaList.get());
}


void bindBasicData(cpgf::GScriptObject * script, cpgf::IMetaService * service)
{
	bindBasicInfo(script, service);

	script->bindFundamental("Magic1", Magic1);
	script->bindFundamental("Magic2", Magic2);
	script->bindFundamental("Magic3", Magic3);

	bindMethod(script, service, "scriptAssert", "scriptAssert");
	bindEnum(script, service, REG_NAME_TestEnum, "TestEnum");

	script->bindFundamental("testInt", testInt);
}


void bindBasicData(cpgf::IScriptObject * script, cpgf::IMetaService * service)
{
	using namespace cpgf;

	bindBasicInfo(script, service);

	GVariant v;

	v = Magic1;
	script->bindFundamental("Magic1", &v.data);
	
	v = Magic2;
	script->bindFundamental("Magic2", &v.data);
	
	v = Magic3;
	script->bindFundamental("Magic3", &v.data);

	bindMethod(script, service, "scriptAssert", "scriptAssert");
	bindEnum(script, service, REG_NAME_TestEnum, "TestEnum");

	v = testInt;
	script->bindFundamental("testInt", &v.data);
}


void scriptAssert(bool b)
{
	if(! b) {
		cpgf::raiseException(1, "Script assertion failure!");
	}
}

GMETA_DEFINE_CLASS(TestData, TestData, REG_NAME_TestData) {
	using namespace cpgf;

	GMETA_FIELD(x);
	GMETA_FIELD(name);
}

GMETA_DEFINE_CLASS(TestBase, TestBase, "testscript::TestBase") {
	using namespace cpgf;

	GMETA_METHOD(getValue);
}

GMETA_DEFINE_CLASS(TestA, TestA, "testscript::TestA", TestBase) {
	using namespace cpgf;

	GMETA_METHOD(getValue);
}

GMETA_DEFINE_CLASS(TestB, TestB, "testscript::TestB", TestBase) {
	using namespace cpgf;

	GMETA_METHOD(getValue);
}

GMETA_DEFINE_CLASS(TestC, TestC, "testscript::TestC", TestB) {
	using namespace cpgf;

	GMETA_METHOD(getValue);
}

GMETA_DEFINE_CLASS(TestObject, TestObject, REG_NAME_TestObject) {
	using namespace cpgf;
	using namespace std;

	reflectConstructor<void * (const TestObject &)>();
	reflectConstructor<void * (int)>();
	reflectConstructor<void * (int, const string &)>(GMetaPolicyCopyAllConstReference());
	
	GMETA_FIELD(value);
	
	GMETA_METHOD(self);
	GMETA_METHOD(selfConst);
	GMETA_METHOD(selfVolatile);
	GMETA_METHOD(selfConstVolatile);
	
	reflectMethod("methodConst", (int (TestObject::*)())&TestObject::methodConst);
	reflectMethod("methodConst", (int (TestObject::*)() const)&TestObject::methodConst);
	
	reflectMethod("methodVolatile", (int (TestObject::*)())&TestObject::methodVolatile);
	reflectMethod("methodVolatile", (int (TestObject::*)() volatile)&TestObject::methodVolatile);
	
	reflectMethod("methodConstVolatile", (int (TestObject::*)())&TestObject::methodConstVolatile);
	reflectMethod("methodConstVolatile", (int (TestObject::*)() const volatile)&TestObject::methodConstVolatile);

	reflectMethod("methodOverload", (int (TestObject::*)(const TestObject &, int) const)&TestObject::methodOverload, GMetaPolicyCopyAllConstReference());
	reflectMethod("methodOverload", (int (TestObject::*)(int, const TestObject &) const)&TestObject::methodOverload, GMetaPolicyCopyAllConstReference());
	reflectMethod("methodOverload", (int (TestObject::*)(int, int) const)&TestObject::methodOverload);
	reflectMethod("methodOverload", (int (TestObject::*)(const char *, int) const)&TestObject::methodOverload);
	reflectMethod("methodOverload", (int (TestObject::*)(int, const char *) const)&TestObject::methodOverload);
	reflectMethod("methodOverload", (int (TestObject::*)(const string &, int) const)&TestObject::methodOverload, GMetaPolicyCopyAllConstReference());
	reflectMethod("methodOverload", (int (TestObject::*)(int, const string &) const)&TestObject::methodOverload, GMetaPolicyCopyAllConstReference());

	reflectMethod("methodOverloadObject", (int (TestObject::*)(const TestBase *) const)&TestObject::methodOverloadObject);
	reflectMethod("methodOverloadObject", (int (TestObject::*)(const TestA *) const)&TestObject::methodOverloadObject);
	reflectMethod("methodOverloadObject", (int (TestObject::*)(const TestB *) const)&TestObject::methodOverloadObject);
	reflectMethod("methodOverloadObject", (int (TestObject::*)(const TestC *) const)&TestObject::methodOverloadObject);

	reflectMethod("add", &TestObject::add);
	
	reflectField("staticValue", &TestObject::staticValue);
	reflectMethod("incStaticValue", &TestObject::incStaticValue);

	GMETA_FIELD(raw);
	GMETA_FIELD(rawPointer);

	GMETA_METHOD(pointerRaw);
	GMETA_METHOD(refRaw);
	GMETA_METHOD(isRawPointer);
	GMETA_METHOD(isRawRef);
	GMETA_METHOD(setRaw);
}


GMETA_DEFINE_CLASS(TestOperator, TestOperator, "testscript::TestOperator") {
	using namespace cpgf;
	using namespace std;

	reflectConstructor<void * (const TestOperator &)>();
	reflectConstructor<void * (int)>();
	
	GMETA_FIELD(value);
	
#define M(OP, RET) \
	reflectOperator<RET (const GMetaSelf, int)>(mopHolder OP mopHolder); \
	reflectOperator<RET (const GMetaSelf, const TestOperator &)>(mopHolder OP mopHolder); \
	reflectOperator<RET (const GMetaSelf, const TestObject &)>(mopHolder OP mopHolder);
	
	M(+, TestOperator)
	M(-, TestOperator)
	M(*, TestOperator)
	M(/, TestOperator)
	M(%, TestOperator)

	M(==, bool)
	M(<, bool)
	M(<=, bool)

#undef M

	reflectOperator<TestOperator (const GMetaSelf)>(-mopHolder);

	reflectOperator<TestOperator (const std::string &, int)>(mopHolder(mopHolder), GMetaPolicyCopyAllConstReference());
	reflectOperator<int (const GMetaVariadicParam *)>(mopHolder(mopHolder));

}

GMETA_DEFINE_CLASS(DeriveA, DeriveA, "testscript::DeriveA") {
	using namespace cpgf;

	GMETA_METHOD(getA);
	GMETA_METHOD(setData);

	GMETA_FIELD(a);
}

GMETA_DEFINE_CLASS(DeriveB, DeriveB, "testscript::DeriveB", DeriveA) {
	using namespace cpgf;

	GMETA_METHOD(getB);

	GMETA_FIELD(b);
}

GMETA_DEFINE_CLASS(DeriveC, DeriveC, "testscript::DeriveC", DeriveA) {
	using namespace cpgf;

	GMETA_METHOD(getC);

	GMETA_FIELD(c);
}

GMETA_DEFINE_CLASS(DeriveD, DeriveD, "testscript::DeriveD", DeriveB, DeriveC) {
	using namespace cpgf;

	GMETA_METHOD(getD);

	GMETA_FIELD(d);
}

GMETA_DEFINE_CLASS(DeriveE, DeriveE, "testscript::DeriveE", DeriveD) {
	using namespace cpgf;

	GMETA_METHOD(getE);
	reflectMethod("pretendA", &DeriveE::pretendA, GMetaPolicyTransferResultOwnership());

	GMETA_FIELD(e);
}



GMETA_DEFINE_CLASS(BasicA::Inner, BasicA_Inner, "Inner") {
	using namespace cpgf;
		
	GMETA_FIELD(x);
	GMETA_METHOD(add);
}
	
GMETA_DEFINE_CLASS(BasicA, BasicA, REG_NAME_BasicA) {
	using namespace cpgf;

	GMETA_QUALIFIED_CLASS(BasicA::Inner);
	
	reflectEnum<BasicA::BasicEnum>("BasicEnum")
		("a", BasicA::a)
		("b", BasicA::b)
		("c", BasicA::c);
}



GMETA_DEFINE_GLOBAL() {
	using namespace cpgf;

	GMETA_QUALIFIED_METHOD(scriptAssert);
	GMETA_QUALIFIED_METHOD(testAdd2);
	GMETA_QUALIFIED_METHOD(testAddN);
	
	reflectEnum<TestEnum>(REG_NAME_TestEnum)
		("teCpp", teCpp)
		("teLua", teLua)
		("teV8", teV8);
}


}

