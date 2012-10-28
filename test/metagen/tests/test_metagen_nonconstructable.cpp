#include "testmetagen.h"


namespace {


void metagenTest_nonconstructable(TestScriptContext * context)
{
	ERR_QNEWOBJ(obj, mtest.MetagenNonconstructable());
}

#define CASE metagenTest_nonconstructable
#include "do_testcase.h"



}
