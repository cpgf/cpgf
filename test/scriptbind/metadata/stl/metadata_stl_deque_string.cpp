#include "metadata_stl_deque.h"


namespace {


template <typename Binding>
void test_stl_deque_string(Binding * binding, TestScriptContext * context)
{
	testStlDeque_Helper<deque<string> >(binding, context, "deque_string", GTestValueProvider_String(context));
}


#define CASE test_stl_deque_string
#include "metadata_stl_testcase.h"



}
