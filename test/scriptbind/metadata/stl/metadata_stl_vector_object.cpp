#include "metadata_stl_vector.h"

namespace {


template <typename Binding>
void test_stl_vector_object(Binding * binding, TestScriptContext * context)
{
	testStlVector_Helper<vector<StlTestData> >(binding, context, "vector_obj", GTestValueProvider_TestData(context));
}


#define CASE test_stl_vector_object
#include "metadata_stl_testcase.h"



}
