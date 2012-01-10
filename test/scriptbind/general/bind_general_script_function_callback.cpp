#include "../bind_common.h"

#include "cpgf/scriptbind/gscriptbindutil.h"


namespace {

string makeFunc(const string & name, TestScriptContext * context)
{
	if(context->isLua()) {
		return "function " + name + "(a, b) return a + b end";
	}
	if(context->isV8()) {
		return "function " + name + "(a, b) { return a + b; }";
	}

	return "";
}

template <typename T>
void doTestScriptFunctionCallback(T * binding, TestScriptContext * context)
{
	(void)binding;

	DO(makeFunc("fcallback", context))
	QDO(a = testAddCallback(fcallback))
	QASSERT(a == 11)
	
	DO(string("b = testAddCallback(") + makeFunc("", context) + ")")
	QASSERT(b == 11)
}

void testScriptFunctionCallback(TestScriptContext * context)
{
	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();

	if(bindingLib) {
		doTestScriptFunctionCallback(bindingLib, context);
	}
	
	if(bindingApi) {
		doTestScriptFunctionCallback(bindingApi, context);
	}
}

#define CASE testScriptFunctionCallback
#include "../bind_testcase.h"




}
