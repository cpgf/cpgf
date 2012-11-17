#include "testmetagen.h"


namespace {


void metagenTest_Operators_BitAnd(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorBitwise(3));

	QDO(b = a._opBitAnd(5));
	QASSERT(b.value == 1);

	QNEWOBJ(other, mtest.MetagenOperatorBitwise(6));
	QDO(b = a._opBitAnd(other));
	QASSERT(b.value == 2);

	QDO(b = a._opBitAnd("abcd"));
	QASSERT(b.value == 0);
}

#define CASE metagenTest_Operators_BitAnd
#include "do_testcase.h"


void metagenTest_Operators_BitOr(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorBitwise(3));

	QDO(b = a._opBitOr(5));
	QASSERT(b.value == 7);

	QNEWOBJ(other, mtest.MetagenOperatorBitwise(12));
	QDO(b = a._opBitOr(other));
	QASSERT(b.value == 15);

	QDO(b = a._opBitOr("abcd"));
	QASSERT(b.value == 7);
}

#define CASE metagenTest_Operators_BitOr
#include "do_testcase.h"


void metagenTest_Operators_BitXor(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorBitwise(3));

	QDO(b = a._opBitXor(5));
	QASSERT(b.value == 6);

	QNEWOBJ(other, mtest.MetagenOperatorBitwise(6));
	QDO(b = a._opBitXor(other));
	QASSERT(b.value == 5);

	QDO(b = a._opBitXor("abcd"));
	QASSERT(b.value == 7);
}

#define CASE metagenTest_Operators_BitXor
#include "do_testcase.h"


void metagenTest_Operators_LeftShift(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorBitwise(3));

	QDO(b = a._opLeftShift(5));
	QASSERT(b.value == 96);

	QNEWOBJ(other, mtest.MetagenOperatorBitwise(6));
	QDO(b = a._opLeftShift(other));
	QASSERT(b.value == 192);

	QDO(b = a._opLeftShift("abcd"));
	QASSERT(b.value == 48);
}

#define CASE metagenTest_Operators_LeftShift
#include "do_testcase.h"


void metagenTest_Operators_RightShift(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorBitwise(255));

	QDO(b = a._opRightShift(5));
	QASSERT(b.value == 7);

	QNEWOBJ(other, mtest.MetagenOperatorBitwise(6));
	QDO(b = a._opRightShift(other));
	QASSERT(b.value == 3);

	QDO(b = a._opRightShift("abcd"));
	QASSERT(b.value == 15);
}

#define CASE metagenTest_Operators_RightShift
#include "do_testcase.h"


void metagenTest_Operators_BitNot(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorBitwise(3));

	QDO(b = a._opBitNot());
	QASSERT(b.value == -4);
}

#define CASE metagenTest_Operators_BitNot
#include "do_testcase.h"

}
