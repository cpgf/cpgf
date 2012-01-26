#include "cpgf/gscopedptr.h"
#include "cpgf/gexception.h"

#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

#include "cpgf/gbytearray.h"
#include "cpgf/gbytearrayapi.h"

#include "cpgf/metadata/util/gmetadata_bytearray.h"

#include "testscriptbindmetadata.h"

#include <string>


using namespace cpgf;

namespace testscript {

int TestObject::staticValue = 0;

GScopedInterface<IScriptFunction> testScriptFunction;

int testAdd2(int a, int b)
{
	return 2 + a + b;
}

int testAddN(const cpgf::GMetaVariadicParam * params)
{
	int total = 3;
	for(size_t i = 0; i < params->paramCount; ++i) {
		total += cpgf::fromVariant<int>(*(params->params[i]));
	}

	return total;
}

int testAddCallback(IScriptFunction * scriptFunction)
{
	return fromVariant<int>(invokeScriptFunction(scriptFunction, 5, 6).getValue());
}

int testAddCallback2(void *)
{
	return 0;
}

void testScriptFunctionSetter(IScriptFunction * scriptFunction)
{
	scriptFunction->addReference();
	testScriptFunction.reset(scriptFunction);
}

IScriptFunction * testScriptFunctionGetter()
{
	return testScriptFunction.get();
}

int testExecAddCallback()
{
	int n = fromVariant<int>(invokeScriptFunction(testScriptFunction.get(), 5, 6).getValue());
	testScriptFunction.reset(); // destroy it to avoid crash between different script engine such as Lua and V8. It's not a bug
	return n;
}

IByteArray * createByteArray()
{
	return byteArrayToInterface(new GByteArray, true);
}

void writeNumberToByteArray(int n, IByteArray * ba)
{
	ba->writeInt32(n);
	ba->writeInt32(n * 2);
}

void writeNumberToByteArrayMemory(int n, void * buffer)
{
	*(int32_t *)buffer = n;
	*(int32_t *)((int8_t *)buffer + sizeof(int32_t)) = n * 2;
}

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


template <typename T>
void bindProperty(T * script, cpgf::IMetaService * service, void * instance, const char * metaName, const char * bindName)
{
	using namespace cpgf;
	
	GScopedInterface<IMetaModule> module(service->getModuleAt(0));
	GScopedInterface<IMetaClass> metaClass(module->getGlobalMetaClass());
	GScopedInterface<IMetaProperty> metaAccessible(metaClass->getProperty(metaName));
	
	script->bindAccessible(bindName, instance, metaAccessible.get());
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

	GScopedInterface<IMetaList> metaList2(createMetaList());
	method.reset(global->getMethod("testAddCallback2"));
	metaList2->add(method.get(), NULL);
	method.reset(global->getMethod("testAddCallback"));
	metaList2->add(method.get(), NULL);
	script->bindMethodList("testAddCallback", metaList2.get());

	bindMethod(script, service, "scriptAssert", "scriptAssert");
	bindMethod(script, service, "scriptNot", "scriptNot");
	bindMethod(script, service, "testExecAddCallback", "testExecAddCallback");
	
	bindMethod(script, service, "createByteArray", "createByteArray");
	bindMethod(script, service, "writeNumberToByteArray", "writeNumberToByteArray");
	bindMethod(script, service, "writeNumberToByteArrayMemory", "writeNumberToByteArrayMemory");
	
	bindEnum(script, service, REG_NAME_TestEnum, "TestEnum");

	bindProperty(script, service, NULL, "testScriptFunction", "testScriptFunction");
	
	bindClass(script, service, "IByteArray", "IByteArray");
}


void bindBasicData(cpgf::GScriptObject * script, cpgf::IMetaService * service)
{
	bindBasicInfo(script, service);

	script->bindFundamental("Magic1", Magic1);
	script->bindFundamental("Magic2", Magic2);
	script->bindFundamental("Magic3", Magic3);

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

	v = testInt;
	script->bindFundamental("testInt", &v.data);
}


void scriptAssert(bool b)
{
	if(! b) {
		cpgf::raiseException(1, "Script assertion failure!");
	}
}

void scriptNot(bool b)
{
	if(b) {
		cpgf::raiseException(1, "Script NOT assertion failure!");
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


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;
	using namespace std;

	GDefineMetaClass<TestObject>
		::define(REG_NAME_TestObject)

		._constructor<void * (const TestObject &)>()
		._constructor<void * (int)>()
		._constructor<void * (int, const string &)>(GMetaPolicyCopyAllConstReference())
	
		._field("value", &TestObject::value)
	
		._method("self", &TestObject::self)
		._method("selfConst", &TestObject::selfConst)
		._method("selfVolatile", &TestObject::selfVolatile)
		._method("selfConstVolatile", &TestObject::selfConstVolatile)
	
		._method("methodConst", (int (TestObject::*)())&TestObject::methodConst)
		._method("methodConst", (int (TestObject::*)() const)&TestObject::methodConst)
	
		._method("methodVolatile", (int (TestObject::*)())&TestObject::methodVolatile)
		._method("methodVolatile", (int (TestObject::*)() volatile)&TestObject::methodVolatile)
	
		._method("methodConstVolatile", (int (TestObject::*)())&TestObject::methodConstVolatile)
		._method("methodConstVolatile", (int (TestObject::*)() const volatile)&TestObject::methodConstVolatile)

		._method("methodOverload", (int (TestObject::*)(const TestObject &, int) const)&TestObject::methodOverload, GMetaPolicyCopyAllConstReference())
		._method("methodOverload", (int (TestObject::*)(int, const TestObject &) const)&TestObject::methodOverload, GMetaPolicyCopyAllConstReference())
		._method("methodOverload", (int (TestObject::*)(int, int) const)&TestObject::methodOverload)
		._method("methodOverload", (int (TestObject::*)(const char *, int) const)&TestObject::methodOverload)
		._method("methodOverload", (int (TestObject::*)(int, const char *) const)&TestObject::methodOverload)
		._method("methodOverload", (int (TestObject::*)(const string &, int) const)&TestObject::methodOverload, GMetaPolicyCopyAllConstReference())
		._method("methodOverload", (int (TestObject::*)(int, const string &) const)&TestObject::methodOverload, GMetaPolicyCopyAllConstReference())

		._method("methodOverloadObject", (int (TestObject::*)(const TestBase *) const)&TestObject::methodOverloadObject)
		._method("methodOverloadObject", (int (TestObject::*)(const TestA *) const)&TestObject::methodOverloadObject)
		._method("methodOverloadObject", (int (TestObject::*)(const TestB *) const)&TestObject::methodOverloadObject)
		._method("methodOverloadObject", (int (TestObject::*)(const TestC *) const)&TestObject::methodOverloadObject)

		._method("add", &TestObject::add)
	
		._field("staticValue", &TestObject::staticValue)
		._method("incStaticValue", &TestObject::incStaticValue)

		._field("data", &TestObject::data)
	
		._field("raw", &TestObject::raw)
		._field("rawPointer", &TestObject::rawPointer)

		._method("pointerRaw", &TestObject::pointerRaw)
		._method("refRaw", &TestObject::refRaw)
		._method("isRawPointer", &TestObject::isRawPointer)
		._method("isRawRef", &TestObject::isRawRef)
		._method("setRaw", &TestObject::setRaw)
	
		._method("refData", &TestObject::refData)
		._method("pointerData", &TestObject::pointerData)
		._method("constRefData", &TestObject::constRefData)
		._method("constPointerData", &TestObject::constPointerData)
		
		._method("scriptObjectCallback", &TestObject::scriptObjectCallback)
		._method("getVariant", &TestObject::getVariant)
	;
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
			GDefineMetaClass<BasicA::Inner>::declare("Inner")
				._field("x", &BasicA::Inner::x)
				._method("add", &BasicA::Inner::add)
		)
		._enum<BasicA::BasicEnum>("BasicEnum")
			._element("a", BasicA::a)
			._element("b", BasicA::b)
			._element("c", BasicA::c)
	;
}


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<IByteArray> byteArrayDefine = GDefineMetaClass<IByteArray>::define("IByteArray");
	buildMetaData_byteArray(byteArrayDefine);

	GDefineMetaGlobal()
		._method("scriptAssert", &scriptAssert)
		._method("scriptNot", &scriptNot)
		._method("testAdd2", &testAdd2)
		._method("testAddN", &testAddN)
		._method("testAddCallback2", &testAddCallback2)
		._method("testAddCallback", &testAddCallback)
		._method("testExecAddCallback", &testExecAddCallback)
		
		._method("createByteArray", &createByteArray, GMetaPolicyTransferResultOwnership())
		._method("writeNumberToByteArray", &writeNumberToByteArray)
		._method("writeNumberToByteArrayMemory", &writeNumberToByteArrayMemory)
		
		._enum<TestEnum>(REG_NAME_TestEnum)
			._element("teCpp", teCpp)
			._element("teLua", teLua)
			._element("teV8", teV8)
		._property("testScriptFunction", &testScriptFunctionGetter, &testScriptFunctionSetter)
	;
}


}

