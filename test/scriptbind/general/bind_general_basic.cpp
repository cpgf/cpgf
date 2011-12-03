#include "../bind_common.h"

namespace {

void testEnum(TestScriptContext * context)
{
	QASSERT(TestEnum.teCpp == 1)
	QASSERT(TestEnum.teLua == 2)
	QASSERT(TestEnum.teV8 == 3)

	QERR(TestEnum.teCpp = 5)

	QNEWOBJ(a, BasicA())
	QASSERT(a.BasicEnum.a == 1)
	QASSERT(a.BasicEnum.b == 2)
	QASSERT(a.BasicEnum.c == 3)

	QERR(a.BasicEnum.a = 1)

	QASSERT(BasicA.BasicEnum.a == 1)
	QASSERT(BasicA.BasicEnum.b == 2)
	QASSERT(BasicA.BasicEnum.c == 3)
}


#define CASE testEnum
#include "../bind_testcase.h"


void testInnerClass(TestScriptContext * context)
{
	QNEWOBJ(b, BasicA())
	QNEWOBJ(a, b.Inner())
	QASSERT(a.x == 5)
	QASSERT(a.add() == 6)
	QASSERT(a.x == 6)
}

#define CASE testInnerClass
#include "../bind_testcase.h"



}
