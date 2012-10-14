#include "testmetagen.h"
#include "../include/testobjectarray.h"
#include "../metadata/include/meta_test_testobjectarray.h"


using namespace meta_test;
using namespace std;

namespace {


void testObjectArray(TestScriptContext * context)
{
	QDO(oa = cpgf.createObjectArray(mtest.OAObject));
	QNEWOBJ(obj, mtest.OAObject(98, "def"));
	QASSERT(obj.data.n == 98);
	QDO(oa.setItem(0, obj));
	QASSERT(oa.getItem(0).data.n == 98);
}

#define CASE testObjectArray
#include "do_testcase.h"



}
