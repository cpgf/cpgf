#include "metadata_stl_list.h"


namespace {


template <typename Binding>
void test_stl_list_int(Binding * binding, TestScriptContext * context)
{
	testStlList_Helper<list<int> >(binding, context, "list_int", GTestValueProvider_Int(context));
}


#define CASE test_stl_list_int
#include "metadata_stl_testcase.h"



}
