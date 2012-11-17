#include "testmetagen.h"


namespace {


void metagenTest_Operators_Plus(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorUnary(10));
	QDO(b = a._opPlus());
	QASSERT(b.value == 10);

	QNEWOBJ(a, mtest.MetagenOperatorUnary(-5));
	QDO(b = a._opPlus());
	QASSERT(b.value == 5);
}

#define CASE metagenTest_Operators_Plus
#include "do_testcase.h"


void metagenTest_Operators_Minus(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorUnary(10));
	QDO(b = a._opMinus());
	QASSERT(b.value == -10);

	QNEWOBJ(a, mtest.MetagenOperatorUnary(-5));
	QDO(b = a._opMinus());
	QASSERT(b.value == 5);
}

#define CASE metagenTest_Operators_Minus
#include "do_testcase.h"


void metagenTest_Operators_Inc(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorUnary(10));
	QDO(b = a._opInc());
	QASSERT(a.value == 11);
	QASSERT(b.value == 11);
}

#define CASE metagenTest_Operators_Inc
#include "do_testcase.h"



void metagenTest_Operators_IncSuffix(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorUnary(10));
	QDO(b = a._opIncSuffix());
	QASSERT(a.value == 11);
	QASSERT(b.value == 10);
}

#define CASE metagenTest_Operators_IncSuffix
#include "do_testcase.h"


void metagenTest_Operators_Dec(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorUnary(10));
	QDO(b = a._opDec());
	QASSERT(a.value == 9);
	QASSERT(b.value == 9);
}

#define CASE metagenTest_Operators_Dec
#include "do_testcase.h"



void metagenTest_Operators_DecSuffix(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorUnary(10));
	QDO(b = a._opDecSuffix());
	QASSERT(a.value == 9);
	QASSERT(b.value == 10);
}

#define CASE metagenTest_Operators_DecSuffix
#include "do_testcase.h"




}
