#include "metadata_stl_deque.h"


namespace {


template <typename Binding>
void test_stl_deque_int(Binding * binding, TestScriptContext * context)
{
	testStlDeque_Helper<deque<int> >(binding, context, "deque_int", GTestValueProvider_Int(context));
}


#define CASE test_stl_deque_int
#include "metadata_stl_testcase.h"



}
