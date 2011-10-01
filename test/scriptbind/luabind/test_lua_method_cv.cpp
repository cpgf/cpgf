#include "test_lua_common.h"


namespace {


GTEST(Method_CV)
{
	TestLuaContext * context = getLuaContext();

	context->doString(""
		LINE(a = TestObject())
		LINE(assert(a.methodConst() == 0))
		
		LINE(a = a.selfConst())
		LINE(assert(a.methodConst() == 1))
		
		LINE(a = TestObject())
		LINE(assert(a.methodVolatile() == 0))
		
		LINE(a = a.selfVolatile())
		LINE(assert(a.methodVolatile() == 1))
		
		LINE(a = TestObject())
		LINE(assert(a.methodConstVolatile() == 0))
		
		LINE(a = a.selfConstVolatile())
		LINE(assert(a.methodConstVolatile() == 1))
		
		LINE(a = TestObject())
		LINE(a = a.self())
		LINE(assert(a.methodConst() == 0))
		LINE(assert(a.methodVolatile() == 0))
		LINE(assert(a.methodConstVolatile() == 0))
		
	);
}



}
