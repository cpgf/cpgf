#include "testmetagen.h"
#include "../include/simpleoverridefromscript.h"
#include "../metadata/include/meta_test_simpleoverridefromscript.h"


using namespace meta_test;
using namespace std;

namespace {


template <typename T>
void doTestSimpleOverrideFromScript_OverrideFromScriptClass(T * binding, TestScriptContext * context)
{
if(context->getBindingApi()) return;
	if(context->isLua()) {
		QDO(function overrideGetValue(me) return me.getValue() + 15 end)
		QDO(function overrideGetName(me) return "abc" end)
	}
	if(context->isV8()) {
		QDO(function overrideGetValue(me) { return me.getValue() + 15; })
		QDO(function overrideGetName(me) { return "abc"; })
	}
	if(context->isPython()) {
		QDO(def overrideGetValue(me): return me.getValue() + 15)
		QDO(def overrideGetName(me): return "abc")
	}

	QDO(DerivedClass = cpgf.cloneClass(mtest.SimpleOverrideWrapper))

	QNEWOBJ(a, DerivedClass(3))
	QASSERT(a.getValue() == 3);
	QDO(DerivedClass.getValue = overrideGetValue)
	QASSERT(a.getValue() == 18);

	QNEWOBJ(b, DerivedClass(5))
	QASSERT(b.getValue() == 20);
	QNEWOBJ(c, mtest.SimpleOverrideWrapper(3))
	QASSERT(c.getValue() == 3);

	SimpleOverrideWrapper * objA = static_cast<SimpleOverrideWrapper *>(binding->getObject("a"));
	GEQUAL(18, objA->getValue());
	
	QDO(DerivedClass.getName = overrideGetName);
	QASSERT(a.getName() == "abc");
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


template <typename T>
void doTestSimpleOverrideFromScript_OverrideFromScriptObject(T * binding, TestScriptContext * context)
{
if(context->getBindingApi()) {
	return;
}
	if(context->isLua()) {
		QDO(function overrideGetValue(me) return me.getValue() + 5 end)
		QDO(function overrideGetName(me) return "abc" end)
		QDO(function overrideGetAnother(me) return 2 end)
	}
	if(context->isV8()) {
		QDO(function overrideGetValue(me) { return 2 + 5; })
		QDO(function overrideGetName(me) { return "abc"; })
		QDO(function overrideGetAnother(me) { return 2; })
	}
	if(context->isPython()) {
		QDO(def overrideGetValue(me): return me.getValue() + 5)
		QDO(def overrideGetName(me): return "abc")
		QDO(def overrideGetAnother(me): return 2)
	}

	QNEWOBJ(a, mtest.SimpleOverrideWrapper(2))
	
	QASSERT(a.getValue() == 2);
	QDO(a.getValue = overrideGetValue);
	QASSERT(a.getValue() == 7);

	QNEWOBJ(b, mtest.SimpleOverrideWrapper(6))
	QASSERT(b.getValue() == 6);

	SimpleOverrideWrapper * objA = static_cast<SimpleOverrideWrapper *>(binding->getObject("a"));
	GEQUAL(7, objA->getValue());

	QDO(a.getAnother = overrideGetAnother);
	QASSERT(a.getAnother() == 2);

	QDO(a.getName = overrideGetName);
	QASSERT(a.getName() == "abc");

	GEQUAL(string("abc"), objA->getName());
}

void testSimpleOverrideFromScript_OverrideFromScriptObject(TestScriptContext * context)
{
	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();

	if(bindingLib) {
		doTestSimpleOverrideFromScript_OverrideFromScriptObject(bindingLib, context);
	}
	
	if(bindingApi) {
		doTestSimpleOverrideFromScript_OverrideFromScriptObject(bindingApi, context);
	}
}

#define CASE testSimpleOverrideFromScript_OverrideFromScriptObject
#include "do_testcase.h"



}
