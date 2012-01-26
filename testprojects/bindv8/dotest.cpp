#include "cpgf/metadata/stl/gmetadata_vector.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"
#include "../../test/scriptbind/bind_common.h"

#include <vector>
#include <iostream>
#include <sstream>

using namespace cpgf;
using namespace std;

typedef vector<int> VectorType;


template <typename T>
void doTestBind(T * binding, TestScriptContext * c)
{
}

void doTest(TestScriptContext * context)
{
	IScriptObject * bindingApi = context->getBindingApi();
	GScriptObject * bindingLib = context->getBindingLib();

	if(bindingLib) {
		doTestBind(bindingLib, context);
	}
	else	
	if(bindingApi) {
		doTestBind(bindingApi, context);
	}
}

void testOpenGL();

void doTest()
{
testOpenGL();

	doTest(createTestScriptContext(tslLua, tsaLib));
	doTest(createTestScriptContext(tslV8, tsaLib));
}

