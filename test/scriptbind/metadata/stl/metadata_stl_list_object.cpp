#include "metadata_stl_list.h"


namespace {


template <typename Binding>
void test_stl_list_object(Binding * binding, TestScriptContext * context)
{
	testStlList_Helper<list<StlTestData> >(binding, context, "list_obj", GTestValueProvider_TestData(context));
}


#define CASE test_stl_list_object
#include "metadata_stl_testcase.h"



}
