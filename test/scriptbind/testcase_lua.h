#define MK2(a, b) a ## b
#define MK(a, b) MK2(a, b)

#ifndef TEST_BIND
#define TEST_BIND testscript::bindBasicData
#endif


#if ENABLE_LUA


GTEST(MK(CASE, _Lua_Lib))
{
	std::unique_ptr<TestScriptContext> context(createTestScriptContext(tslLua, tsaLib));

	TEST_BIND(context->getBindingLib(), context->getService());

	CASE(context.get());
}

GTEST(MK(CASE, _Lua_Api))
{
	std::unique_ptr<TestScriptContext> context(createTestScriptContext(tslLua, tsaApi));

	TEST_BIND(context->getBindingApi(), context->getService());

	CASE(context.get());
}


#endif


#undef CASE
#undef MK2
#undef MK
