#include "../bind_common.h"

namespace {


void testValueAssign(TestScriptContext * context)
{
	QDO(a = "what")
	QDO(b = 1)
	QASSERT(a == "what")
	QASSERT(b == 1)

	if(context->getBindingLib()) {
		context->getBindingLib()->assignValue("a", "b");
		context->getBindingLib()->assignValue("b", "c");
	}
	
	if(context->getBindingApi()) {
		context->getBindingApi()->assignValue("a", "b");
		context->getBindingApi()->assignValue("b", "c");
	}
	QASSERT(b == "what")
	QASSERT(c == "what")
}

#define CASE testValueAssign
#include "../bind_testcase.h"


void testValueIsNull(TestScriptContext * context)
{
	if(context->isLua()) {
		QDO(imnull = nil)
	}

	if(context->isV8()) {
		QDO(imnull = null)
	}
	
	if(context->getBindingLib()) {
		GCHECK(context->getBindingLib()->valueIsNull("imnull"));
	}
	
	if(context->getBindingApi()) {
		GCHECK(context->getBindingApi()->valueIsNull("imnull"));
	}
}

#define CASE testValueIsNull
#include "../bind_testcase.h"


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


void testMisc(TestScriptContext * context)
{
	QASSERT(testString == "TestingScript!")
	QASSERT(testInt == 1978)
	QASSERT(testObj.value == 2012)
}

#define CASE testMisc
#include "../bind_testcase.h"



}
