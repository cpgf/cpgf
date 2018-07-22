#include "testscriptbindmetadata6.h"
#include "bind_common.h"
#include "cpgf/gmetadefine.h"

using namespace cpgf;
using namespace std;

namespace testscript {

int TestObjectLeak::instanceCount = 0;

TestObjectLeak::TestObjectLeak()
{
	++instanceCount;
}

TestObjectLeak::~TestObjectLeak()
{
	--instanceCount;
}

void TestScriptBindMetaData6()
{
	GDefineMetaClass<TestObjectLeak>
		::define("testscript::TestObjectLeak")
		._constructor<void * ()>()
	;
}

}

