#include "test_lua_common.h"


namespace {


GTEST(Hierarchy)
{
	GScopedPointer<TestLuaContext> context(createLuaContext());

	QDO(a = DeriveE())
	DO("scriptAssert(a.getA() == \"DeriveE\")")
	QASSERT(a.getB() == Magic1 + 1)
	QASSERT(a.getC() == Magic2 + 2)
	QASSERT(a.getD() == Magic3 + 3)
	QASSERT(a.getE() == Magic1 + Magic2 + Magic3)

	QDO(a = DeriveE.pretendA())
	DO("scriptAssert(a.getA() == \"DeriveE\")")
	QERR(a.getB())
	QERR(a.getC())

	QDO(a = DeriveD())
	DO("scriptAssert(a.getA() == \"DeriveA\")")
	QASSERT(a.getB() == Magic1)
	QASSERT(a.getC() == Magic2)
	QASSERT(a.getD() == Magic3)
}




}
