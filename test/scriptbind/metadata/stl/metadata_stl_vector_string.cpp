#include "metadata_stl_vector.h"

namespace {


template <typename Binding>
void test_stl_vector_string(Binding * binding, TestScriptContext * context)
{
	testStlVector_Helper<vector<string> >(binding, context, "vector_string", GTestValueProvider_String(context));
}


#define CASE test_stl_vector_string
#include "metadata_stl_testcase.h"



}
