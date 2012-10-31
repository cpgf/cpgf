#include "testmetagen.h"


namespace {


void metagenTest_MultipleInheritanceCast(TestScriptContext * context)
{
	QDO(d = mtest.getMultipleInheritanceCastD());
	QASSERT(mtest.checkMultipleInheritanceCastDAsR(d));

	QDO(r = mtest.getMultipleInheritanceCastDAsR());
	QASSERT(mtest.checkMultipleInheritanceCastRAsD(r));
}

#define CASE metagenTest_MultipleInheritanceCast
#include "do_testcase.h"



}
