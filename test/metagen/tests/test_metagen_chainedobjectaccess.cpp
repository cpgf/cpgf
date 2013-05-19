#include "testmetagen.h"


using namespace std;

namespace {


void testChainedObjectAccess(TestScriptContext * context)
{
	QNEWOBJ(c, mtest.ChainedObjectC());
	QASSERT(c.getB().getA().getA() == 1);
	QASSERT(c.getConstB().getConstA().getA() == 1);
}

#define CASE testChainedObjectAccess
#include "do_testcase.h"



}

