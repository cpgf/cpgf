#include "testmetagen.h"


namespace {


void testMisc(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.SimpleData());
//	QASSERT(a.n == 0);
}


#define CASE testMisc
#include "do_testcase.h"


}
