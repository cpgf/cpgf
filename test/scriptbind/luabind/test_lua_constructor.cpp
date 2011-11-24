#include "test_lua_common.h"


namespace {


GTEST(Constructor)
{
	GScopedPointer<TestLuaContext> context(createLuaContext());

	QDO(a = TestObject())
	QASSERT(a.value == Magic1)

	QDO(a.value = 1999)
	QDO(b = TestObject(a))
	QASSERT(b.value == 1999)

	QDO(a = TestObject(180))
	QASSERT(a.value == 180)

	QDO(a = TestObject(5, "abc"))
	QASSERT(a.value == 8)
}




}
