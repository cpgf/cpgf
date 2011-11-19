#include "cpgf/gmetareflect.h"
#include "cpgf/gscopedptr.h"

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


void bindBasicData(cpgf::GScriptObject * script, cpgf::IMetaService * service)
{
	bindClass(script, service, "testscript::TestObject", "TestObject");
	bindClass(script, service, "testscript::TestData", "TestData");
	bindClass(script, service, "testscript::TestBase", "TestBase");
	bindClass(script, service, "testscript::TestA", "TestA");
	bindClass(script, service, "testscript::TestB", "TestB");
	bindClass(script, service, "testscript::TestC", "TestC");

	script->bindFundamental("Magic1", Magic1);
	script->bindFundamental("Magic2", Magic2);
	script->bindFundamental("Magic3", Magic3);
}


void bindBasicData(cpgf::IScriptObject * script, cpgf::IMetaService * service)
{
	using namespace cpgf;

	bindClass(script, service, "testscript::TestObject", "TestObject");
	bindClass(script, service, "testscript::TestData", "TestData");
	bindClass(script, service, "testscript::TestBase", "TestBase");
	bindClass(script, service, "testscript::TestA", "TestA");
	bindClass(script, service, "testscript::TestB", "TestB");
	bindClass(script, service, "testscript::TestC", "TestC");

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
}



}

