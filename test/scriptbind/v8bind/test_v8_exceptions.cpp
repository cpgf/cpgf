#include "../testscriptbind.h"

namespace {

template <typename T>
void doTestExceptions(T * binding, TestScriptContext * context)
{
	QDO(function funcError() { throw new Error("test error") } )
	try {
		invokeScriptFunction(binding, "funcError");
	} catch (std::runtime_error &e) {
		GCHECK(0 == std::string(e.what()).compare("Error: test error"));
		return;
	}
	GCHECK(false);
}

void testExceptions(TestScriptContext * context)
{
	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();

	if(bindingLib) {
		doTestExceptions(bindingLib, context);
	}

	if(bindingApi) {
		doTestExceptions(bindingApi, context);
	}
}

#define CASE testExceptions
#include "../testcase_v8.h"


}
