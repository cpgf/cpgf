#include "../testscriptbind.h"

namespace {

void testConstructor(TestScriptContext * context)
{
	QNEWOBJ(a, TestObject())
	QASSERT(a.value == Magic1)

	QDO(a.value = 1999)
	QNEWOBJ(b, TestObject(a))
	QASSERT(b.value == 1999)

	QNEWOBJ(a, TestObject(180))
	QASSERT(a.value == 180)

	QNEWOBJ(a, TestObject(5, "abc"))
	QASSERT(a.value == 8)
}

#define CASE testConstructor
#include "../bind_testcase.h"



}
