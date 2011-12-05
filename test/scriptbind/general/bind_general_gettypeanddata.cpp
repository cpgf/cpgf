#include "../bind_common.h"

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

	QNEWOBJ(obj, TestObject())
	QDO(s = "abc")
	QDO(i = 38)
	QDO(f = 3.14159265)

	RITEM;
	GCHECK(binding->getType("notExist", &tempItem) == sdtNull);
	CHKITEM;
	GCHECK(! item);

	RITEM;
	GCHECK(binding->getType("obj", &tempItem) == sdtObject);
	CHKITEM;
	GCHECK(item);
	GCHECK(string(item->getName()) == REG_NAME_TestObject);

//#error memory leak!!!!!!!!
	RITEM;
	GCHECK(binding->getType("TestObject", &tempItem) == sdtClass);
	CHKITEM;
	GCHECK(item);
	GCHECK(string(item->getName()) == REG_NAME_TestObject);

	RITEM;
	GCHECK(binding->getType("TestEnum", &tempItem) == sdtEnum);
	CHKITEM;
	GCHECK(item);
	GCHECK(string(item->getName()) == REG_NAME_TestEnum);

	RITEM;
	GCHECK(binding->getType("s", &tempItem) == sdtString);
	CHKITEM;
	GCHECK(! item);

	RITEM;
	GCHECK(binding->getType("i", &tempItem) == sdtFundamental);
	CHKITEM;
	GCHECK(! item);

	RITEM;
	GCHECK(binding->getType("f", &tempItem) == sdtFundamental);
	CHKITEM;
	GCHECK(! item);

	RITEM;
	if(context->isLua()) {
		GCHECK(binding->getType("scriptAssert", &tempItem) == sdtScriptMethod);
	}
	if(context->isV8()) {
		GCHECK(binding->getType("scriptAssert", &tempItem) == sdtMethodList);
	}
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
void doTestGetClass(T * binding, TestScriptContext * context)
{
	(void)context;

	GScopedInterface<IMetaClass> metaClass;
	
	metaClass.reset(binding->getClass("TestObject"));
	GCHECK(string(metaClass->getName()) == REG_NAME_TestObject);

	GCHECK(binding->getClass("notExistClass") == NULL);
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

	metaEnum.reset(binding->getEnum("TestEnum"));
	GCHECK(string(metaEnum->getName()) == REG_NAME_TestEnum);

	GCHECK(binding->getEnum("notExistEnum") == NULL);
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
	QNEWOBJ(obj, TestObject(99))
	QDO(f = "abc")
	
	void * instance;

	instance = binding->getObject("f");
	GCHECK(instance == NULL);
		
	instance = binding->getObject("obj");
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
	QDO(f = 100)
	QDO(s = "abc")
	
	GVariant v;

	if(context->getBindingLib()) {
		v = context->getBindingLib()->getFundamental("f");
		GCHECK(fromVariant<int>(v) == 100);
		v = context->getBindingLib()->getFundamental("s");
		GCHECK(v.isEmpty());
	}
	
	if(context->getBindingApi()) {
		context->getBindingApi()->getFundamental(&v.data, "f");
		GCHECK(fromVariant<int>(v) == 100);
		context->getBindingApi()->getFundamental(&v.data, "s");
		GCHECK(v.isEmpty());
	}
}

#define CASE testGetFundamental
#include "../bind_testcase.h"


void testGetString(TestScriptContext * context)
{
	QDO(oneString = "what")

	if(context->getBindingLib()) {
		GCHECK(context->getBindingLib()->getString("oneString") == "what");
	}
	
	if(context->getBindingApi()) {
		GScopedInterface<IMemoryAllocator> allocator(context->getService()->getAllocator());
		char * cs = context->getBindingApi()->getString("oneString", allocator.get());
		string s = cs;
		allocator->free(cs);
		GCHECK(s == "what");
	}
}

#define CASE testGetString
#include "../bind_testcase.h"

}
