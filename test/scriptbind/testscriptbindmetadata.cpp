#include "cpgf/gmetareflect.h"
#include "cpgf/gscopedptr.h"
#include "cpgf/gexception.h"

#include "testscriptbindmetadata.h"

#include <string>

namespace testscript {

void bindClass(cpgf::GScriptObject * script, cpgf::IMetaService * service, const char * metaName, const char * bindName)
{
	using namespace cpgf;
	
	GScopedInterface<IMetaClass> metaClass(service->findClassByName(metaName));
	
	script->bindClass(bindName, metaClass.get());
}


void bindClass(cpgf::IScriptObject * script, cpgf::IMetaService * service, const char * metaName, const char * bindName)
{
	using namespace cpgf;
	
	GScopedInterface<IMetaClass> metaClass(service->findClassByName(metaName));
	
	GScopedInterface<IScriptName> scriptName;

	scriptName.reset(script->createName(bindName));
	script->bindClass(scriptName.get(), metaClass.get());
}

void bindMethod(cpgf::GScriptObject * script, cpgf::IMetaService * service, const char * metaName, const char * bindName)
{
	using namespace cpgf;
	
	GScopedInterface<IMetaModule> module(service->getModuleAt(0));
	GScopedInterface<IMetaClass> metaClass(module->getGlobalMetaClass());
	GScopedInterface<IMetaMethod> method(metaClass->getMethod(metaName));
	
	script->bindMethod(bindName, NULL, method.get());
}


void bindMethod(cpgf::IScriptObject * script, cpgf::IMetaService * service, const char * metaName, const char * bindName)
{
	using namespace cpgf;
	
	GScopedInterface<IMetaModule> module(service->getModuleAt(0));
	GScopedInterface<IMetaClass> metaClass(module->getGlobalMetaClass());
	GScopedInterface<IMetaMethod> method(metaClass->getMethod(metaName));
	
	GScopedInterface<IScriptName> scriptName;

	scriptName.reset(script->createName(bindName));
	script->bindMethod(scriptName.get(), NULL, method.get());
}

template <typename T>
void bindBasicClass(T * script, cpgf::IMetaService * service)
{
	bindClass(script, service, "testscript::TestObject", "TestObject");
	bindClass(script, service, "testscript::TestData", "TestData");
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
}


void bindBasicData(cpgf::GScriptObject * script, cpgf::IMetaService * service)
{
	bindBasicClass(script, service);

	script->bindFundamental("Magic1", Magic1);
	script->bindFundamental("Magic2", Magic2);
	script->bindFundamental("Magic3", Magic3);

	bindMethod(script, service, "scriptAssert", "scriptAssert");
}


void bindBasicData(cpgf::IScriptObject * script, cpgf::IMetaService * service)
{
	using namespace cpgf;

	bindBasicClass(script, service);

	GScopedInterface<IScriptName> scriptName;

	GVariant v;

	v = Magic1;
	scriptName.reset(script->createName("Magic1"));
	script->bindFundamental(scriptName.get(), &v.data);
	
	v = Magic2;
	scriptName.reset(script->createName("Magic2"));
	script->bindFundamental(scriptName.get(), &v.data);
	
	v = Magic3;
	scriptName.reset(script->createName("Magic3"));
	script->bindFundamental(scriptName.get(), &v.data);

	bindMethod(script, service, "scriptAssert", "scriptAssert");
}


void scriptAssert(bool b)
{
	if(! b) {
		cpgf::raiseException(1, "Script assertion failure!");
	}
}

GMETA_DEFINE_CLASS(TestData, TestData, "testscript::TestData") {
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

GMETA_DEFINE_CLASS(TestObject, TestObject, "testscript::TestObject") {
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
}

GMETA_DEFINE_CLASS(DeriveB, DeriveB, "testscript::DeriveB", DeriveA) {
	using namespace cpgf;

	GMETA_METHOD(getB);
}

GMETA_DEFINE_CLASS(DeriveC, DeriveC, "testscript::DeriveC", DeriveA) {
	using namespace cpgf;

	GMETA_METHOD(getC);
}

GMETA_DEFINE_CLASS(DeriveD, DeriveD, "testscript::DeriveD", DeriveB, DeriveC) {
	using namespace cpgf;

	GMETA_METHOD(getD);
}

GMETA_DEFINE_CLASS(DeriveE, DeriveE, "testscript::DeriveE", DeriveD) {
	using namespace cpgf;

	GMETA_METHOD(getE);
	reflectMethod("pretendA", &DeriveE::pretendA, GMetaPolicyTransferResultOwnership());
}



GMETA_DEFINE_GLOBAL() {
	using namespace cpgf;

	reflectMethod("scriptAssert", &scriptAssert);
}


}

