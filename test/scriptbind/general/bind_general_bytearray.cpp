#include "../testscriptbind.h"

namespace {

void testByteArrayInScript(TestScriptContext * context)
{
	QDO(cpgf._import("cpgf", "builtin.collections.bytearray"))
	
	QDO(a = cpgf.createByteArray())
	QDO(a.setLength(10))

	QDO(writeNumberToByteArray(38, a))
	QASSERT(a.getPosition() == 8)
	QDO(a.setPosition(0))
	QASSERT(a.readInt32() == 38)
	QASSERT(a.readInt32() == 38 * 2)

	QDO(writeNumberToByteArrayMemory(53, a.getPointer()))
	QDO(a.position = 0)
	QASSERT(a.readInt32() == 53)
	QASSERT(a.readInt32() == 53 * 2)
}

#define CASE testByteArrayInScript
#include "../bind_testcase.h"


void testByteArrayInScriptWriteVariadic(TestScriptContext * context)
{
	QDO(cpgf._import("cpgf", "builtin.collections.bytearray"))
	
	QDO(a = cpgf.createByteArray())
	QDO(a.writeInt32(38, 78, 98))
	QASSERT(a.getPosition() == 12)
	QDO(a.setPosition(0))
	QASSERT(a.readInt32() == 38)
	QASSERT(a.readInt32() == 78)
	QASSERT(a.readInt32() == 98)
}

#define CASE testByteArrayInScriptWriteVariadic
#include "../bind_testcase.h"



}
