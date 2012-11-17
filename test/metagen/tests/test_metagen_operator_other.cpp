#include "testmetagen.h"


namespace {


void metagenTest_Operators_Comma(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorOther(10));
	QDO(b = a._opComma(5));
	QASSERT(b.value == 15);
}

#define CASE metagenTest_Operators_Comma
#include "do_testcase.h"


void metagenTest_Operators_Subscript(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorOther(3));
	QDO(b = a._opSubscript(5));
	QASSERT(b == 8);
	
	QDO(b = a._opSubscript("abcd"));
	QASSERT(b == 7);
}

#define CASE metagenTest_Operators_Subscript
#include "do_testcase.h"


void metagenTest_Operators_Address(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorOther(8));
	QDO(b = a._opAddress());
	QASSERT(b == 9);
}

#define CASE metagenTest_Operators_Address
#include "do_testcase.h"



void metagenTest_Operators_Derefer(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorOther(8));
	QDO(b = a._opDerefer());
	QASSERT(b == 7);
}

#define CASE metagenTest_Operators_Derefer
#include "do_testcase.h"


void metagenTest_Operators_Function(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorOther(8));
	QDO(b = a._opFunction("abcd", 5));
	QASSERT(b == 17);
	
	QDO(b = a._opFunction(1, 2, 3, 4, 5));
	QASSERT(b == 23);
}

#define CASE metagenTest_Operators_Function
#include "do_testcase.h"



}
