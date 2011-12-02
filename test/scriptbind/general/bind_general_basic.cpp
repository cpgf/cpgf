#include "bind_general_common.h"

namespace {

void testEnum(TestScriptContext * context)
{
	QASSERT(TestEnum.teCpp == 1)
	QASSERT(TestEnum.teLua == 2)
	QASSERT(TestEnum.teV8 == 3)

	QERR(TestEnum.teCpp = 5)

	QNEWOBJ(a, BasicA())
	QASSERT(a.BasicEnum.a == 1)
	QASSERT(a.BasicEnum.b == 2)
	QASSERT(a.BasicEnum.c == 3)

	QERR(a.BasicEnum.a = 1)

	QASSERT(BasicA.BasicEnum.a == 1)
}


GTEST(TestEnum_Lua_Lib)
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslLua, tsaLib));

	testEnum(context.get());
}

GTEST(TestEnum_Lua_Api)
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslLua, tsaApi));

	testEnum(context.get());
}

GTEST(TestEnum_V8_Lib)
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslV8, tsaLib));

	testEnum(context.get());
}

GTEST(TestEnum_V8_Api)
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslV8, tsaApi));

	testEnum(context.get());
}


void testInnerClass(TestScriptContext * context)
{
	QNEWOBJ(b, BasicA())
	QNEWOBJ(a, b.Inner())
	QASSERT(a.x == 5)
	QASSERT(a.add() == 6)
	QASSERT(a.x == 6)
}

GTEST(TestInnerClass_Lua_Lib)
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslLua, tsaLib));

	testInnerClass(context.get());
}

GTEST(TestInnerClass_Lua_Api)
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslLua, tsaApi));

	testInnerClass(context.get());
}

GTEST(TestInnerClass_V8_Lib)
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslV8, tsaLib));

	testInnerClass(context.get());
}

GTEST(TestInnerClass_V8_Api)
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslV8, tsaApi));

	testInnerClass(context.get());
}




}
