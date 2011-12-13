#include "cpgf/gmetareflect.h"
#include "cpgf/gscopedptr.h"
#include "cpgf/gexception.h"

#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

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

G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<TestData>
		::define(REG_NAME_TestData)
		._field("x", &TestData::x)
		._field("name", &TestData::name)
	;
}


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<TestBase>
		::define("testscript::TestBase")
		._method("getValue", &TestBase::getValue);
	;

	GDefineMetaClass<TestA, TestBase>
		::define("testscript::TestA")
	;

	GDefineMetaClass<TestB, TestBase>
		::define("testscript::TestB")
	;

	GDefineMetaClass<TestC, TestB>
		::define("testscript::TestC")
	;
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

	GMETA_FIELD(data);
	
	GMETA_FIELD(raw);
	GMETA_FIELD(rawPointer);

	GMETA_METHOD(pointerRaw);
	GMETA_METHOD(refRaw);
	GMETA_METHOD(isRawPointer);
	GMETA_METHOD(isRawRef);
	GMETA_METHOD(setRaw);
	
	GMETA_METHOD(refData);
	GMETA_METHOD(pointerData);
	GMETA_METHOD(constRefData);
	GMETA_METHOD(constPointerData);
}

G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;
	using namespace std;

	GDefineMetaClass<TestOperator>
		::define("testscript::TestOperator")

		._constructor<void * (const TestOperator &)>()
		._constructor<void * (int)>()
		
		._field("value", &TestOperator::value)
		
		._operator<TestOperator (const GMetaSelf)>(-mopHolder)
		._operator<TestOperator (const std::string &, int)>(mopHolder(mopHolder), GMetaPolicyCopyAllConstReference())
		._operator<int (const GMetaVariadicParam *)>(mopHolder(mopHolder))
#define M(OP, RET) \
		._operator<RET (const GMetaSelf, int)>(mopHolder OP mopHolder) \
		._operator<RET (const GMetaSelf, const TestOperator &)>(mopHolder OP mopHolder) \
		._operator<RET (const GMetaSelf, const TestObject &)>(mopHolder OP mopHolder)
	
		M(+, TestOperator)
		M(-, TestOperator)
		M(*, TestOperator)
		M(/, TestOperator)
		M(%, TestOperator)

		M(==, bool)
		M(<, bool)
		M(<=, bool)

#undef M
	;
}

G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<DeriveA>
		::define("testscript::DeriveA")
		._method("getA", &DeriveA::getA)
		._method("setData", &DeriveA::setData)
		._field("a", &DeriveA::a)
	;

	GDefineMetaClass<DeriveB, DeriveA>
		::define("testscript::DeriveB")
		._method("getB", &DeriveB::getB)
		._field("b", &DeriveB::b)
	;

	GDefineMetaClass<DeriveC, DeriveA>
		::define("testscript::DeriveC")
		._method("getC", &DeriveC::getC)
		._field("c", &DeriveC::c)
	;

	GDefineMetaClass<DeriveD, DeriveB, DeriveC>
		::define("testscript::DeriveD")
		._method("getD", &DeriveD::getD)
		._field("d", &DeriveD::d)
	;

	GDefineMetaClass<DeriveE, DeriveD>
		::define("testscript::DeriveE")
		._method("getE", &DeriveE::getE)
		._method("pretendA", &DeriveE::pretendA, GMetaPolicyTransferResultOwnership())
		._field("e", &DeriveE::e)
	;
}


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<BasicA>
		::define(REG_NAME_BasicA)
		._class(
			GDefineMetaClass<BasicA::Inner>::inner("Inner")
				._field("x", &BasicA::Inner::x)
				._method("add", &BasicA::Inner::add)
		)
		._enum<BasicA::BasicEnum>("BasicEnum")
			._value("a", BasicA::a)
			._value("b", BasicA::b)
			._value("c", BasicA::c)
	;
}


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaGlobal()
		._method("scriptAssert", &scriptAssert)
		._method("testAdd2", &testAdd2)
		._method("testAddN", &testAddN)
		._enum<TestEnum>(REG_NAME_TestEnum)
			._value("teCpp", teCpp)
			._value("teLua", teLua)
			._value("teV8", teV8)
	;
}


}

