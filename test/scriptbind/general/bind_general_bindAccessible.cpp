#include "../testscriptbind.h"
#include "../testscriptbindmetadata2.h"


namespace {

template <typename T>
void doTestBindAccessible(T * binding, TestScriptContext * context)
{
	GScopedInterface<IMetaClass> metaClass(context->getService()->findClassByName(REG_NAME_TestObject));
	GCHECK(metaClass);

	TestObject bound(10);
	bound.data.x = 0;
	bound.data.name = "";

	GScopedInterface<IMetaField> field;
	
	field.reset(metaClass->getField("data"));
	scriptSetValue(binding, "baData", GScriptValue::fromAccessible(&bound, field.get()));

	if(context->isPython()) {
		QDO(baData.__get__(0).x = 38);
		QDO(baData.__get__(0).name = "aha");
	}
	else {
		QDO(baData.x = 38);
		QDO(baData.name = "aha");
	}

	GEQUAL(bound.data.x, 38);
	GEQUAL(bound.data.name, "aha");
}

void testBindAccessible(TestScriptContext * context)
{
	IScriptObject * bindingApi = context->getBindingApi();
	GScriptObject * bindingLib = context->getBindingLib();

	if(bindingLib) {
		doTestBindAccessible(bindingLib, context);
	}
	
	if(bindingApi) {
		doTestBindAccessible(bindingApi, context);
	}
}

#define CASE testBindAccessible
#include "../bind_testcase.h"




}
