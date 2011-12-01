#include "bind_general_common.h"

namespace {

void testConstructor(TestScriptContext * context)
{
	NEWOBJ("a", "TestObject()")
	QASSERT(a.value == Magic1)

	QDO(a.value = 1999)
	NEWOBJ("b", "TestObject(a)")
	QASSERT(b.value == 1999)

	NEWOBJ("a", "TestObject(180)")
	QASSERT(a.value == 180)

	NEWOBJ("a", "TestObject(5, \"abc\")")
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




}
