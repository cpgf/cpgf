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
#include "../do_testcase_spidermonkey.h"


/*
// This test is to hide C++ method from script, it's not overriding. That's to say
// calling the method from C++ will call the C++ method, not the script method.
// To use overriding, we have to use class wrapper.
void testInheritFromScript_SimpleObject_HideCppMethod(TestScriptContext * context)
{
	DO("function DerivedSimpleObject() {}");
	QDO(DerivedSimpleObject.prototype = new mtest.SimpleObject());
	QNEWOBJ(a, DerivedSimpleObject());

	// Hide all C++ methods checkData
	QDO(a.checkData = function(d) { return d == "overrided"; });

	QDO(a.getData().n = 78);
	QDO(a.getData().s = "def");
	QDO(a.getData().atom.value = 98);
	QNOT(a.checkData(78));
	QNOT(a.checkData("def"));
	QASSERT(a.checkData("overrided"));
}

#define CASE testInheritFromScript_SimpleObject_HideCppMethod
#include "../do_testcase_spidermonkey.h"
*/


}
