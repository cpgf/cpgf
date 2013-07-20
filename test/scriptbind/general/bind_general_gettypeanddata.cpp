#include "../testscriptbind.h"
#include "../testscriptbindmetadata2.h"


#define RITEM tempItem = (IMetaTypedItem *)100
#define CHKITEM \
	item.reset(tempItem); \
	GCHECK(tempItem != (IMetaTypedItem *)100)

namespace {

template <typename T>
void doTestGetType(T * binding, TestScriptContext * context)
{
	GScopedInterface<IMetaTypedItem> item;
	IMetaTypedItem * tempItem;
	GScriptValue value;

	QVARNEWOBJ(obj, TestObject())
	QVAR(s = "abc")
	QVAR(i = 38)
	QVAR(f = 3.14159265)
	QVAR(func = obj.add)

	RITEM;
	value = scriptGetValue(binding, "notExist");
	GCHECK(value.getType() == GScriptValue::typeNull);
	tempItem = getTypedItemFromScriptValue(value);
	CHKITEM;
	GCHECK(! item);

	RITEM;
	value = scriptGetValue(binding, "obj");
	GCHECK(value.getType() == GScriptValue::typeObject);
	tempItem = getTypedItemFromScriptValue(value);
	CHKITEM;
	GCHECK(item);
	GCHECK(string(item->getName()) == REG_NAME_TestObject);

	RITEM;
	value = scriptGetValue(binding, "func");
	GCHECK(value.getType() == GScriptValue::typeMethod);
	tempItem = getTypedItemFromScriptValue(value);
	CHKITEM;
	GCHECK(! item);

	RITEM;
	value = scriptGetValue(binding, "TestObject");
	GCHECK(value.getType() == GScriptValue::typeClass);
	tempItem = getTypedItemFromScriptValue(value);
	CHKITEM;
	GCHECK(item);
	GCHECK(string(item->getName()) == REG_NAME_TestObject);

	RITEM;
	value = scriptGetValue(binding, "TestEnum");
	GCHECK(value.getType() == GScriptValue::typeEnum);
	tempItem = getTypedItemFromScriptValue(value);
	CHKITEM;
	GCHECK(item);
	GCHECK(string(item->getName()) == REG_NAME_TestEnum);

	RITEM;
	value = scriptGetValue(binding, "s");
	GCHECK(value.getType() == GScriptValue::typeString);
	tempItem = getTypedItemFromScriptValue(value);
	CHKITEM;
	GCHECK(! item);

	RITEM;
	value = scriptGetValue(binding, "i");
	GCHECK(value.getType() == GScriptValue::typeFundamental);
	tempItem = getTypedItemFromScriptValue(value);
	CHKITEM;
	GCHECK(! item);

	RITEM;
	value = scriptGetValue(binding, "f");
	GCHECK(value.getType() == GScriptValue::typeFundamental);
	tempItem = getTypedItemFromScriptValue(value);
	CHKITEM;
	GCHECK(! item);

	RITEM;
	value = scriptGetValue(binding, "scriptAssert");
	GCHECK(value.getType() == GScriptValue::typeMethod);
	tempItem = getTypedItemFromScriptValue(value);
	CHKITEM;
	GCHECK(! item);

	RITEM;
	value = scriptGetValue(binding, "testAdd");
	GCHECK(value.getType() == GScriptValue::typeOverloadedMethods);
	tempItem = getTypedItemFromScriptValue(value);
	CHKITEM;
	GCHECK(! item);
}

void testGetType(TestScriptContext * context)
{
	if(context->getBindingLib() != NULL) {
		doTestGetType(context->getBindingLib(), context);
	}
	if(context->getBindingApi() != NULL) {
		doTestGetType(context->getBindingApi(), context);
	}
}

#define CASE testGetType
#include "../bind_testcase.h"


template <typename T>
void doTestGetMethod(T * binding, TestScriptContext * context)
{
	(void)context;

	GScopedInterface<IMetaMethod> method;

	GCHECK(scriptGetValue(binding, "notExist").toMethod(NULL) == NULL);
	
	method.reset(scriptGetValue(binding, "scriptAssert").toMethod(NULL));
	GCHECK(method);
}

void testGetMethod(TestScriptContext * context)
{
	if(context->getBindingLib() != NULL) {
		doTestGetMethod(context->getBindingLib(), context);
	}
	if(context->getBindingApi() != NULL) {
		doTestGetMethod(context->getBindingApi(), context);
	}
}

#define CASE testGetMethod
#include "../bind_testcase.h"


template <typename T>
void doTestGetMethodList(T * binding, TestScriptContext * context)
{
	(void)context;

	GScopedInterface<IMetaList> methodList;

	GCHECK(scriptGetValue(binding, "notExist").toOverloadedMethods() == NULL);
	
	methodList.reset(scriptGetValue(binding, "testAdd").toOverloadedMethods());
	GCHECK(methodList);
	GCHECK(methodList->getCount() == 3);
}

void testGetMethodList(TestScriptContext * context)
{
	if(context->getBindingLib() != NULL) {
		doTestGetMethodList(context->getBindingLib(), context);
	}
	if(context->getBindingApi() != NULL) {
		doTestGetMethodList(context->getBindingApi(), context);
	}
}

#define CASE testGetMethodList
#include "../bind_testcase.h"


template <typename T>
void doTestGetClass(T * binding, TestScriptContext * context)
{
	(void)context;

	GScopedInterface<IMetaClass> metaClass;
	
	metaClass.reset(scriptGetValue(binding, "TestObject").toClass());
	GCHECK(string(metaClass->getName()) == REG_NAME_TestObject);

	GCHECK(scriptGetValue(binding, "notExistClass").toClass() == NULL);
}

void testGetClass(TestScriptContext * context)
{
	if(context->getBindingLib() != NULL) {
		doTestGetClass(context->getBindingLib(), context);
	}
	if(context->getBindingApi() != NULL) {
		doTestGetClass(context->getBindingApi(), context);
	}
}

#define CASE testGetClass
#include "../bind_testcase.h"


template <typename T>
void doTestGetEnum(T * binding, TestScriptContext * context)
{
	(void)context;

	GScopedInterface<IMetaEnum> metaEnum;

	metaEnum.reset(scriptGetValue(binding, "TestEnum").toEnum());
	GCHECK(string(metaEnum->getName()) == REG_NAME_TestEnum);

	GCHECK(scriptGetValue(binding, "notExistEnum").toEnum() == NULL);
}

void testGetEnum(TestScriptContext * context)
{
	if(context->getBindingLib() != NULL) {
		doTestGetEnum(context->getBindingLib(), context);
	}
	if(context->getBindingApi() != NULL) {
		doTestGetEnum(context->getBindingApi(), context);
	}
}

#define CASE testGetEnum
#include "../bind_testcase.h"


template <typename T>
void doTestGetObject(T * binding, TestScriptContext * context)
{
	QVARNEWOBJ(obj, TestObject(99))
	QVAR(f = "abc")
	
	void * instance = NULL;

	instance = scriptGetValue(binding, "f").toObjectAddress(NULL, NULL);
	GCHECK(instance == NULL);
		
	instance = scriptGetValue(binding, "obj").toObjectAddress(NULL, NULL);
	GCHECK(instance != NULL);
	GCHECK(static_cast<TestObject *>(instance)->value == 99);
}

void testGetObject(TestScriptContext * context)
{
	if(context->getBindingLib() != NULL) {
		doTestGetObject(context->getBindingLib(), context);
	}
	if(context->getBindingApi() != NULL) {
		doTestGetObject(context->getBindingApi(), context);
	}
}

#define CASE testGetObject
#include "../bind_testcase.h"


void testGetFundamental(TestScriptContext * context)
{
	QVAR(f = 100)
	QVAR(s = "abc")
	
	GVariant v;

	if(context->getBindingLib()) {
		v = scriptGetValue(context->getBindingLib(), "f").toFundamental();
		GCHECK(fromVariant<int>(v) == 100);
		v = scriptGetValue(context->getBindingLib(), "s").toFundamental();
		GCHECK(v.isEmpty());
	}
	
	if(context->getBindingApi()) {
		v = scriptGetValue(context->getBindingApi(), "f").toFundamental();
		GCHECK(fromVariant<int>(v) == 100);
		v = scriptGetValue(context->getBindingApi(), "s").toFundamental();
		GCHECK(v.isEmpty());
	}
}

#define CASE testGetFundamental
#include "../bind_testcase.h"


void testGetString(TestScriptContext * context)
{
	QVAR(oneString = "what")

	if(context->getBindingLib()) {
		GCHECK(scriptGetValue(context->getBindingLib(), "oneString").toString() == "what");
	}
	
	if(context->getBindingApi()) {
		string s = scriptGetValue(context->getBindingApi(), "oneString").toString();
		GCHECK(s == "what");
	}
}

#define CASE testGetString
#include "../bind_testcase.h"

}
