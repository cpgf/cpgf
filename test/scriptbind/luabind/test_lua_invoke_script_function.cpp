#include "test_lua_common.h"

#include "cpgf/scriptbind/gscriptbindutil.h"


namespace {


GTEST(InvokeScriptFunction)
{
	using namespace cpgf;

	TestLuaContext * context = getLuaContext();

	context->doString(
		LINE(function luaAdd(a, b) return a + b end)
		LINE(function luaLen(a, b) return string.len(a) + b end)
		LINE(function luaTestData(a) return a.x + string.len(a.name) end)
		LINE(function luaNewTestData() a = TestData() a.x = 3 a.name = "def" return a end)

	);

	GScriptObject * bindingLib = context->getBindingLib();
	GScopedInterface<IScriptObject> bindingApi(context->getBindingApi());
	
	GMetaVariant result;
	
	result = invokeScriptFunction(bindingLib, "luaAdd", 8, 2);
	GEQUAL(fromVariant<int>(result.getValue()), 10);
	
	result = invokeScriptFunction(bindingLib, "luaLen", "abc", 2);
	GEQUAL(fromVariant<int>(result.getValue()), 5);

	TestData data;
	data.x = 5;
	data.name = "abc";
	
	result = invokeScriptFunction(bindingLib, "luaTestData", &data);
	GEQUAL(fromVariant<int>(result.getValue()), 8);
	
	result = 0;
	GDIFF(fromVariant<int>(result.getValue()), 8);
	
	result = invokeScriptFunction(bindingApi.get(), "luaTestData", &data);
	GEQUAL(fromVariant<int>(result.getValue()), 8);

	TestData * pdata;
	result = invokeScriptFunction(bindingApi.get(), "luaNewTestData");
	pdata = fromVariant<TestData *>(result.getValue());
	GEQUAL(pdata->x, 3);
	GEQUAL(pdata->name, "def");
}




}
