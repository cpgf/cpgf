#include "../testscriptbind.h"


namespace {

void testEnumObjectProperty(TestScriptContext * context)
{
	QNEWOBJ(a, DeriveA())

	QDO(namesA = new Array())
	QASSERT(namesA.indexOf("getA") < 0)
	QASSERT(namesA.indexOf("setData") < 0)
	QASSERT(namesA.indexOf("a") < 0)

	QDO(for(p in a) { namesA.push(p) })

	QASSERT(namesA.indexOf("getA") >= 0)
	QASSERT(namesA.indexOf("setData") >= 0)
	QASSERT(namesA.indexOf("a") >= 0)

	QNEWOBJ(b, DeriveB())
	QDO(namesB = new Array())
	QDO(for(p in b) { namesB.push(p) })
	QASSERT(namesB.indexOf("getA") >= 0)
	QASSERT(namesB.indexOf("setData") >= 0)
	QASSERT(namesB.indexOf("a") >= 0)
	QASSERT(namesB.indexOf("getB") >= 0)
	QASSERT(namesB.indexOf("b") >= 0)
}

#define CASE testEnumObjectProperty
#include "../testcase_v8.h"


void testEnumEnumProperty(TestScriptContext * context)
{
	QDO(namesA = new Array())
	QASSERT(namesA.indexOf("teCpp") < 0)
	QASSERT(namesA.indexOf("teLua") < 0)
	QASSERT(namesA.indexOf("teV8") < 0)

	QDO(for(p in TestEnum) { namesA.push(p) })

	QASSERT(namesA.indexOf("teCpp") >= 0)
	QASSERT(namesA.indexOf("teLua") >= 0)
	QASSERT(namesA.indexOf("teV8") >= 0)
}

#define CASE testEnumEnumProperty
#include "../testcase_v8.h"



}
