#include "../testscriptbindmetadata5.h"
#include "../testscriptbind.h"

#include "cpgf/scriptbind/gscriptbindutil.h"


namespace {


template <typename T>
void doTestNewDataProperty(T * /*binding*/, TestScriptContext * context)
{
	QNEWOBJ(a, TestObject())
	QDO(a.veryNewNumericDataProperty = 38);
	QASSERT(a.veryNewNumericDataProperty == 38);

	QDO(a.veryNewStringDataProperty = "abcde");
	QASSERT(a.veryNewStringDataProperty == "abcde");
}

void testNewDataProperty(TestScriptContext * context)
{
	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();

	if(bindingLib) {
		doTestNewDataProperty(bindingLib, context);
	}

	if(bindingApi) {
		doTestNewDataProperty(bindingApi, context);
	}
}

#define CASE testNewDataProperty
#include "../bind_testcase.h"


template <typename T>
void doTestNewMethodProperty(T * /*binding*/, TestScriptContext * context)
{
	if(context->isLua()) {
		QDO(function funcOverride() return 58 end)
	}
	if(context->isV8() || context->isSpiderMonkey()) {
		QDO(function funcOverride() { return 58; })
	}
	if(context->isPython()) {
		QDO(def funcOverride(): return 58)
	}

	QNEWOBJ(a, TestObject())
	QDO(a.veryNewMethodProperty = funcOverride);
	QASSERT(a.veryNewMethodProperty() == 58);
}

void testNewMethodProperty(TestScriptContext * context)
{
	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();

	if(bindingLib) {
		doTestNewMethodProperty(bindingLib, context);
	}

	if(bindingApi) {
		doTestNewMethodProperty(bindingApi, context);
	}
}

#define CASE testNewMethodProperty
#include "../bind_testcase.h"


string getCodeScriptArray(TestScriptContext * context, const string & elements)
{
	string code;

	if(context->isLua()) {
		code = "{" + elements + "}";
	}
	else if(context->isPython()) {
		code = "[" + elements + "]";
	}
	else if(context->isJavascript()) {
		code = "[" + elements + "]";
	}

	return code;
}

void doCreateScriptArray(TestScriptContext * context, const string & name, const string & elements)
{
	string code = VAR + name + " = " + getCodeScriptArray(context, elements);
	DO(code);
}

string doCreateScriptArrayIndex(TestScriptContext * context, const string & name, int index)
{
	char buffer[1024];

	if(context->isLua()) {
		++index;
	}

	sprintf(buffer, "%s[%d]", name.c_str(), index);
	return buffer;
}

template <typename T>
void doTestNewArrayProperty(T * /*binding*/, TestScriptContext * context)
{
	doCreateScriptArray(context, "myArray", "99, 98, 97");

	QNEWOBJ(a, TestObject())
	QDO(a.veryNewArrayProperty = myArray);
	DOASSERT(doCreateScriptArrayIndex(context, "myArray", 0) + " == 99");
	DOASSERT(doCreateScriptArrayIndex(context, "myArray", 1) + " == 98");
}

void testNewArrayProperty(TestScriptContext * context)
{
	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();

	if(bindingLib) {
		doTestNewArrayProperty(bindingLib, context);
	}

	if(bindingApi) {
		doTestNewArrayProperty(bindingApi, context);
	}
}

#define CASE testNewArrayProperty
#include "../bind_testcase.h"


string makeObject(const string & name, TestScriptContext * context)
{
	if(context->isLua()) {
		return "" + name + (name == "" ? "" : " = ") + " { myInt = 20 }";
	}
	if(context->isV8() || context->isSpiderMonkey()) {
		return "function xxx() { this.myInt = 20; }; " + name + " = new xxx();";
	}
	if(context->isPython()) {
		return "" + name + (name == "" ? "" : " = ") + " { 'myInt' : 20 }";
	}

	return "";
}

template <typename T>
void doTestNewObjectProperty(T * /*binding*/, TestScriptContext * context)
{
	DO(makeObject("myObject", context))

	QNEWOBJ(a, TestObject())
	QDO(a.veryNewObjectProperty = myObject);
	if(context->isPython()) {
		QASSERT(a.veryNewObjectProperty['myInt'] == 20);
	}
	else {
		QASSERT(a.veryNewObjectProperty.myInt == 20);
	}
}

void testNewObjectProperty(TestScriptContext * context)
{
	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();

	if(bindingLib) {
		doTestNewObjectProperty(bindingLib, context);
	}

	if(bindingApi) {
		doTestNewObjectProperty(bindingApi, context);
	}
}

#define CASE testNewObjectProperty
#include "../bind_testcase.h"



}
