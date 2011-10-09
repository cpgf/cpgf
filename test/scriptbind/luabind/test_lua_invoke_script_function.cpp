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

	);

	GScriptObject * bindingLib = context->getBindingLib();
	
	GVariant result;
	
	result = invokeScriptFunction(bindingLib, "luaAdd", 8, 2);
	GEQUAL(fromVariant<int>(result), 10);
	
	result = invokeScriptFunction(bindingLib, "luaLen", "abc", 2);
	GEQUAL(fromVariant<int>(result), 5);

	TestData data;
	data.x = 5;
	data.name = "abc";
	
//	result = invokeScriptFunction(bindingLib, "luaTestData", &data);
//	GEQUAL(fromVariant<int>(result), 8);
}




}
