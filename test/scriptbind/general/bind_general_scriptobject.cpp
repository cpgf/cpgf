#include "../testscriptbind.h"
#include "../testscriptbindmetadata2.h"


namespace {

void testCreateScriptObject(TestScriptContext * context)
{
	QVARNEWOBJ(existObj, TestObject());

	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();

	if(bindingLib != NULL) {
		GScopedPointer<GScriptObject> existScriptObject(bindingLib->createScriptObject("existObj"));
		GCHECK(! existScriptObject);
		
		GCHECK(bindingLib->getValue("nso").isNull());
		GScopedPointer<GScriptObject> firstScriptObject(bindingLib->createScriptObject("nso"));
		firstScriptObject->bindFundamental("ix", 38);
		
		GCHECK(bindingLib->getValue("sec.third.fourth").isNull());
		GScopedPointer<GScriptObject> secondScriptObject(bindingLib->createScriptObject("sec.third.fourth"));
		secondScriptObject->bindFundamental("iy", 6);
	}
	if(bindingApi != NULL) {
		GScopedInterface<IScriptObject> existScriptObject(bindingApi->createScriptObject("existObj"));
		GCHECK(! existScriptObject);
		
		GCHECK(scriptGetValue(bindingApi, "nso").isNull());
		GScopedInterface<IScriptObject> newScriptObject(bindingApi->createScriptObject("nso"));
		GVariant v = 38;
		newScriptObject->bindFundamental("ix", &v.refData());

		GCHECK(scriptGetValue(bindingApi, "sec.third.fourth").isNull());
		GScopedInterface<IScriptObject> secondScriptObject(bindingApi->createScriptObject("sec.third.fourth"));
		v = 6;
		secondScriptObject->bindFundamental("iy", &v.refData());
	}

	QASSERT(nso.ix == 38);
	QASSERT(sec.third.fourth.iy == 6);

	if(bindingLib != NULL) {
		GScopedPointer<GScriptObject> firstScriptObject(bindingLib->createScriptObject("nso"));
		GCHECK(fromVariant<int>(firstScriptObject->getValue("ix").toFundamental()) == 38);
		
		GScopedPointer<GScriptObject> secondScriptObject(bindingLib->createScriptObject("sec.third.fourth"));
		GCHECK(fromVariant<int>(secondScriptObject->getValue("iy").toFundamental()) == 6);
	}
	if(bindingApi != NULL) {
		GVariant v;
		GScopedInterface<IScriptObject> newScriptObject(bindingApi->createScriptObject("nso"));
		v = scriptGetValue(newScriptObject.get(), "ix").toFundamental();
		GCHECK(fromVariant<int>(v) == 38);

		GScopedInterface<IScriptObject> secondScriptObject(bindingApi->createScriptObject("sec.third.fourth"));
		v = 0;
		v = scriptGetValue(secondScriptObject.get(), "iy").toFundamental();
		GCHECK(fromVariant<int>(v) == 6);
	}
}

#define CASE testCreateScriptObject
#include "../bind_testcase.h"


void testGetScriptObject(TestScriptContext * context)
{
	QVARNEWOBJ(existObj, TestObject());

	QVAR(scope = {});
	if(context->isLua()) {
		QDO(scope.obj = TestObject(6));
		QDO(scope.i = 5);
	}
	if(context->isV8() || context->isSpiderMonkey()) {
		QDO(scope.obj = new TestObject(6));
		QDO(scope.i = 5);
	}
	if(context->isPython()) {
		QDO(scope['obj'] = TestObject(6));
		QDO(scope['i'] = 5);
	}

	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();
	
	GVariant v;
	void * instance = NULL;

	if(bindingLib != NULL) {
		GScopedPointer<GScriptObject> existScriptObject(bindingLib->createScriptObject("existObj"));
//		GCHECK(! existScriptObject);

		GScopedPointer<GScriptObject> newScriptObject(bindingLib->createScriptObject("scope"));
		v = GVariant();
		v = newScriptObject->getValue("i").toFundamental();
		GCHECK(fromVariant<int>(v) == 5);
		instance = newScriptObject->getValue("obj").toObjectAddress(NULL, NULL);
	}
	if(bindingApi != NULL) {
		GScopedInterface<IScriptObject> existScriptObject(bindingApi->createScriptObject("existObj"));
//		GCHECK(! existScriptObject);

		GScopedInterface<IScriptObject> newScriptObject(bindingApi->createScriptObject("scope"));
		v = scriptGetValue(newScriptObject.get(), "i").toFundamental();
		GCHECK(fromVariant<int>(v) == 5);
		instance = scriptGetValue(newScriptObject.get(), "obj").toObjectAddress(NULL, NULL);
	}
	
	GCHECK(instance != NULL);
	if(instance)
	GCHECK(static_cast<TestObject *>(instance)->value == 6);
}

#define CASE testGetScriptObject
#include "../bind_testcase.h"


}
