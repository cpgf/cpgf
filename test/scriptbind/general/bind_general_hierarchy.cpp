#include "bind_general_common.h"


namespace {


void testHierarchy(TestScriptContext * context)
{
	QNEWOBJ(a, DeriveE())
	DO("scriptAssert(a.getA() == \"DeriveE\")")
return;
	QASSERT(a.getB() == Magic1 + 1)
	QASSERT(a.getC() == Magic2 + 2)
	QASSERT(a.getD() == Magic3 + 3)
	QASSERT(a.getE() == Magic1 + Magic2 + Magic3)

	QDO(a = DeriveE.pretendA())
	DO("scriptAssert(a.getA() == \"DeriveE\")")
	QERR(a.getB())
	QERR(a.getC())

	QNEWOBJ(a, DeriveD())
	DO("scriptAssert(a.getA() == \"DeriveA\")")
	QASSERT(a.getB() == Magic1)
	QASSERT(a.getC() == Magic2)
	QASSERT(a.getD() == Magic3)
}

#define CASE testHierarchy
#include "bind_general_testcase.h"



}
