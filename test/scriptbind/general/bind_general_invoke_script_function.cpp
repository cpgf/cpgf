#include "../bind_common.h"

#include "cpgf/scriptbind/gscriptbindutil.h"


namespace {


template <typename T>
void doTestInvokeScriptFunction(T * binding, TestScriptContext * context)
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

	GMetaVariant result;
	
	TestData data;
	data.x = 5;
	data.name = "abc";
	
	result = invokeScriptFunction(binding, "funcAdd", 8, 2);
	GEQUAL(fromVariant<int>(result.getValue()), 10);
		
	result = invokeScriptFunction(binding, "funcLen", "abc", 2);
	GEQUAL(fromVariant<int>(result.getValue()), 5);

	result = invokeScriptFunction(binding, "funcTestData", &data);
	GEQUAL(fromVariant<int>(result.getValue()), 8);

	result = 0;
	GDIFF(fromVariant<int>(result.getValue()), 8);

	result = invokeScriptFunction(binding, "funcTestData", &data);
	GEQUAL(fromVariant<int>(result.getValue()), 8);

	TestData * pdata;
	result = invokeScriptFunction(binding, "funcNewTestData");
	pdata = fromVariant<TestData *>(result.getValue());
	GEQUAL(pdata->x, 3);
	GEQUAL(pdata->name, "def");
}

void testInvokeScriptFunction(TestScriptContext * context)
{
	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();

	if(bindingLib) {
		doTestInvokeScriptFunction(bindingLib, context);
	}
	
	if(bindingApi) {
		doTestInvokeScriptFunction(bindingApi, context);
	}
}

#define CASE testInvokeScriptFunction
#include "../bind_testcase.h"


template <typename T>
void doTestInvokeCppFunction(T * binding, TestScriptContext * context)
{
	(void)context;

	GMetaVariant result;
	
	result = invokeScriptFunction(binding, "testAdd", 25);
	GEQUAL(fromVariant<int>(result.getValue()), 38 + 25);
		
	result = invokeScriptFunction(binding, "testAdd", 7, 8);
	GEQUAL(fromVariant<int>(result.getValue()), 2 + 7 + 8);

	result = invokeScriptFunction(binding, "testAdd", 9, 7, 8);
	GEQUAL(fromVariant<int>(result.getValue()), 3 + 9 + 7 + 8);

	result = invokeScriptFunction(binding, "testAdd", 9, 7, 8, 38, 68);
	GEQUAL(fromVariant<int>(result.getValue()), 3 + 9 + 7 + 8 + 38 + 68);
}

void testInvokeCppFunction(TestScriptContext * context)
{
	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();

	if(bindingLib) {
		doTestInvokeCppFunction(bindingLib, context);
	}
	
	if(bindingApi) {
		doTestInvokeCppFunction(bindingApi, context);
	}
}


#define CASE testInvokeCppFunction
#include "../bind_testcase.h"




}
