#include "../testscriptbind.h"


namespace {

void test__Proto__(TestScriptContext * context)
{
	QNEWOBJ(a, TestObject(3))
	QDO(b = {})
	QDO(b.__proto__ = a)
	QASSERT(b.add(5) == 8)

	QDO(f = a.add)
	QERR(f(5))

	QNEWOBJ(a, DeriveE())
	QDO(b = {})
	QNOT(b instanceof DeriveE)
	QNOT(b instanceof DeriveD)
	QNOT(b instanceof DeriveB)
	QNOT(b instanceof DeriveA)
	QDO(b.__proto__ = a)
	QASSERT(b instanceof DeriveE)
	QASSERT(b instanceof DeriveD)
	QASSERT(b instanceof DeriveB)
	QASSERT(b instanceof DeriveA)
	QNOT(b instanceof DeriveC)
}

#define CASE test__Proto__
#include "../testcase_v8.h"



}
