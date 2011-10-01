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



}
