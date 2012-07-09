#include "../testscriptbind.h"

#include "cpgf/scriptbind/gscriptbindutil.h"

#include <iostream>
using namespace std;

namespace {


template <typename T>
void doTestOverrideCppFunctionFromScriptClass(T * binding, TestScriptContext * context)
{
	if(context->isLua()) {
		QDO(function funcOverride(my) return 18 end)
	}
	if(context->isV8()) {
		QDO(function funcOverride(my) { return 18; })
	}
	if(context->isPython()) {
		QDO(def funcOverride(my): return 18)
	}

	QNEWOBJ(a, ScriptOverride())
	QASSERT(a.getValue() == 1);
	QDO(ScriptOverride.getValue = funcOverride)
	QASSERT(a.getValue() == 18);

if(context->isPython()) return;
	ScriptOverride * objA = static_cast<ScriptOverride *>(binding->getObject("a"));
cout << typeid(objA).name() << endl;
	GEQUAL(18, objA->getValue());
}

void testOverrideCppFunctionFromScriptClass(TestScriptContext * context)
{
	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();

	if(bindingLib) {
		doTestOverrideCppFunctionFromScriptClass(bindingLib, context);
	}
	
	if(bindingApi) {
		doTestOverrideCppFunctionFromScriptClass(bindingApi, context);
	}
}

#define CASE testOverrideCppFunctionFromScriptClass
#include "../bind_testcase.h"



template <typename T>
void doTestOverrideCppFunctionFromScriptObject(T * binding, TestScriptContext * context)
{
	if(context->isLua()) {
		QDO(function funcOverride(my) return 38 end)
	}
	if(context->isV8()) {
		QDO(function funcOverride(my) { return 38; })
	}
	if(context->isPython()) {
		QDO(def funcOverride(my): return 38)
	}

	QNEWOBJ(a, ScriptOverride())
	QASSERT(a.getValue() == 1);
	QDO(a.getValue = funcOverride)
	QASSERT(a.getValue() == 38);
}

void testOverrideCppFunctionFromScriptObject(TestScriptContext * context)
{
	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();

	if(bindingLib) {
		doTestOverrideCppFunctionFromScriptObject(bindingLib, context);
	}
	
	if(bindingApi) {
		doTestOverrideCppFunctionFromScriptObject(bindingApi, context);
	}
}

#define CASE testOverrideCppFunctionFromScriptObject
#include "../bind_testcase.h"






}
