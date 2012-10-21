#include "testmetagen.h"


using namespace std;

namespace {


void testObjectNoDtor(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.ObjectNoDestructor());
	QDO(mtest.ObjectNoDestructor.free(a));
}


#define CASE testObjectNoDtor
#include "do_testcase.h"



}
