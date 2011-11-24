#include "test_lua_common.h"


namespace {


GTEST(Method_Overload)
{
	GScopedPointer<TestLuaContext> context(createLuaContext());

	QDO(a = TestObject())
	QASSERT(a.methodOverload(2, 3) == 2 * 3)
			
	QDO(a = TestObject())
	QASSERT(a.methodOverload(TestObject(2), 3) == 2 + 3)
			
	QDO(a = TestObject())
	QASSERT(a.methodOverload(3, TestObject(2)) == 2 + 3 + 1)
			
	QDO(a = TestObject())
	QASSERT(a.methodOverload("ab", 3) == 2 + 3)
			
	QDO(a = TestObject())
	QASSERT(a.methodOverload(3, "ab") == 2 + 3 + 1)
}


GTEST(Method_OverloadObject)
{
	GScopedPointer<TestLuaContext> context(createLuaContext());

	QDO(a = TestObject())
	QASSERT(a.methodOverloadObject(TestBase()) == Magic1)
			
	QDO(a = TestObject())
	QASSERT(a.methodOverloadObject(TestA()) == Magic2)
			
	QDO(a = TestObject())
	QASSERT(a.methodOverloadObject(TestB()) == Magic3)
			
	QDO(a = TestObject())
	QASSERT(a.methodOverloadObject(TestC()) == Magic1 + Magic2)
}



}
