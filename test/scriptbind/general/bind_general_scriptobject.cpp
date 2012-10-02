#include "../testscriptbind.h"
#include "../testscriptbindmetadata2.h"


namespace {

void testCreateScriptObject(TestScriptContext * context)
{
	QNEWOBJ(existObj, TestObject());

	GScriptObject * bindingLib = context->getBindingLib();
	IScriptObject * bindingApi = context->getBindingApi();

	if(bindingLib != NULL) {
		GScopedPointer<GScriptObject> existScriptObject(bindingLib->createScriptObject("existObj"));
		GCHECK(! existScriptObject);
		
		GCHECK(bindingLib->valueIsNull("nso"));
		GScopedPointer<GScriptObject> firstScriptObject(bindingLib->createScriptObject("nso"));
		firstScriptObject->bindFundamental("ix", 38);
		
		GCHECK(bindingLib->valueIsNull("sec"));
		GScopedPointer<GScriptObject> secondScriptObject(bindingLib->createScriptObject("sec"));
		secondScriptObject->bindFundamental("iy", 6);
	}
	if(bindingApi != NULL) {
		GScopedInterface<IScriptObject> existScriptObject(bindingApi->createScriptObject("existObj"));
		GCHECK(! existScriptObject);
		
		GCHECK(bindingApi->valueIsNull("nso"));
		GScopedInterface<IScriptObject> newScriptObject(bindingApi->createScriptObject("nso"));
		GVariant v = 38;
		newScriptObject->bindFundamental("ix", &v.refData());

		GCHECK(bindingApi->valueIsNull("sec"));
		GScopedInterface<IScriptObject> secondScriptObject(bindingApi->createScriptObject("sec"));
		v = 6;
		secondScriptObject->bindFundamental("iy", &v.refData());
	}

	QASSERT(nso.ix == 38);
	QASSERT(sec.iy == 6);

	if(bindingLib != NULL) {
		GScopedPointer<GScriptObject> firstScriptObject(bindingLib->gainScriptObject("nso"));
		GCHECK(fromVariant<int>(firstScriptObject->getFundamental("ix")) == 38);
		
		GScopedPointer<GScriptObject> secondScriptObject(bindingLib->gainScriptObject("sec"));
		GCHECK(fromVariant<int>(secondScriptObject->getFundamental("iy")) == 6);
	}
	if(bindingApi != NULL) {
		GVariant v;
		GScopedInterface<IScriptObject> newScriptObject(bindingApi->gainScriptObject("nso"));
		newScriptObject->getFundamental(&v.refData(), "ix");
		GCHECK(fromVariant<int>(v) == 38);

		GScopedInterface<IScriptObject> secondScriptObject(bindingApi->gainScriptObject("sec"));
		v = 0;
		secondScriptObject->getFundamental(&v.refData(), "iy");
		GCHECK(fromVariant<int>(v) == 6);
	}
}

#define CASE testCreateScriptObject
#include "../bind_testcase.h"


void testGetScriptObject(TestScriptContext * context)
{
	QNEWOBJ(existObj, TestObject());

	QDO(scope = {});
	if(context->isLua()) {
		QDO(scope.obj = TestObject(6));
		QDO(scope.i = 5);
	}
	if(context->isV8()) {
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
		GScopedPointer<GScriptObject> existScriptObject(bindingLib->gainScriptObject("existObj"));
//		GCHECK(! existScriptObject);

		GScopedPointer<GScriptObject> newScriptObject(bindingLib->gainScriptObject("scope"));
		v = GVariant();
		v = newScriptObject->getFundamental("i");
		GCHECK(fromVariant<int>(v) == 5);
		instance = newScriptObject->getObject("obj");
	}
	if(bindingApi != NULL) {
		GScopedInterface<IScriptObject> existScriptObject(bindingApi->gainScriptObject("existObj"));
//		GCHECK(! existScriptObject);

		GCHECK(bindingApi->valueIsNull("nso"));
		GScopedInterface<IScriptObject> newScriptObject(bindingApi->gainScriptObject("scope"));
		v = GVariant();
		newScriptObject->getFundamental(&v.refData(), "i");
		GCHECK(fromVariant<int>(v) == 5);
		instance = newScriptObject->getObject("obj");
	}
	
	GCHECK(instance != NULL);
	if(instance)
	GCHECK(static_cast<TestObject *>(instance)->value == 6);
}

#define CASE testGetScriptObject
#include "../bind_testcase.h"


}
