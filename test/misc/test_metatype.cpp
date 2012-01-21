#include "test_misc_common.h"
#include "cpgf/gmetatype.h"


using namespace std;
using namespace cpgf;

namespace Test_Metatype { namespace {

struct TestType {};

GTEST(TestMetaType)
{
	GCHECK(createMetaType<int>() == createMetaType<const int &>());
	GCHECK(createMetaType<int>() != createMetaType<const int *>());
	GCHECK(createMetaType<int>() != createMetaType<long>());

	GCHECK(! createMetaType<int>().isPointer());
	GCHECK(createMetaType<int *>().isPointer());
	GCHECK(! createMetaType<int *>().isPointerToConst());
	GCHECK(createMetaType<const int *>().isPointerToConst());

	GCHECK(! createMetaType<int (TestType::*)()>().isConstFunction());
	GCHECK(createMetaType<int (TestType::*)() const>().isConstFunction());
}




} }
