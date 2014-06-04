#include "../testmetagen.h"
#include "../../include/simpleoverridableincrementer.h"

namespace {


void testInheritFromScript_SimpleObject(TestScriptContext * context)
{
	DO("function DerivedSimpleObject() { }");
	QDO(DerivedSimpleObject.prototype = new mtest.SimpleObject());
	QNEWOBJ(a, DerivedSimpleObject());

	QDO(a.someRandomDynamicProperty = 5); // dynamic property is allowed in derived object
	QDO(obj = {'a':'b'}); // dynamic property is allowed in derived object
	QDO(a.someRandomDynamicObjectProperty = obj); // dynamic property is allowed in derived object

	QDO(a.getData().n = 78);
	QDO(a.getData().s = "def");
	QDO(a.getData().atom.value = 98);
	QASSERT(a.checkData(78));
	QASSERT(a.checkData("def"));
	QASSERT(a.checkAtom(98));

	QASSERT(a.someRandomDynamicProperty === 5);
	QASSERT(a.someRandomDynamicObjectProperty === obj);
}

#define CASE testInheritFromScript_SimpleObject
#include "../do_testcase_v8.h"


void testInheritFromScript_SimpleObject_HiddingCppMethodIsNotPossible(TestScriptContext * context)
{
	DO("function DerivedSimpleObject() {}");
	QDO(DerivedSimpleObject.prototype = new mtest.SimpleObject());
	QNEWOBJ(a, DerivedSimpleObject());

	QDO(a.checkData = function(d) { return d == "overriden"; });

	QDO(a.getData().n = 78);
	QDO(a.getData().s = "def");
	QDO(a.getData().atom.value = 98);
	QASSERT(a.checkData(78));
	QASSERT(a.checkData("def"));
	QNOT(a.checkData("overriden"));
}

#define CASE testInheritFromScript_SimpleObject_HiddingCppMethodIsNotPossible
#include "../do_testcase_v8.h"

void testInheritFromScript_SimpleOverridableIncrementer(TestScriptContext * context)
{
	QDO(cpgf._import("cpgf", "builtin.core"));
	QDO(DerivedClass = cpgf.cloneClass(mtest.SimpleOverridableIncrementerWrapper);)
	QDO(
		DerivedClass.increment = function() {
			if(!this.counter) {
				this.counter=this.init_data.counter;
			};
			this.counter++;
			return this.counter;
		}
	);

	QNEWOBJ(a, DerivedClass());
	QDO(a.init_data = {counter:0};);

	SimpleOverridableIncrementer * obj = NULL;
	IScriptObject * bindingApi = context->getBindingApi();
	if (bindingApi) {
		obj = static_cast<SimpleOverridableIncrementer *>(scriptGetValue(bindingApi, "a").toObjectAddress(NULL, NULL));
	} else {
		GScriptObject * bindingLib = context->getBindingLib();
		obj = static_cast<SimpleOverridableIncrementer *>(scriptGetValue(bindingLib, "a").toObjectAddress(NULL, NULL));
	}
	GEQUAL(1, obj->increment());
	GEQUAL(2, obj->increment());
}

#define CASE testInheritFromScript_SimpleOverridableIncrementer
#include "../do_testcase_v8.h"


}
