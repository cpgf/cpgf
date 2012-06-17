#define MK2(a, b) a ## b
#define MK(a, b) MK2(a, b)


#if ENABLE_LUA


GTEST(MK(CASE, _Python_Lib))
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslPython, tsaLib));

	testscript::bindBasicData(context->getBindingLib(), context->getService());

	CASE(context.get());
}

GTEST(MK(CASE, _Python_Api))
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslPython, tsaApi));

	testscript::bindBasicData(context->getBindingApi(), context->getService());

	CASE(context.get());
}


#endif


#undef CASE
#undef MK2
#undef MK
