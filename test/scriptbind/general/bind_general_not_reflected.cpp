#include "../testscriptbind.h"
#include "../testscriptbindmetadata2.h"


namespace {

template <typename T>
void doTestNotReflected(T * binding, TestScriptContext * context)
{
	(void)binding;

	QNEWOBJ(obj, TestObject())
	QDO(a = obj.pointerRaw())
	QASSERT(obj.isRawPointer(a))
	QASSERT(obj.isRawRef(a))
	QDO(obj.setRaw(a, 38, "what"))

	QNEWOBJ(obj2, TestObject())
	QDO(b = obj2.refRaw())
	QASSERT(obj2.isRawPointer(b))
	QASSERT(obj2.isRawRef(b))
	QDO(obj2.setRaw(b, 19, "how"))

	QERR(obj2.isRawPointer(18))
	QERR(obj2.isRawPointer("cd"))
	
	QERR(obj2.isRawRef(0))
	QERR(obj2.isRawRef("cd"))
}

void testNotReflected(TestScriptContext * context)
{
	IScriptObject * bindingApi = context->getBindingApi();
	GScriptObject * bindingLib = context->getBindingLib();

	GVariant a;
	GVariant b;

	if(bindingLib) {
		doTestNotReflected(bindingLib, context);

		a = bindingLib->getRaw("a");
		b = bindingLib->getRaw("b");
	}
	
	if(bindingApi) {
		doTestNotReflected(bindingApi, context);
		
		bindingApi->getRaw(&a.data, "a");
		bindingApi->getRaw(&b.data, "b");
	}

	GCHECK(!a.isEmpty());
	GCHECK(!b.isEmpty());

	GCHECK(fromVariant<RawData *>(a)->n == 38);
	GCHECK(fromVariant<RawData *>(a)->s == "what");

	GCHECK(fromVariant<RawData &>(b).n == 19);
	GCHECK(fromVariant<RawData &>(b).s == "how");
}

#define CASE testNotReflected
#include "../bind_testcase.h"



}
