#include "../bind_common.h"

#include "cpgf/scriptbind/gscriptbindutil.h"


namespace {


void TestInvokeScriptFunction(TestScriptContext * context)
{
	if(context->isLua()) {
		QDO(function funcAdd(a, b) return a + b end)
		QDO(function funcLen(a, b) return string.len(a) + b end)
		QDO(function funcTestData(a) return a.x + string.len(a.name) end)
		QDO(function funcNewTestData() a = TestData() a.x = 3 a.name = "def" return a end)
	}
	if(context->isV8()) {
		QDO(function funcAdd(a, b) { return a + b; })
		QDO(function funcLen(a, b) { return a.length + b; })
		QDO(function funcTestData(a) { return a.x + a.name.length; })
		QDO(function funcNewTestData() { a = new TestData(); a.x = 3; a.name = "def"; return a; })
	}

	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();

	GMetaVariant result;
	
	TestData data;
	data.x = 5;
	data.name = "abc";
	
	if(bindingLib) {
		result = invokeScriptFunction(bindingLib, "funcAdd", 8, 2);
		GEQUAL(fromVariant<int>(result.getValue()), 10);
		
		result = invokeScriptFunction(bindingLib, "funcLen", "abc", 2);
		GEQUAL(fromVariant<int>(result.getValue()), 5);

		result = invokeScriptFunction(bindingLib, "funcTestData", &data);
		GEQUAL(fromVariant<int>(result.getValue()), 8);
	}
	
	if(bindingApi) {
		result = 0;
		GDIFF(fromVariant<int>(result.getValue()), 8);

		result = invokeScriptFunction(bindingApi, "funcTestData", &data);
		GEQUAL(fromVariant<int>(result.getValue()), 8);

		TestData * pdata;
		result = invokeScriptFunction(bindingApi, "funcNewTestData");
		pdata = fromVariant<TestData *>(result.getValue());
		GEQUAL(pdata->x, 3);
		GEQUAL(pdata->name, "def");
	}
}

#define CASE TestInvokeScriptFunction
#include "../bind_testcase.h"



}
