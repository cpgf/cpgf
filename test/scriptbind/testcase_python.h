#define MK2(a, b) a ## b
#define MK(a, b) MK2(a, b)


#ifndef TEST_BIND
#define TEST_BIND testscript::bindBasicData
#endif

#if ENABLE_PYTHON


GTEST(MK(CASE, _Python_Lib))
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslPython, tsaLib));

	TEST_BIND(context->getBindingLib(), context->getService());

	CASE(context.get());
}

GTEST(MK(CASE, _Python_Api))
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslPython, tsaApi));

	TEST_BIND(context->getBindingApi(), context->getService());

	CASE(context.get());
}


#endif


#undef CASE
#undef MK2
#undef MK
