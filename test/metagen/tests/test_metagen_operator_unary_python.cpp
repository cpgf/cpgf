#include "testmetagen.h"


namespace {


void metagenTest_Operators_Python_Plus(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorUnary(10));
	QDO(b = +a);
	QASSERT(b.value == 10);

	QNEWOBJ(a, mtest.MetagenOperatorUnary(-5));
	QDO(b = +a);
	QASSERT(b.value == 5);
}

#define CASE metagenTest_Operators_Python_Plus
#include "do_testcase_python.h"


void metagenTest_Operators_Python_Minus(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorUnary(10));
	QDO(b = -a);
	QASSERT(b.value == -10);

	QNEWOBJ(a, mtest.MetagenOperatorUnary(-5));
	QDO(b = -a);
	QASSERT(b.value == 5);
}

#define CASE metagenTest_Operators_Python_Minus
#include "do_testcase_python.h"




}
