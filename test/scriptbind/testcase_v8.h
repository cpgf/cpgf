#define MK2(a, b) a ## b
#define MK(a, b) MK2(a, b)


#if ENABLE_V8

GTEST(MK(CASE, _V8_Lib))
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslV8, tsaLib));

	testscript::bindBasicData(context->getBindingLib(), context->getService());

	CASE(context.get());
}


GTEST(MK(CASE, _V8_Api))
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslV8, tsaApi));

	testscript::bindBasicData(context->getBindingApi(), context->getService());

	CASE(context.get());
}


#endif


#undef CASE
#undef MK2
#undef MK
