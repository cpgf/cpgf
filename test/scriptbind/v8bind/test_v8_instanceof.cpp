#include "../bind_common.h"


namespace {

void testInstanceof(TestScriptContext * context)
{
	QNEWOBJ(a, DeriveE())
	QASSERT(a instanceof DeriveE)
}

#define CASE testInstanceof
#include "../testcase_v8.h"



}
