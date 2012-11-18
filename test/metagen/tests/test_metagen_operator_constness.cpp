#include "testmetagen.h"


namespace {


void metagenTest_Operators_Constness(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenOperatorConstness(10));

	QDO(b = a._opAdd(5));
	QASSERT(b.value == 15);
	
	QDO(c = a.constSelf());
	QERR(c._opAdd(6));
	QDO(b = c._opSub(3));
	QASSERT(b.value == 7);
	
	QDO(c = a.constSelfRef());
	QERR(c._opAdd(9));
	QDO(b = c._opSub(2));
	QASSERT(b.value == 8);
}

#define CASE metagenTest_Operators_Constness
#include "do_testcase.h"



}
