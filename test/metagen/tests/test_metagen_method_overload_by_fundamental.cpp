#include "testmetagen.h"


namespace {


void metagenTest_MetagenMethodOverloadByFundamental_overload_Boolean_Int(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenMethodOverloadByFundamental());
	if(context->isPython()) {
		QDO(t = True);
		QDO(f = False);
	}
	else {
		QDO(t = true);
		QDO(f = false);
	}
	QASSERT(a.overload_Boolean_Int(t) == "true");
	QASSERT(a.overload_Boolean_Int(f) == "false");
	QASSERT(a.overload_Boolean_Int(1) == "int");
	QASSERT(a.overload_Boolean_Int(123456) == "int");
	QASSERT(a.overload_Boolean_Int(0.5) == "int");
	QASSERT(a.overload_Boolean_Int(3.1415926) == "int");
}

#define CASE metagenTest_MetagenMethodOverloadByFundamental_overload_Boolean_Int
#include "do_testcase.h"


void metagenTest_MetagenMethodOverloadByFundamental_overload_Boolean_Real(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MetagenMethodOverloadByFundamental());
	if(context->isPython()) {
		QDO(t = True);
		QDO(f = False);
	}
	else {
		QDO(t = true);
		QDO(f = false);
	}
	QASSERT(a.overload_Boolean_Real(t) == "true");
	QASSERT(a.overload_Boolean_Real(f) == "false");
	QASSERT(a.overload_Boolean_Real(1) == "real");
	QASSERT(a.overload_Boolean_Real(123456) == "real");
	QASSERT(a.overload_Boolean_Real(0.5) == "real");
	QASSERT(a.overload_Boolean_Real(3.1415926) == "real");
}

#define CASE metagenTest_MetagenMethodOverloadByFundamental_overload_Boolean_Real
#include "do_testcase.h"


void metagenTest_MetagenMethodOverloadByFundamental_overload_Boolean_Int_Real(TestScriptContext * context)
{
	if(context->isLua()) {
		// Lua doesn't pass this test since Lua doesn't distinguish between integer and real number
		return;
	}

	QNEWOBJ(a, mtest.MetagenMethodOverloadByFundamental());
	if(context->isPython()) {
		QDO(t = True);
		QDO(f = False);
	}
	else {
		QDO(t = true);
		QDO(f = false);
	}
	QASSERT(a.overload_Boolean_Int_Real(t) == "true");
	QASSERT(a.overload_Boolean_Int_Real(f) == "false");
	QASSERT(a.overload_Boolean_Int_Real(1) == "int");
	QASSERT(a.overload_Boolean_Int_Real(123456) == "int");
	QASSERT(a.overload_Boolean_Int_Real(0.5) == "real");
	QASSERT(a.overload_Boolean_Int_Real(3.1415926) == "real");
}

#define CASE metagenTest_MetagenMethodOverloadByFundamental_overload_Boolean_Int_Real
#include "do_testcase.h"


void metagenTest_MetagenMethodOverloadByFundamental_global_overload_Boolean_Int(TestScriptContext * context)
{
	if (context->isPython()) {
		QDO(t = True);
		QDO(f = False);
	}
	else {
		QDO(t = true);
		QDO(f = false);
	}
	QASSERT(mtest.global_overload_Boolean_Int(t) == "true");
	QASSERT(mtest.global_overload_Boolean_Int(f) == "false");
	QASSERT(mtest.global_overload_Boolean_Int(1) == "int");
	QASSERT(mtest.global_overload_Boolean_Int(123456) == "int");
	QASSERT(mtest.global_overload_Boolean_Int(0.5) == "int");
	QASSERT(mtest.global_overload_Boolean_Int(3.1415926) == "int");
}

#define CASE metagenTest_MetagenMethodOverloadByFundamental_global_overload_Boolean_Int
#include "do_testcase.h"



}
