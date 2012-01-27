#include "../testscriptbind.h"


namespace {


void testHierarchyMethod(TestScriptContext * context)
{
	QNEWOBJ(e, DeriveE())
	DO("scriptAssert(e.getA() == \"DeriveE\")")

	QASSERT(e.getB() == Magic1 + 1)
	QASSERT(e.getC() == Magic2 + 2)
	QASSERT(e.getD() == Magic3 + 3)
	QASSERT(e.getE() == Magic1 + Magic2 + Magic3)

	QDO(a = DeriveE.pretendA())
	DO("scriptAssert(a.getA() == \"DeriveE\")")
	QERR(a.getB())
	QERR(a.getC())

	QNEWOBJ(d, DeriveD())
	DO("scriptAssert(d.getA() == \"DeriveA\")")
	QASSERT(d.getB() == Magic1)
	QASSERT(d.getC() == Magic2)
	QASSERT(d.getD() == Magic3)
}

#define CASE testHierarchyMethod
#include "../bind_testcase.h"


void testHierarchyData(TestScriptContext * context)
{
	QNEWOBJ(e, DeriveE())
	QDO(e.setData(8))

	QASSERT(e.e == 8 * 5)
	QASSERT(e.d == 8 * 4)
	QASSERT(e.c == 8 * 3)
	QASSERT(e.b == 8 * 2)
	QASSERT(e.a == 8 * 1)
}

#define CASE testHierarchyData
#include "../bind_testcase.h"



}
