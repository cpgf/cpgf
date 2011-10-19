#include "test_lua_common.h"


namespace {


GTEST(Method_Overload)
{
	TestLuaContext * context = getLuaContext();

	context->doString(""
		LINE(a = TestObject())
		LINE(assert(a.methodOverload(2, 3) == 2 * 3))
		
		LINE(a = TestObject())
		LINE(assert(a.methodOverload(TestObject(2), 3) == 2 + 3))
		
		LINE(a = TestObject())
		LINE(assert(a.methodOverload(3, TestObject(2)) == 2 + 3 + 1))
		
		LINE(a = TestObject())
		LINE(assert(a.methodOverload("ab", 3) == 2 + 3))
		
		LINE(a = TestObject())
		LINE(assert(a.methodOverload(3, "ab") == 2 + 3 + 1))
		
	);
}


GTEST(Method_OverloadObject)
{
	TestLuaContext * context = getLuaContext();

	context->doString(""
		LINE(a = TestObject())
		LINE(assert(a.methodOverloadObject(TestBase()) == Magic1))
		
		LINE(a = TestObject())
		LINE(assert(a.methodOverloadObject(TestA()) == Magic2))
		
		LINE(a = TestObject())
		LINE(assert(a.methodOverloadObject(TestB()) == Magic3))
		
		LINE(a = TestObject())
		LINE(assert(a.methodOverloadObject(TestC()) == Magic1 + Magic2))
		
	);
}



}
