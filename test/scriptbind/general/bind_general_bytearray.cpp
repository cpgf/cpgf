#include "../bind_common.h"

namespace {

void testByteArrayInScript(TestScriptContext * context)
{
if(context->isV8()) return;
	QDO(a = createByteArray())
	QDO(a.setLength(10))
	QDO(writeNumberToByteArray(38, a))
	QASSERT(a.getPosition() == 4)
}

#define CASE testByteArrayInScript
#include "../bind_testcase.h"



}
