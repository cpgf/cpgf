#include "testmetagen.h"


namespace {


void metagenTest_Method_Overload_ConstMethod_ConstFirst(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenMethodOverload());
	QASSERT(a.testOverloadByConst_ConstFirst() == 2);

	QDO(b = a.makeConst());
	QASSERT(b.testOverloadByConst_ConstFirst() == 1);
}

#define CASE metagenTest_Method_Overload_ConstMethod_ConstFirst
#include "do_testcase.h"


void metagenTest_Method_Overload_ConstMethod_NonConstFirst(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenMethodOverload());
	QASSERT(a.testOverloadByConst_NonConstFirst() == 3);

	QDO(b = a.makeConst());
	QASSERT(b.testOverloadByConst_NonConstFirst() == 4);
}

#define CASE metagenTest_Method_Overload_ConstMethod_NonConstFirst
#include "do_testcase.h"


}
