#include "metadata_stl_vector.h"

namespace {


template <typename Binding>
void test_stl_vector_int(Binding * binding, TestScriptContext * context)
{
	testStlVector_Helper<vector<int> >(binding, context, "vector_int", GTestValueProvider_Int(context));
}


#define CASE test_stl_vector_int
#include "metadata_stl_testcase.h"



}
