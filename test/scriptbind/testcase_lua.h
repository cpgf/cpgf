#define MK2(a, b) a ## b
#define MK(a, b) MK2(a, b)


#if ENABLE_LUA


GTEST(MK(CASE, _Lua_Lib))
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslLua, tsaLib));

	CASE(context.get());
}

GTEST(MK(CASE, _Lua_Api))
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslLua, tsaApi));

	CASE(context.get());
}


#endif


#undef CASE
#undef MK2
#undef MK
