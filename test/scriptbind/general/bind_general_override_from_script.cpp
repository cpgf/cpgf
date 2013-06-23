#include "../testscriptbindmetadata5.h"
#include "../testscriptbind.h"

#include "cpgf/scriptbind/gscriptbindutil.h"


namespace {


template <typename T>
void doTestOverrideCppFunctionFromScriptClass(T * binding, TestScriptContext * context)
{
	if(context->isLua()) {
		QDO(function funcOverride(me) return me.n + 15 end)
	}
	if(context->isV8() || context->isSpiderMonkey()) {
		QDO(function funcOverride(me) { return me.n + 15; })
	}
	if(context->isPython()) {
		QDO(def funcOverride(me): return me.n + 15)
	}

	QNEWOBJ(a, ScriptOverride(3))
	QASSERT(a.getValue() == 3);
	QDO(ScriptOverride.getValue = funcOverride)
	QASSERT(a.getValue() == 18);

	ScriptOverride * objA = static_cast<ScriptOverride *>(scriptGetValue(binding, "a").toObjectAddress(NULL, NULL));
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
		QDO(function funcOverrideA(me) return 38 end)
		QDO(function funcOverrideB(me) return 18 end)
	}
	if(context->isV8() || context->isSpiderMonkey()) {
		QDO(function funcOverrideA(me) { return 38; })
		QDO(function funcOverrideB(me) { return 18; })
	}
	if(context->isPython()) {
		QDO(def funcOverrideA(me): return 38)
		QDO(def funcOverrideB(me): return 18)
	}

	QNEWOBJ(a, ScriptOverride())
	QASSERT(a.getValue() == 1);
	QDO(a.getValue = funcOverrideA)

	QNEWOBJ(b, ScriptOverride())
	QASSERT(b.getValue() == 1);
	QDO(b.getValue = funcOverrideB)

	QASSERT(a.getValue() == 38);
	QASSERT(b.getValue() == 18);

	ScriptOverrideBase * objA = static_cast<ScriptOverrideBase *>(static_cast<ScriptOverride *>(scriptGetValue(binding, "a").toObjectAddress(NULL, NULL)));
	GEQUAL(38, objA->getValue());

	ScriptOverride * objB = static_cast<ScriptOverride *>(scriptGetValue(binding, "b").toObjectAddress(NULL, NULL));
	GEQUAL(18, objB->getValue());
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
