#include "luabind_common.h"


namespace {


void doTest()
{
	using namespace cpgf;
	using namespace testscript;

	TestLuaContext context;
	context.doString("print(\"Global\")");

	GScopedInterface<IMetaClass> metaClass(context.getService()->findClassByName("testscript::TestData"));
	testCheckAssert(metaClass);
	
	context.getBinding()->bindClass("TestData", metaClass.get());

	GScopedPointer<TestData> data(new TestData);
	data->x = 10;
	data->name = "abc";

	context.getBinding()->bindObject("data", data.get(), metaClass.get(), false);
	context.doString("assert(data.x == 10)");
	context.doString("assert(data.name == \"abc\")");

	context.doString("data.x = data.x + 1");
	testCheckAssert(data->x == 11);
	
	context.getBinding()->nullifyValue("data");
	context.doString("assert(data == nil)");
}


gTestCase(doTest);



}
