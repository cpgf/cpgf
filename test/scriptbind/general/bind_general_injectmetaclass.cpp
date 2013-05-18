#include "../testscriptbind.h"
#include "../testscriptbindmetadata2.h"


namespace {

void doTestInjectMetaClass(IScriptObject * binding, TestScriptContext * context)
{
if(! context->isSpiderMonkey()) {
	QERR(methodConst())
	QERR(incStaticValue())
	QERR(add(18))

	QERR(BasicEnum.a == 1)
	QERR(BasicEnum.b == 2)
	QERR(BasicEnum.c == 3)
	QERR(a, Inner())
}

	GScopedInterface<IMetaClass> metaClass(context->getService()->findClassByName(REG_NAME_TestObject));
	TestObject obj(38);
	TestObject::staticValue = 6;
	
	injectObjectToScript(binding, metaClass.get(), &obj);
	
	QASSERT(methodConst() == 0)
	QASSERT(incStaticValue() == 7)
	QASSERT(add(18) == 38 + 18)

	metaClass.reset(context->getService()->findClassByName(REG_NAME_BasicA));
	
	injectObjectToScript(binding, metaClass.get(), NULL);
	
	QASSERT(BasicEnum.a == 1);
	QASSERT(BasicEnum.b == 2);
	QASSERT(BasicEnum.c == 3);
	QNEWOBJ(a, Inner());
	QASSERT(a.x == 5);
	QASSERT(a.add() == 6);
	QASSERT(a.x == 6);
}

void testInjectMetaClass(TestScriptContext * context)
{
	IScriptObject * bindingApi = context->getBindingApi();
	GScriptObject * bindingLib = context->getBindingLib();

	if(bindingLib) {
		GScopedInterface<IScriptObject> binding(scriptObjectToInterface(bindingLib, false));
		doTestInjectMetaClass(binding.get(), context);
	}
	
	if(bindingApi) {
		doTestInjectMetaClass(bindingApi, context);
	}
}

#define CASE testInjectMetaClass
#include "../bind_testcase.h"




}
