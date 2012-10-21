#include "testmetagen.h"


using namespace std;

namespace {


void testInterfaceObject(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.MyInterfaceObject());
	QNEWOBJ(b, mtest.MyInterfaceHolder(a));
}


#define CASE testInterfaceObject
#include "do_testcase.h"



}
