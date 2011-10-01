#include "test_lua_common.h"


namespace {


GTEST(global)
{
	using namespace cpgf;
	using namespace testscript;

	TestLuaContext * context = getLuaContext();

	GApiScopedPointer<IMetaClass> metaClass(context->getService()->findClassByName("testscript::TestData"));
	GCHECK(metaClass);
	
	GScopedPointer<TestData> data(new TestData);
	data->x = 10;
	data->name = "abc";

	context->getBinding()->setObject("data", data.get(), metaClass.get(), false);
	context->doString("assert(data.x == 10)");
	context->doString("assert(data.name == \"abc\")");

	context->doString("data.x = data.x + 1");
	GEQUAL(data->x, 11);
	
	context->getBinding()->nullifyValue("data");
	context->doString("assert(data == nil)");
}




}
