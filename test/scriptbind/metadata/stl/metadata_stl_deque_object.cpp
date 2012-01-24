#include "metadata_stl_deque.h"


namespace {


template <typename Binding>
void test_stl_deque_object(Binding * binding, TestScriptContext * context)
{
	testStlDeque_Helper<deque<StlTestData> >(binding, context, "deque_obj", GTestValueProvider_TestData(context));
}


#define CASE test_stl_deque_object
#include "metadata_stl_testcase.h"


}
