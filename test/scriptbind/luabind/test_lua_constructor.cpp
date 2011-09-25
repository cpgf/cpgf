#include "test_lua_common.h"


namespace {


GTEST(Constructor)
{
	TestLuaContext * context = getLuaContext();

	context->doString(
		LINE(a = TestObject())
		LINE(assert(a.value == Magic1))

		LINE(a.value = 1999)
		LINE(b = TestObject(a))
		LINE(assert(b.value == 1999))

		LINE(a = TestObject(180))
		LINE(assert(a.value == 180))

		LINE(a = TestObject(5, "abc"))
		LINE(assert(a.value == 8))

	);
}




}
