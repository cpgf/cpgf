#define MK2(a, b) a ## b
#define MK(a, b) MK2(a, b)


#if ENABLE_LUA


GTEST(MK(CASE, _Lua_Lib))
{
	TestScriptContext * context = createTestScriptContext(tslLua, tsaLib);
	
	bindCommonData(context);
	CASE(context->getBindingLib(), context);
}

GTEST(MK(CASE, _Lua_Api))
{
	TestScriptContext * context = createTestScriptContext(tslLua, tsaApi);

	bindCommonData(context);
	CASE(context->getBindingApi(), context);
}


#endif


#if ENABLE_V8

GTEST(MK(CASE, _V8_Lib))
{
	TestScriptContext * context = createTestScriptContext(tslV8, tsaLib);

	bindCommonData(context);
	CASE(context->getBindingLib(), context);
}


GTEST(MK(CASE, _V8_Api))
{
	TestScriptContext * context = createTestScriptContext(tslV8, tsaApi);
	
	bindCommonData(context);
	CASE(context->getBindingApi(), context);
}


#endif


#undef CASE
#undef MK2
#undef MK

