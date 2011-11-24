#include "test_lua_common.h"


namespace {


GTEST(Method_CV)
{
	GScopedPointer<TestLuaContext> context(createLuaContext());

	QDO(a = TestObject())
	QASSERT(a.methodConst() == 0)
			
	QDO(a = a.selfConst())
	QASSERT(a.methodConst() == 1)
			
	QDO(a = TestObject())
	QASSERT(a.methodVolatile() == 0)
			
	QDO(a = a.selfVolatile())
	QASSERT(a.methodVolatile() == 1)
			
	QDO(a = TestObject())
	QASSERT(a.methodConstVolatile() == 0)
			
	QDO(a = a.selfConstVolatile())
	QASSERT(a.methodConstVolatile() == 1)
			
	QDO(a = TestObject())
	QDO(a = a.self())
	QASSERT(a.methodConst() == 0)
	QASSERT(a.methodVolatile() == 0)
	QASSERT(a.methodConstVolatile() == 0)

	QDO(a = TestObject())
	QASSERT(a.methodConst() == 0)
			
	QDO(a = a.selfConst())
	QERR(a.value = 1) // a is const object
}



}
