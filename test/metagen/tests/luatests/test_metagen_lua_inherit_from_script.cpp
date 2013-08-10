#include "../testmetagen.h"

namespace {


const char * codeInheritsFrom = ""
"function inheritsFrom( baseClass ) \n"
"	local new_class = {} \n"
"	local class_mt = { __index = new_class } \n"
"	function new_class:create() \n"
"		local newinst = {} \n"
"		setmetatable( newinst, { __index = baseClass() } ) \n"
"		return newinst \n"
"	end \n"
"	return new_class \n"
"end \n"
;

void testInheritFromScript_SimpleObject(TestScriptContext * context)
{
	DO(codeInheritsFrom);
	DO("DerivedSimpleObject = inheritsFrom(mtest.SimpleObject)");
	QNEWOBJ(a, DerivedSimpleObject:create());
	QNEWOBJ(b, DerivedSimpleObject:create());

	QDO(a.someRandomDynamicProperty = 5); // dynamic property is allowed in derived object
	
	QDO(a.getData().n = 78);
	QDO(b.getData().n = 18);
	QDO(a.getData().s = "def");
	QDO(a.getData().atom.value = 98);
	QASSERT(a.checkData(78));
	QASSERT(b.checkData(18));
	QASSERT(a.checkData("def"));
	QASSERT(a.checkAtom(98));
}

#define CASE testInheritFromScript_SimpleObject
#include "../do_testcase_lua.h"


}
