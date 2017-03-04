#include "cpgf/scriptbind/gluabind.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

#include "../benchmark.h"
#include "luabind_common.h"

namespace {

using namespace cpgf;

struct TestObject
{
	TestObject() : x(0) {
	}

	int addX(const int delta) {
		this->x += delta;
		return this->x;
	}

	int x;
};

} //unnamed namespace

void doBenchmarkLuaBind()
{
	TestLuaContext context;
	GScopedInterface<IMetaClass> metaClass(context.getService()->findClassByName("TestObject"));
	context.getBinding()->setValue("TestObject", GScriptValue::fromClass(metaClass.get()));

	std::string code = R"(
		a = TestObject()
		for i = 1, 1000000 do
			a.addX(5)
		end
	)";

	// 1000000
	// Before optimizing: 6900 ms
	// After refactored doInvokeMethodList: 6200 ms
	{
		BenchmarkTimer timer;
		context.doString(code.c_str());
	}
}

G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<TestObject>
		::define("TestObject")

		._constructor<void * ()>()
		._method("addX", &TestObject::addX)
		._field("x", &TestObject::x)
	;
}

