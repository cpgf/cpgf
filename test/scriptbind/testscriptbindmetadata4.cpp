#include "testscriptbindmetadata4.h"
#include "testscriptbindmetadata2.h"
#include "cpgf/gmetadefine.h"

#include "cpgf/metautility/gmetabytearray.h"

#include <iostream>


using namespace cpgf;
using namespace std;

namespace testscript {

GScopedInterface<IScriptFunction> testScriptFunction;

void scriptTrace(const char * s)
{
	cout << "script message: " << s << endl;
}

int testAddCallback2(void *)
{
	return 0;
}

void writeNumberToByteArray(int n, GMetaByteArray * ba)
{
	ba->writeInt32(n);
	ba->writeInt32(n * 2);
}

void writeNumberToByteArrayMemory(int n, void * buffer)
{
	*(int32_t *)buffer = n;
	*(int32_t *)((int8_t *)buffer + sizeof(int32_t)) = n * 2;
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

bool testDefaultParam(int type, int i, std::string s, TestObject obj)
{
	switch(type) {
		case 0: // all are default
			return i == 5 && s == "abc" && obj.value == 8;

		case 1: // i is 98
			return i == 98 && s == "abc" && obj.value == 8;

		case 2: // i is 98, s is "def"
			return i == 98 && s == "def" && obj.value == 8;

		case 3: // i is 98, s is "def", obj.value is 38
			return i == 98 && s == "def" && obj.value == 38;

		default:
			return false;
	}
}

const wchar_t * testWideStringParam(const wchar_t * ws)
{
	static const wchar_t * isTrue = L"TRUE";
	static const wchar_t * isFalse = L"FALSE";
	
	if(wstring(ws) == L"WideTest") {
		return isTrue;
	}
	else {
		return isFalse;
	}
}

void TestScriptBindMetaData4()
{
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

	GDefineMetaGlobal()
		._method("scriptTrace", &scriptTrace)
		._method("scriptAssert", &scriptAssert)
		._method("scriptNot", &scriptNot)
		._method("testAdd2", &testAdd2)
		._method("testAddN", &testAddN)
		._method("testAddCallback2", &testAddCallback2)
		._method("testAddCallback", &testAddCallback)
		._method("testExecAddCallback", &testExecAddCallback)
		
		._method("writeNumberToByteArray", &writeNumberToByteArray)
		._method("writeNumberToByteArrayMemory", &writeNumberToByteArrayMemory)

		._method("testDefaultParam", &testDefaultParam)
			._default(copyVariantFromCopyable(TestObject(8)))
			._default(copyVariantFromCopyable(string("abc")))
			._default(5)
			
		._method("testWideStringParam", &testWideStringParam)
		
		._enum<TestEnum>(REG_NAME_TestEnum)
			._element("teCpp", teCpp)
			._element("teLua", teLua)
			._element("teV8", teV8)
		._property("testScriptFunction", &testScriptFunctionGetter, &testScriptFunctionSetter)
	;
}

}

