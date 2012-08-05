#include "testmetagen.h"


namespace {


void testMisc(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.SimpleData());
	QASSERT(a.n == 0);

	QDO(scriptTrace("Olay olay"));

	QNEWOBJ(a, mtest.SimpleObject());
	QDO(a.data.n = 38);
	QDO(a.data.s = "abc");
	QDO(a.data.atom.value = 58);
	QASSERT(a.checkData(38));
	QASSERT(a.checkData("abc"));
	QASSERT(a.checkAtom(58));

	QASSERT(mtest.GlobalEnum.globalEnumA == 5);
	QASSERT(mtest.GlobalEnum.globalEnumB == 8);
	QASSERT(mtest.globalEnumA == 5);
	QASSERT(mtest.globalEnumB == 8);
}


#define CASE testMisc
#include "do_testcase.h"


}
