#include "testmetagen.h"
#include "../include/simpleoverridefromscript.h"
#include "../metadata/include/meta_test_simpleoverridefromscript.h"

#include <iostream>
using namespace std;

using namespace meta_test;

namespace {


template <typename T>
void doTestSimpleOverrideFromScript_OverrideFromScriptClass(T * binding, TestScriptContext * context)
{
	if(context->isLua()) {
		QDO(function funcOverride(me) return me.n + 15 end)
	}
	if(context->isV8()) {
		QDO(function funcOverride(me) { return me.n + 15; })
	}
	if(context->isPython()) {
		QDO(def funcOverride(me): return me.n + 15)
	}

	QNEWOBJ(a, mtest.SimpleOverrideWrapper(3))
	QASSERT(a.getValue() == 3);
	QDO(mtest.SimpleOverrideWrapper.getValue = funcOverride)
//	QASSERT(a.getValue() == 18);

	SimpleOverrideWrapper * objA = static_cast<SimpleOverrideWrapper *>(binding->getObject("a"));
	cout << objA->getValue() << endl;
//	GEQUAL(18, objA->getValue());
}

void testSimpleOverrideFromScript_OverrideFromScriptClass(TestScriptContext * context)
{
	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();

	if(bindingLib) {
		doTestSimpleOverrideFromScript_OverrideFromScriptClass(bindingLib, context);
	}
	
	if(bindingApi) {
		doTestSimpleOverrideFromScript_OverrideFromScriptClass(bindingApi, context);
	}
}

#define CASE testSimpleOverrideFromScript_OverrideFromScriptClass
#include "do_testcase.h"



}
