#include "../testscriptbind.h"


namespace {


void TestLuaOperator(TestScriptContext * context)
{
	QDO(a = TestOperator(99))
	QDO(b = a + 5)
	QASSERT(b.value == 99 + 5)

	QDO(b = a + TestOperator(8))
	QASSERT(b.value == 99 + 8)

	QDO(b = a + TestObject(15))
	QASSERT(b.value == 99 + 15)


	QDO(a = TestOperator(98))
	QDO(b = a - 5)
	QASSERT(b.value == 98 - 5)

	QDO(b = a - TestOperator(8))
	QASSERT(b.value == 98 - 8)

	QDO(b = a - TestObject(15))
	QASSERT(b.value == 98 - 15)


	QDO(a = TestOperator(97))
	QDO(b = a * 5)
	QASSERT(b.value == 97 * 5)

	QDO(b = a * TestOperator(8))
	QASSERT(b.value == 97 * 8)

	QDO(b = a * TestObject(15))
	QASSERT(b.value == 97 * 15)


	QDO(a = TestOperator(99))
	QDO(b = a / 5)
	QASSERT(b.value == math.floor(99 / 5))

	QDO(b = a / TestOperator(8))
	QASSERT(b.value == math.floor(99 / 8))

	QDO(b = a / TestObject(15))
	QASSERT(b.value == math.floor(99 / 15))


	QDO(a = TestOperator(88))
	QDO(b = a % 5)
	QASSERT(b.value == 88 % 5)

	QDO(b = a % TestOperator(8))
	QASSERT(b.value == 88 % 8)

	QDO(b = a % TestObject(15))
	QASSERT(b.value == 88 % 15)


	QDO(a = TestOperator(99))
	QDO(b = -a)
	QASSERT(b.value == -99)

	QDO(c = TestOperator(9))
	QDO(a = c)
	QDO(b = -a)
	QASSERT(b.value == -9)


	QDO(d = TestOperator(3))
	QDO(b = d(5, 7, 9, 1, 2, 6, 8))
	QASSERT(b == 3 + 5 + 7 + 9 + 1 + 2 + 6 + 8)
}

#define CASE TestLuaOperator
#include "../testcase_lua.h"



}
