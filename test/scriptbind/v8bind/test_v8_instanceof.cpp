#include "../testscriptbind.h"


namespace {

void testInstanceof(TestScriptContext * context)
{
	QNEWOBJ(a, DeriveE())
	QASSERT(a instanceof DeriveE)
	QASSERT(a instanceof DeriveD)
	QASSERT(a instanceof DeriveB)
	QASSERT(a instanceof DeriveA)
	QNOT(a instanceof DeriveC)

	QNEWOBJ(a, DeriveD())
	QASSERT(a instanceof DeriveB)
	QASSERT(a instanceof DeriveA)
	QASSERT(a instanceof DeriveD)
	QNOT(a instanceof DeriveE)
	QNOT(a instanceof DeriveC)

	QNEWOBJ(a, DeriveC())
	QASSERT(a instanceof DeriveA)
	QASSERT(a instanceof DeriveC)
	QNOT(a instanceof DeriveB)
	QNOT(a instanceof DeriveE)
	QNOT(a instanceof DeriveD)

	QNEWOBJ(a, DeriveB())
	QASSERT(a instanceof DeriveA)
	QASSERT(a instanceof DeriveB)
	QNOT(a instanceof DeriveE)
	QNOT(a instanceof DeriveD)
	QNOT(a instanceof DeriveC)

	QNEWOBJ(a, DeriveA())
	QASSERT(a instanceof DeriveA)
	QNOT(a instanceof DeriveB)
	QNOT(a instanceof DeriveE)
	QNOT(a instanceof DeriveD)
	QNOT(a instanceof DeriveC)
}

#define CASE testInstanceof
#include "../testcase_v8.h"



}
