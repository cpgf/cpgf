#include "testscriptbindmetadata3.h"
#include "cpgf/gmetadefine.h"

using namespace cpgf;
using namespace std;

namespace testscript {

void TestScriptBindMetaData3()
{
	GDefineMetaClass<TestOperator>
		::define("testscript::TestOperator")

		._constructor<void * (const TestOperator &)>()
		._constructor<void * (int)>()
		
		._field("value", &TestOperator::value)
		
		._operator<TestOperator (const GMetaSelf)>(-mopHolder)
		._operator<TestOperator (const std::string &, int)>(mopHolder(mopHolder), GMetaPolicyCopyAllConstReference())
		._operator<int (const GMetaVariadicParam *)>(mopHolder(mopHolder))
#define M(OP, RET) \
		._operator<RET (const GMetaSelf, int)>(mopHolder OP mopHolder) \
		._operator<RET (const GMetaSelf, const TestOperator &)>(mopHolder OP mopHolder) \
		._operator<RET (const GMetaSelf, const TestObject &)>(mopHolder OP mopHolder)
	
		M(+, TestOperator)
		M(-, TestOperator)
		M(*, TestOperator)
		M(/, TestOperator)
		M(%, TestOperator)

		M(==, bool)
		M(<, bool)
		M(<=, bool)

#undef M
	;
}



}

