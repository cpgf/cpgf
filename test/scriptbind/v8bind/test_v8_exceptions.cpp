#include "../testscriptbind.h"

namespace {

template <typename T>
void doTestJsToCppExceptions(T * binding, TestScriptContext * context)
{
	QDO(function funcError() { throw new Error("test error") } )
	try {
		invokeScriptFunction(binding, "funcError");
	} catch (std::runtime_error &e) {
		GCHECK(0 == std::string(e.what()).compare("Error: test error"));
		return;
	}
	GFAIL("exception was not thrown");
}

void testJsToCppExceptions(TestScriptContext * context)
{
	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();

	if(bindingLib) {
		doTestJsToCppExceptions(bindingLib, context);
	}

	if(bindingApi) {
		doTestJsToCppExceptions(bindingApi, context);
	}
}

#define CASE testJsToCppExceptions
#include "../testcase_v8.h"

void testCppPassthroughExceptions(TestScriptContext * context)
{
	QDO(function funcError() { throw new Error("test error") } )
	QDO(
		try {
			testAddCallback(funcError);
			throw new Error("exception was not thrown");
		} catch (e) {
			if (e.message != "test error") {
				throw e
			}
		}
	)
}

#define CASE testCppPassthroughExceptions
#include "../testcase_v8.h"

}
