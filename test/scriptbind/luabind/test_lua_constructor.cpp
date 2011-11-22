#include "test_lua_common.h"


namespace {


GTEST(Constructor)
{
	TestLuaContext * context = getLuaContext();

	GCHECK(
	   	context->doString(
   			LINE(a = TestObject())
  			LINE(scriptAssert(a.value == Magic1))

	  		LINE(a.value = 1999)
  			LINE(b = TestObject(a))
  			LINE(scriptAssert(b.value == 1999))

	  		LINE(a = TestObject(180))
  			LINE(scriptAssert(a.value == 180))

  			LINE(a = TestObject(5, "abc"))
	  		LINE(scriptAssert(a.value == 8))

  		)
	);
}




}
