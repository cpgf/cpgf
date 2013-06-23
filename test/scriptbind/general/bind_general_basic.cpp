#include "../testscriptbind.h"

namespace {


template <typename T>
void doTestValueAssign(T * binding, TestScriptContext * context)
{
	(void)context;

	QVAR(a = "what")
	QVAR(b = 1)
	QASSERT(a == "what")
	QASSERT(b == 1)

	binding->assignValue("a", "b");
	binding->assignValue("b", "c");

	QASSERT(b == "what")
	QASSERT(c == "what")
}

void testValueAssign(TestScriptContext * context)
{
	QVAR(a = "what")
	QVAR(b = 1)
	QASSERT(a == "what")
	QASSERT(b == 1)

	if(context->getBindingLib()) {
		doTestValueAssign(context->getBindingLib(), context);
	}
	
	if(context->getBindingApi()) {
		doTestValueAssign(context->getBindingApi(), context);
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
	
	if(context->isPython()) {
		QDO(imnull = None)
	}
	
	if(context->getBindingLib()) {
		GCHECK(scriptGetValue(context->getBindingLib(), "imnull").isNull());
	}
	
	if(context->getBindingApi()) {
		GCHECK(scriptGetValue(context->getBindingApi(), "imnull").isNull());
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
	
	QASSERT(BasicA.a == 1)
	QASSERT(BasicA.b == 2)
	QASSERT(BasicA.c == 3)

	QASSERT(a.a == 1)
	QASSERT(a.b == 2)
	QASSERT(a.c == 3)
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


void testReturnVariant(TestScriptContext * context)
{
	QNEWOBJ(a, TestObject())
	QASSERT(a.getVariant(1) == "abc")
	QASSERT(a.getVariant(2) == 1.5)
	QASSERT(a.getVariant(0) == 38)
}

#define CASE testReturnVariant
#include "../bind_testcase.h"


void testDefaultParameters(TestScriptContext * context)
{
	QDO(classTestObject = TestObject)
	QNEWOBJ(a, classTestObject(38))
	QASSERT(testDefaultParam(0));
	QASSERT(testDefaultParam(0, 5));
	QASSERT(testDefaultParam(1, 98));
	QASSERT(testDefaultParam(2, 98, "def"));
	QASSERT(testDefaultParam(3, 98, "def", a));
	QERR(testDefaultParam(0, 5, "abc", a, 0));
}

#define CASE testDefaultParameters
#include "../bind_testcase.h"


void testWideStringParameter(TestScriptContext * context)
{
	QASSERT(testWideStringParam("WideTest") == "TRUE");
}

#define CASE testWideStringParameter
#include "../bind_testcase.h"



}
