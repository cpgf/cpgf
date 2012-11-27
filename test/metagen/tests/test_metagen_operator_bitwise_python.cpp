#include "testmetagen.h"


namespace {


void metagenTest_Operators_Python_BitAnd(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorBitwise(3));

	QDO(b = a & 5);
	QASSERT(b.value == 1);

	QNEWOBJ(other, mtest.MetagenOperatorBitwise(6));
	QDO(b = a & other);
	QASSERT(b.value == 2);

	QDO(b = a & "abcd");
	QASSERT(b.value == 0);
}

#define CASE metagenTest_Operators_Python_BitAnd
#include "do_testcase_python.h"


void metagenTest_Operators_Python_BitOr(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorBitwise(3));

	QDO(b = a | 5);
	QASSERT(b.value == 7);

	QNEWOBJ(other, mtest.MetagenOperatorBitwise(12));
	QDO(b = a | other);
	QASSERT(b.value == 15);

	QDO(b = a | "abcd");
	QASSERT(b.value == 7);
}

#define CASE metagenTest_Operators_Python_BitOr
#include "do_testcase_python.h"


void metagenTest_Operators_Python_BitXor(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorBitwise(3));

	QDO(b = a ^ 5);
	QASSERT(b.value == 6);

	QNEWOBJ(other, mtest.MetagenOperatorBitwise(6));
	QDO(b = a ^ other);
	QASSERT(b.value == 5);

	QDO(b = a ^ "abcd");
	QASSERT(b.value == 7);
}

#define CASE metagenTest_Operators_Python_BitXor
#include "do_testcase_python.h"


void metagenTest_Operators_Python_LeftShift(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorBitwise(3));

	QDO(b = a << 5);
	QASSERT(b.value == 96);

	QNEWOBJ(other, mtest.MetagenOperatorBitwise(6));
	QDO(b = a << other);
	QASSERT(b.value == 192);

	QDO(b = a << "abcd");
	QASSERT(b.value == 48);
}

#define CASE metagenTest_Operators_Python_LeftShift
#include "do_testcase_python.h"


void metagenTest_Operators_Python_RightShift(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorBitwise(255));

	QDO(b = a >> 5);
	QASSERT(b.value == 7);

	QNEWOBJ(other, mtest.MetagenOperatorBitwise(6));
	QDO(b = a >> other);
	QASSERT(b.value == 3);

	QDO(b = a >> "abcd");
	QASSERT(b.value == 15);
}

#define CASE metagenTest_Operators_Python_RightShift
#include "do_testcase_python.h"


void metagenTest_Operators_Python_BitNot(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorBitwise(3));

	QDO(b = ~a);
	QASSERT(b.value == -4);
}

#define CASE metagenTest_Operators_Python_BitNot
#include "do_testcase_python.h"

}
