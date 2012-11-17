#include "testmetagen.h"


namespace {


void metagenTest_Operators_AddAssign(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorArithmeticAssign(3));

	QDO(a = a._opAddAssign(5));
	QASSERT(a.value == 8);

	QNEWOBJ(other, mtest.MetagenOperatorArithmeticAssign(12));
	QDO(a = a._opAddAssign(other)._opAddAssign("abcd"));
	QASSERT(a.value == 24);
}

#define CASE metagenTest_Operators_AddAssign
#include "do_testcase.h"


void metagenTest_Operators_SubAssign(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorArithmeticAssign(30));

	QDO(a = a._opSubAssign(5));
	QASSERT(a.value == 25);

	QNEWOBJ(other, mtest.MetagenOperatorArithmeticAssign(12));
	QDO(a = a._opSubAssign(other)._opSubAssign("abcd"));
	QASSERT(a.value == 9);
}

#define CASE metagenTest_Operators_SubAssign
#include "do_testcase.h"


void metagenTest_Operators_MulAssign(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorArithmeticAssign(3));

	QDO(a = a._opMulAssign(5));
	QASSERT(a.value == 15);

	QNEWOBJ(other, mtest.MetagenOperatorArithmeticAssign(2));
	QDO(a = a._opMulAssign(other)._opMulAssign("abcd"));
	QASSERT(a.value == 120);
}

#define CASE metagenTest_Operators_MulAssign
#include "do_testcase.h"


void metagenTest_Operators_DivAssign(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorArithmeticAssign(192));

	QDO(a = a._opDivAssign(2));
	QASSERT(a.value == 96);

	QNEWOBJ(other, mtest.MetagenOperatorArithmeticAssign(12));
	QDO(a = a._opDivAssign(other)._opDivAssign("abcd"));
	QASSERT(a.value == 2);
}

#define CASE metagenTest_Operators_DivAssign
#include "do_testcase.h"


void metagenTest_Operators_ModAssign(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorArithmeticAssign(61));

	QDO(a = a._opModAssign(31));
	QASSERT(a.value == 30);

	QNEWOBJ(other, mtest.MetagenOperatorArithmeticAssign(12));
	QDO(a = a._opModAssign(other)._opModAssign("abcd"));
	QASSERT(a.value == 2);
}

#define CASE metagenTest_Operators_ModAssign
#include "do_testcase.h"




}
