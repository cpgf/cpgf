#include "testmetagen.h"


namespace {


void metagenTest_Operators_BitAndAssign(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorBitwiseAssign(255));

	QDO(a = a._opBitAndAssign(5));
	QASSERT(a.value == 5);

	QNEWOBJ(other, mtest.MetagenOperatorBitwiseAssign(6));
	QDO(a = a._opBitAndAssign(other)._opBitAndAssign("abcd"));
	QASSERT(a.value == 4);
}

#define CASE metagenTest_Operators_BitAndAssign
#include "do_testcase.h"


void metagenTest_Operators_BitOrAssign(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorBitwiseAssign(2));

	QDO(a = a._opBitOrAssign(5));
	QASSERT(a.value == 7);

	QNEWOBJ(other, mtest.MetagenOperatorBitwiseAssign(6));
	QDO(a = a._opBitOrAssign(other)._opBitOrAssign("abcd"));
	QASSERT(a.value == 7);
}

#define CASE metagenTest_Operators_BitOrAssign
#include "do_testcase.h"


void metagenTest_Operators_BitXorAssign(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorBitwiseAssign(10));

	QDO(a = a._opBitXorAssign(5));
	QASSERT(a.value == 15);

	QNEWOBJ(other, mtest.MetagenOperatorBitwiseAssign(6));
	QDO(a = a._opBitXorAssign(other)._opBitXorAssign("abcd"));
	QASSERT(a.value == 13);
}

#define CASE metagenTest_Operators_BitXorAssign
#include "do_testcase.h"


void metagenTest_Operators_LeftShiftAssign(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorBitwiseAssign(3));

	QDO(a = a._opLeftShiftAssign(1));
	QASSERT(a.value == 6);

	QNEWOBJ(other, mtest.MetagenOperatorBitwiseAssign(2));
	QDO(a = a._opLeftShiftAssign(other)._opLeftShiftAssign("abcd"));
	QASSERT(a.value == 384);
}

#define CASE metagenTest_Operators_LeftShiftAssign
#include "do_testcase.h"


void metagenTest_Operators_RightShiftAssign(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorBitwiseAssign(255));

	QDO(a = a._opRightShiftAssign(1));
	QASSERT(a.value == 127);

	QNEWOBJ(other, mtest.MetagenOperatorBitwiseAssign(2));
	QDO(a = a._opRightShiftAssign(other)._opRightShiftAssign("abcd"));
	QASSERT(a.value == 1);
}

#define CASE metagenTest_Operators_RightShiftAssign
#include "do_testcase.h"



}
