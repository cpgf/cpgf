#include "../testmetagen.h"

namespace {


void testInheritFromScript_SimpleObject(TestScriptContext * context)
{
	DO("function DerivedSimpleObject() {}");
	QDO(DerivedSimpleObject.prototype = new mtest.SimpleObject());
	QNEWOBJ(a, DerivedSimpleObject());

	QDO(a.someRandomDynamicProperty = 5); // dynamic property is allowed in derived object
	
	QDO(a.getData().n = 78);
	QDO(a.getData().s = "def");
	QDO(a.getData().atom.value = 98);
	QASSERT(a.checkData(78));
	QASSERT(a.checkData("def"));
	QASSERT(a.checkAtom(98));
}

#define CASE testInheritFromScript_SimpleObject
#include "../do_testcase_v8.h"



}
