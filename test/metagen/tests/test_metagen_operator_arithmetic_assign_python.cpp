#include "testmetagen.h"


namespace {


void metagenTest_Operators_Python_AddAssign(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorArithmeticAssign(3));

	QDO(a += 5);
	QASSERT(a.value == 8);

	QNEWOBJ(other, mtest.MetagenOperatorArithmeticAssign(12));
	QDO(a += other);
	QDO(a += "abcd");
	QASSERT(a.value == 24);
}

#define CASE metagenTest_Operators_Python_AddAssign
#include "do_testcase_python.h"


void metagenTest_Operators_Python_SubAssign(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorArithmeticAssign(30));

	QDO(a -= 5);
	QASSERT(a.value == 25);

	QNEWOBJ(other, mtest.MetagenOperatorArithmeticAssign(12));
	QDO(a -= other);
	QDO(a -= "abcd");
	QASSERT(a.value == 9);
}

#define CASE metagenTest_Operators_Python_SubAssign
#include "do_testcase_python.h"


void metagenTest_Operators_Python_MulAssign(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorArithmeticAssign(3));

	QDO(a *= 5);
	QASSERT(a.value == 15);

	QNEWOBJ(other, mtest.MetagenOperatorArithmeticAssign(2));
	QDO(a *= other);
	QDO(a *= "abcd");
	QASSERT(a.value == 120);
}

#define CASE metagenTest_Operators_Python_MulAssign
#include "do_testcase_python.h"


void metagenTest_Operators_Python_DivAssign(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorArithmeticAssign(192));

	QDO(a /= 2);
	QASSERT(a.value == 96);

	QNEWOBJ(other, mtest.MetagenOperatorArithmeticAssign(12));
	QDO(a /= other);
	QDO(a /= "abcd");
	QASSERT(a.value == 2);
}

#define CASE metagenTest_Operators_Python_DivAssign
#include "do_testcase_python.h"


void metagenTest_Operators_Python_ModAssign(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorArithmeticAssign(61));

	QDO(a %= 31);
	QASSERT(a.value == 30);

	QNEWOBJ(other, mtest.MetagenOperatorArithmeticAssign(12));
	QDO(a %= other);
	QDO(a %= "abcd");
	QASSERT(a.value == 2);
}

#define CASE metagenTest_Operators_Python_ModAssign
#include "do_testcase_python.h"




}
