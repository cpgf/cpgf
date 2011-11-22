#include "test_lua_common.h"


namespace {


GTEST(Method_CV)
{
	TestLuaContext * context = getLuaContext();

	GCHECK(
		context->doString(""
			LINE(a = TestObject())
			LINE(scriptAssert(a.methodConst() == 0))
			
			LINE(a = a.selfConst())
			LINE(scriptAssert(a.methodConst() == 1))
			
			LINE(a = TestObject())
			LINE(scriptAssert(a.methodVolatile() == 0))
			
			LINE(a = a.selfVolatile())
			LINE(scriptAssert(a.methodVolatile() == 1))
			
			LINE(a = TestObject())
			LINE(scriptAssert(a.methodConstVolatile() == 0))
			
			LINE(a = a.selfConstVolatile())
			LINE(scriptAssert(a.methodConstVolatile() == 1))
			
			LINE(a = TestObject())
			LINE(a = a.self())
			LINE(scriptAssert(a.methodConst() == 0))
			LINE(scriptAssert(a.methodVolatile() == 0))
			LINE(scriptAssert(a.methodConstVolatile() == 0))
			
		)
	);

	GCHECK(
		context->doString(""
			LINE(a = TestObject())
			LINE(scriptAssert(a.methodConst() == 0))
			
			LINE(a = a.selfConst())
		)
	);
	GCHECK(
		context->doError(""
			LINE(a.value = 1) // a is const object
		)
	);
}



}
