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
	
	scriptSetValue(context.getBinding(), "TestData", GScriptValue::fromClass(metaClass.get()));

	GScopedPointer<TestData> data(new TestData);
	data->x = 10;
	data->name = "abc";

	scriptSetValue(context.getBinding(), "data", GScriptValue::fromObject(data.get(), metaClass.get(), false, GScriptInstanceCv::sicvNone));
	context.doString("assert(data.x == 10)");
	context.doString("assert(data.name == \"abc\")");

	context.doString("data.x = data.x + 1");
	testCheckAssert(data->x == 11);
	
	scriptSetValue(context.getBinding(), "data", GScriptValue::fromNull());
	context.doString("assert(data == nil)");
}


gTestCase(doTest);



}
