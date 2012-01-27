#include "../testscriptbind.h"


namespace {


void Method_OverloadGlobal(TestScriptContext * context)
{
	QASSERT(testAdd(25) == 38 + 25)
	QASSERT(testAdd(7, 8) == 2 + 7 + 8)
	QASSERT(testAdd(9, 7, 8) == 3 + 9 + 7 + 8)
	QASSERT(testAdd(9, 7, 8, 38, 68) == 3 + 9 + 7 + 8 + 38 + 68)
}


#define CASE Method_OverloadGlobal
#include "../bind_testcase.h"



void Method_Overload(TestScriptContext * context)
{
	QNEWOBJ(a, TestObject())
	QASSERT(a.methodOverload(2, 3) == 2 * 3)
			
	QNEWOBJ(a, TestObject())
	QNEWOBJ(b, TestObject(2))
	QASSERT(a.methodOverload(b, 3) == 2 + 3)
			
	QNEWOBJ(a, TestObject())
	QNEWOBJ(b, TestObject(2))
	QASSERT(a.methodOverload(3, b) == 2 + 3 + 1)
			
	QNEWOBJ(a, TestObject())
	QASSERT(a.methodOverload("ab", 3) == 2 + 3)
			
	QNEWOBJ(a, TestObject())
	QASSERT(a.methodOverload(3, "ab") == 2 + 3 + 1)
}


#define CASE Method_Overload
#include "../bind_testcase.h"



void Method_OverloadObject(TestScriptContext * context)
{
	QNEWOBJ(a, TestObject())
	QNEWOBJ(b, TestBase())
	QASSERT(a.methodOverloadObject(b) == Magic1)
			
	QNEWOBJ(a, TestObject())
	QNEWOBJ(b, TestA())
	QASSERT(a.methodOverloadObject(b) == Magic2)
			
	QNEWOBJ(a, TestObject())
	QNEWOBJ(b, TestB())
	QASSERT(a.methodOverloadObject(b) == Magic3)
			
	QNEWOBJ(a, TestObject())
	QNEWOBJ(b, TestC())
	QASSERT(a.methodOverloadObject(b) == Magic1 + Magic2)
}

#define CASE Method_OverloadObject
#include "../bind_testcase.h"



}
