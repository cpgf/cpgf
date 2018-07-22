#include "../testscriptbind.h"
#include "../testscriptbindmetadata6.h"

namespace {

void testObjectLeak(TestScriptContext * context)
{
	TestObjectLeak::instanceCount = 0;

	QNEWOBJ(a, TestObjectLeak())
	QNEWOBJ(b, TestObjectLeak())
	QNEWOBJ(c, TestObjectLeak())
	QNEWOBJ(d, TestObjectLeak())
	QNEWOBJ(e, TestObjectLeak())

	GCHECK(TestObjectLeak::instanceCount > 0);

	if(context->isLua()) {
		QDO(a = nil)
		QDO(b = nil)
		QDO(c = nil)
		QDO(d = nil)
		QDO(e = nil)
		QDO(collectgarbage("collect"))
		QDO(collectgarbage("collect"))
		GCHECK(TestObjectLeak::instanceCount == 0);
	}

	if(context->isPython()) {
		QDO(a = None)
		QDO(b = None)
		QDO(c = None)
		QDO(d = None)
		QDO(e = None)
		GCHECK(TestObjectLeak::instanceCount == 0);
	}
}

#define CASE testObjectLeak
#include "../bind_testcase.h"



}
