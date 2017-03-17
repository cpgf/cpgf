#include "testscriptbindmetadata2.h"
#include "cpgf/gmetadefine.h"

using namespace cpgf;
using namespace std;

namespace testscript {

void TestScriptBindMetaData2()
{
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
		
		._method("testParamReferenceToPointer", &TestObject::testParamReferenceToPointer)
	;
}



}

