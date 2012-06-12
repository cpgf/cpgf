#include "../testscriptbind.h"

namespace {

void testByteArrayInScript(TestScriptContext * context)
{
	QDO(a = createByteArray())
	QDO(a.setLength(10))

	QDO(writeNumberToByteArray(38, a))
	QASSERT(a.getPosition() == 8)
	QDO(a.setPosition(0))
	QASSERT(a.readInt32() == 38)
	QASSERT(a.readInt32() == 38 * 2)

	QDO(writeNumberToByteArrayMemory(53, a.getMemory()))
	QDO(a.position = 0)
	QASSERT(a.readInt32() == 53)
	QASSERT(a.readInt32() == 53 * 2)
}

#define CASE testByteArrayInScript
#include "../bind_testcase.h"



}
