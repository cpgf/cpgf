#include "metadata_stl_list.h"


namespace {


template <typename Binding>
void test_stl_list_string(Binding * binding, TestScriptContext * context)
{
	testStlList_Helper<list<string> >(binding, context, "list_string", GTestValueProvider_String(context));
}


#define CASE test_stl_list_string
#include "metadata_stl_testcase.h"



}
