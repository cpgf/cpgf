#include "testmetagen.h"


namespace {


void testGlobalMethod(TestScriptContext * context)
{
	QASSERT(mtest.hello("world") == "123world");
	QASSERT(mtest.getStdString(0) == "zero");
	QASSERT(mtest.getStdString(1) == "one");
}


#define CASE testGlobalMethod
#include "do_testcase.h"


void testGlobalEnum(TestScriptContext * context)
{
	QASSERT(mtest.GlobalEnum.globalEnumA == 5);
	QASSERT(mtest.GlobalEnum.globalEnumB == 8);
	QASSERT(mtest.globalEnumA == 5);
	QASSERT(mtest.globalEnumB == 8);

	QERR(mtest.GlobalEnum.globalEnumA = 5);
	QERR(mtest.GlobalEnum.globalEnumB = 8);
	QERR(mtest.globalEnumA = 5);
	QERR(mtest.globalEnumB = 8);
}


#define CASE testGlobalEnum
#include "do_testcase.h"


void testGlobalFields(TestScriptContext * context)
{
	QASSERT(mtest.globalInt == 38);
	QASSERT(mtest.globalString == "global");
	QASSERT(mtest.globalStdString == "globalStd");
}


#define CASE testGlobalFields
#include "do_testcase.h"


}
