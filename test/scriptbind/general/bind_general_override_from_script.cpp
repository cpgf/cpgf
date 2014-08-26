#include "../testscriptbindmetadata5.h"
#include "../testscriptbind.h"

#include "cpgf/scriptbind/gscriptbindutil.h"


namespace {


template <typename T>
void doTestOverrideCppFunctionFromScriptClass(T * binding, TestScriptContext * context)
{
	if(context->isLua()) {
		QDO(function funcOverride(me) return me.n + 15 end)
		QDO(function funcOverrideSecond(me) return me.n + 16 end)
	}
	if(context->isV8() || context->isSpiderMonkey()) {
		QDO(function funcOverride(me) { return me.n + 15; })
		QDO(function funcOverrideSecond(me) { return me.n + 16; })
	}
	if(context->isPython()) {
		QDO(def funcOverride(me): return me.n + 15)
		QDO(def funcOverrideSecond(me): return me.n + 16)
	}

	QNEWOBJ(a, ScriptOverride(3))
	QASSERT(a.getValue() == 3);
	QDO(ScriptOverride.getValue = funcOverride)
	QASSERT(a.getValue() == 18);

	ScriptOverride * objA = static_cast<ScriptOverride *>(scriptGetValue(binding, "a").toObjectAddress(NULL, NULL));
	GEQUAL(18, objA->getValue());

  QDO(ScriptOverride.getValue = funcOverrideSecond)
  QASSERT(a.getValue() == 19);
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
void doTestOverrideCppFunctionOnNativePtrFromScriptClass(T * binding, TestScriptContext * context)
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

	QDO(ScriptOverride.getValue = funcOverride)

	IMetaClass * scriptClass = scriptGetValue(binding, "ScriptOverride").toClass();

	ScriptOverride obj(68);

	binding->getContext()->bindExternalObjectToClass(&obj, scriptClass);

	GEQUAL(83, obj.getValue());
	GEQUAL(83, obj.getValue()); // repeat the query to avoid caches
}

void testOverrideCppFunctionOnNativePtrFromScriptClass(TestScriptContext * context)
{
	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();

	if(bindingLib) {
		doTestOverrideCppFunctionOnNativePtrFromScriptClass(bindingLib, context);
	}

	if(bindingApi) {
		doTestOverrideCppFunctionOnNativePtrFromScriptClass(bindingApi, context);
	}
}

#define CASE testOverrideCppFunctionOnNativePtrFromScriptClass
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
