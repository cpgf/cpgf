#include "bind_general_common.h"

namespace {

void testConstructor(TestScriptContext * context)
{
	QNEWOBJ(a, TestObject())
	QASSERT(a.value == Magic1)

	QDO(a.value = 1999)
	QNEWOBJ(b, TestObject(a))
	QASSERT(b.value == 1999)

	QNEWOBJ(a, TestObject(180))
	QASSERT(a.value == 180)

	QNEWOBJ(a, TestObject(5, "abc"))
	QASSERT(a.value == 8)
}

GTEST(Constructor_Lua_Lib)
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslLua, tsaLib));

	testConstructor(context.get());
}

GTEST(Constructor_Lua_Api)
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslLua, tsaApi));

	testConstructor(context.get());
}

GTEST(Constructor_V8_Lib)
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslV8, tsaLib));

	testConstructor(context.get());
}

GTEST(Constructor_V8_Api)
{
	GScopedPointer<TestScriptContext> context(createTestScriptContext(tslV8, tsaApi));

	testConstructor(context.get());
}




}
