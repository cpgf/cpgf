#include "../bind_common.h"

namespace {

void testCreateScriptObject(TestScriptContext * context)
{
	QNEWOBJ(existObj, TestObject());

	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();

	if(bindingLib != NULL) {
		GScriptObject * existScriptObject = bindingLib->createScriptObject("existObj");
		GCHECK(existScriptObject == NULL);
		
		GCHECK(bindingLib->valueIsNull("nso"));
		GScriptObject * newScriptObject = bindingLib->createScriptObject("nso");
		newScriptObject->bindFundamental("ix", 38);
	}
	if(bindingApi != NULL) {
		GScopedInterface<IScriptObject> existScriptObject(bindingApi->createScriptObject("existObj"));
		GCHECK(! existScriptObject);
		
		GCHECK(bindingApi->valueIsNull("nso"));
		GScopedInterface<IScriptObject> newScriptObject(bindingApi->createScriptObject("nso"));
		GVariant v = 38;
		newScriptObject->bindFundamental("ix", &v.data);
	}
	
	QASSERT(nso.ix == 38);
}

#define CASE testCreateScriptObject
#include "../bind_testcase.h"


void testGetScriptObject(TestScriptContext * context)
{
	QNEWOBJ(existObj, TestObject());

	QDO(scope = {});
	QDO(scope.i = 5);
	if(context->isLua()) {
		QDO(scope.obj = TestObject(6));
	}
	if(context->isV8()) {
		QDO(scope.obj = new TestObject(6));
	}

	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();
	
	GVariant v;
	void * instance = NULL;

	if(bindingLib != NULL) {
		GScriptObject * existScriptObject = bindingLib->getScriptObject("existObj");
		GCHECK(existScriptObject == NULL);

		GScriptObject * newScriptObject = bindingLib->getScriptObject("scope");
		v = GVariant();
		v = newScriptObject->getFundamental("i");
		GCHECK(fromVariant<int>(v) == 5);
		instance = newScriptObject->getObject("obj");
	}
	if(bindingApi != NULL) {
		GScopedInterface<IScriptObject> existScriptObject(bindingApi->getScriptObject("existObj"));
		GCHECK(! existScriptObject);

		GCHECK(bindingApi->valueIsNull("nso"));
		GScopedInterface<IScriptObject> newScriptObject(bindingApi->getScriptObject("scope"));
		v = GVariant();
		newScriptObject->getFundamental(&v.data, "i");
		GCHECK(fromVariant<int>(v) == 5);
		instance = newScriptObject->getObject("obj");
	}
	
	GCHECK(instance != NULL);
	GCHECK(static_cast<TestObject *>(instance)->value == 6);
}

#define CASE testGetScriptObject
#include "../bind_testcase.h"


}
