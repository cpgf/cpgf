#include "testmetagen.h"
#include "../include/testmetamethod.h"


using namespace std;

namespace {


void testMetaMethod(TestScriptContext * context)
{
	if(context->isPython()) {
		QDO(null = None);
	}
	
	QNEWOBJ(obj, mtest.TestMethodMethodClass());
	QASSERT(obj.func_null_38_abc(null, 38, "abc"));
	QASSERT(mtest.TestMethodMethodClass.sfunc_dEf_neg99("dEf", -99));
	QASSERT(obj.sfunc_dEf_neg99("dEf", -99));
	QASSERT(obj.func_return_What() == "What");
	QASSERT(obj.func_return_ReferenceToString() == "referenceToString");
	QASSERT(obj.func_return_data().check());
}

#define CASE testMetaMethod
#include "do_testcase.h"



}
