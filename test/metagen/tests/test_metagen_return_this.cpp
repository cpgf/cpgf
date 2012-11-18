#include "testmetagen.h"


namespace {


void metagenTest_ReturnThis(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenReturnThis());
	QASSERT(a.check());

	QDO(a = a.selfRef());
	QASSERT(a.check());

	QNEWOBJ(a, mtest.MetagenReturnThis());
	QASSERT(a.check());

	QDO(a = a.selfPointer());
	QASSERT(a.check());
}

#define CASE metagenTest_ReturnThis
#include "do_testcase.h"




}
