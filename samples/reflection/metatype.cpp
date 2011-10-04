#include "reflect_common.h"



namespace {

struct Test {};

void doTest()
{
	using namespace cpgf;

	testCheckAssert(createMetaType<int>() == createMetaType<const int &>());
	testCheckAssert(createMetaType<int>() != createMetaType<const int *>());
	testCheckAssert(createMetaType<int>() != createMetaType<long>());

	testCheckAssert(! createMetaType<int>().isPointer());
	testCheckAssert(createMetaType<int *>().isPointer());
	testCheckAssert(! createMetaType<int *>().isPointerToConst());
	testCheckAssert(createMetaType<const int *>().isPointerToConst());

	testCheckAssert(! createMetaType<int (Test::*)()>().isConstFunction());
	testCheckAssert(createMetaType<int (Test::*)() const>().isConstFunction());
}


gTestCase(doTest);


}
