#include "../bind_common.h"


namespace {

template <typename T>
void doTestAccessData(T * binding, TestScriptContext * context)
{
	QNEWOBJ(a, TestObject())
	QASSERT(a.data.x == 0)
	QASSERT(a.data.name == "")

	TestObject * obj;
	
	obj = static_cast<TestObject *>(binding->getObject("a"));
	obj->data.x = 38;
	obj->data.name = "blah";

	QASSERT(a.data.x == 38)
	QASSERT(a.data.name == "blah")

	QDO(a.data.x = 5)
	QDO(a.data.name = "script")

	obj = static_cast<TestObject *>(binding->getObject("a"));
	GCHECK(obj->data.x == 5);
	GCHECK(obj->data.name == "script");

	QNEWOBJ(b, TestData())
	QDO(b.x = 96)
	QDO(b.name = "what")
	QDO(a.data = b)

	obj = static_cast<TestObject *>(binding->getObject("a"));
	GCHECK(obj->data.x == 96);
	GCHECK(obj->data.name == "what");
}

void testAccessData(TestScriptContext * context)
{
	IScriptObject * bindingApi = context->getBindingApi();
	GScriptObject * bindingLib = context->getBindingLib();

	if(bindingLib) {
		doTestAccessData(bindingLib, context);
	}
	
	if(bindingApi) {
		doTestAccessData(bindingApi, context);
	}
}


#define CASE testAccessData
#include "../bind_testcase.h"


template <typename T>
void doTestRefData(T * binding, TestScriptContext * context)
{
	QNEWOBJ(a, TestObject())
	QASSERT(a.data.x == 0)
	QASSERT(a.data.name == "")

	TestObject * obj;
	
	QDO(b = a.refData())
	QDO(c = a.constRefData())
	
	obj = static_cast<TestObject *>(binding->getObject("a"));
	obj->data.x = 38;
	obj->data.name = "blah";

	QASSERT(b.x == 38)
	QASSERT(b.name == "blah")
	QASSERT(c.x == 38)
	QASSERT(c.name == "blah")

	QDO(b.x = 96)
	QDO(b.name = "what")

	QDO(b = a.pointerData())
	QDO(c = a.constPointerData())
	
	obj = static_cast<TestObject *>(binding->getObject("a"));
	GCHECK(obj->data.x == 96);
	GCHECK(obj->data.name == "what");
	
	QASSERT(b.x == 96)
	QASSERT(b.name == "what")
	QASSERT(c.x == 96)
	QASSERT(c.name == "what")

	QDO(b.x = 18)
	QDO(b.name = "how")
	QASSERT(c.x == 18)
	QASSERT(c.name == "how")

	obj = static_cast<TestObject *>(binding->getObject("a"));
	GCHECK(obj->data.x == 18);
	GCHECK(obj->data.name == "how");

	QERR(c.x = 1)
	QERR(c.name = "")
}

void testRefData(TestScriptContext * context)
{
	IScriptObject * bindingApi = context->getBindingApi();
	GScriptObject * bindingLib = context->getBindingLib();

	if(bindingLib) {
		doTestRefData(bindingLib, context);
	}
	
	if(bindingApi) {
		doTestRefData(bindingApi, context);
	}
}


#define CASE testRefData
#include "../bind_testcase.h"


template <typename T>
void doTestDataConstness(T * binding, TestScriptContext * context)
{
	QNEWOBJ(a, TestObject())
	
	QDO(b = a.selfConst())
	QASSERT(a.data.x == 0)
	QASSERT(a.data.name == "")

	TestObject * obj;
	
	obj = static_cast<TestObject *>(binding->getObject("a"));
	obj->data.x = 38;
	obj->data.name = "blah";

	QASSERT(b.data.x == 38)
	QASSERT(b.data.name == "blah")

	QERR(b.data.x = 1)
	QERR(b.data.name = "")
}

void testDataConstness(TestScriptContext * context)
{
	IScriptObject * bindingApi = context->getBindingApi();
	GScriptObject * bindingLib = context->getBindingLib();

	if(bindingLib) {
		doTestDataConstness(bindingLib, context);
	}
	
	if(bindingApi) {
		doTestDataConstness(bindingApi, context);
	}
}


#define CASE testDataConstness
#include "../bind_testcase.h"


}
