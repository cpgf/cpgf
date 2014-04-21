#include "cpgf/gscopedptr.h"
#include "cpgf/gexception.h"

#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

#include "testscriptbindmetadata.h"
#include "testscriptbindmetadata2.h"

#include <string>
#include <iostream>


using namespace cpgf;
using namespace std;

namespace testscript {

void TestScriptBindMetaData1();
void TestScriptBindMetaData2();
void TestScriptBindMetaData3();
void TestScriptBindMetaData4();
void TestScriptBindMetaData5();

int TestObject::staticValue = 0;

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

template <typename T>
void bindClass(T * script, cpgf::IMetaService * service, const char * metaName, const char * bindName)
{
	using namespace cpgf;
	
	GScopedInterface<IMetaClass> metaClass(service->findClassByName(metaName));
	
	scriptSetValue(script, bindName, GScriptValue::fromClass(metaClass.get()));
}


template <typename T>
void bindMethod(T * script, cpgf::IMetaService * service, const char * metaName, const char * bindName)
{
	using namespace cpgf;
	
	GScopedInterface<IMetaModule> module(service->getModuleAt(0));
	GScopedInterface<IMetaClass> metaClass(module->getGlobalMetaClass());
	GScopedInterface<IMetaMethod> method(metaClass->getMethod(metaName));
	
	scriptSetValue(script, bindName, GScriptValue::fromMethod(NULL, method.get()));
}


template <typename T>
void bindEnum(T * script, cpgf::IMetaService * service, const char * metaName, const char * bindName)
{
	using namespace cpgf;
	
	GScopedInterface<IMetaModule> module(service->getModuleAt(0));
	GScopedInterface<IMetaClass> metaClass(module->getGlobalMetaClass());
	GScopedInterface<IMetaEnum> metaEnum(metaClass->getEnum(metaName));
	
	scriptSetValue(script, bindName, GScriptValue::fromEnum(metaEnum.get()));
}


template <typename T>
void bindProperty(T * script, cpgf::IMetaService * service, void * instance, const char * metaName, const char * bindName)
{
	using namespace cpgf;
	
	GScopedInterface<IMetaModule> module(service->getModuleAt(0));
	GScopedInterface<IMetaClass> metaClass(module->getGlobalMetaClass());
	GScopedInterface<IMetaProperty> metaAccessible(metaClass->getProperty(metaName));
	
	scriptSetValue(script, bindName, GScriptValue::fromAccessible(instance, metaAccessible.get()));
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
	bindClass(script, service, "testscript::ScriptOverride", "ScriptOverride");

	bindClass(script, service, REG_NAME_BasicA, "BasicA");
	
	scriptSetValue(script, "testString", GScriptValue::fromString(testString));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(REG_NAME_TestObject));
	TestObject * obj = new TestObject(testObjValue);
	scriptSetValue(script, "testObj", GScriptValue::fromObject(obj, metaClass.get(), true));

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
	scriptSetValue(script, "testAdd", GScriptValue::fromOverloadedMethods(metaList.get()));

	GScopedInterface<IMetaList> metaList2(createMetaList());
	method.reset(global->getMethod("testAddCallback2"));
	metaList2->add(method.get(), NULL);
	method.reset(global->getMethod("testAddCallback"));
	metaList2->add(method.get(), NULL);
	scriptSetValue(script, "testAddCallback", GScriptValue::fromOverloadedMethods(metaList2.get()));

	bindMethod(script, service, "scriptAssert", "scriptAssert");
	bindMethod(script, service, "scriptNot", "scriptNot");
	bindMethod(script, service, "testExecAddCallback", "testExecAddCallback");
	
	bindMethod(script, service, "writeNumberToByteArray", "writeNumberToByteArray");
	bindMethod(script, service, "writeNumberToByteArrayMemory", "writeNumberToByteArrayMemory");
	bindMethod(script, service, "testDefaultParam", "testDefaultParam");
	bindMethod(script, service, "testWideStringParam", "testWideStringParam");
	
	bindEnum(script, service, REG_NAME_TestEnum, "TestEnum");

	bindProperty(script, service, NULL, "testScriptFunction", "testScriptFunction");
}


void bindBasicData(cpgf::GScriptObject * script, cpgf::IMetaService * service)
{
	bindBasicInfo(script, service);

	scriptSetValue(script, "Magic1", GScriptValue::fromFundamental(Magic1));
	scriptSetValue(script, "Magic2", GScriptValue::fromFundamental(Magic2));
	scriptSetValue(script, "Magic3", GScriptValue::fromFundamental(Magic3));

	scriptSetValue(script, "testInt", GScriptValue::fromFundamental(testInt));
}


void bindBasicData(cpgf::IScriptObject * script, cpgf::IMetaService * service)
{
	using namespace cpgf;

	bindBasicInfo(script, service);

	GVariant v;

	v = Magic1;
	scriptSetValue(script, "Magic1", GScriptValue::fromFundamental(v));
	
	v = Magic2;
	scriptSetValue(script, "Magic2", GScriptValue::fromFundamental(v));
	
	v = Magic3;
	scriptSetValue(script, "Magic3", GScriptValue::fromFundamental(v));

	v = testInt;
	scriptSetValue(script, "testInt", GScriptValue::fromFundamental(v));
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
	TestScriptBindMetaData1();
	TestScriptBindMetaData2();
	TestScriptBindMetaData3();
	TestScriptBindMetaData4();
	TestScriptBindMetaData5();
}




}

