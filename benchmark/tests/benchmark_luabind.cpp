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
	
	int add5(const int a, const float b, const double c, const unsigned int d, const long long e)
	{
		this->x += (int)a + (int)b + (int)c + (int)d + (int)e;
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

	// 1000000
	// Before optimizing: 6900 ms
	// After refactored doInvokeMethodList: 6200 ms
	// After changed sourceClass and userConverter in ConvertRank, 5750 ms.
	// After changed the object array to char buffer in InvokeCallableParam, 4750 ms.
	// After changed holders from GVariant holders[REF_MAX_ARITY] to GVariant holder in doInvokeCallable, 4150 ms
	// After used VariantParameterBuffer in gmetaapi.cpp, 4050 ms
	// After changed map to unordered_map in class GMetaMapClass, 3900 ms
	// After changed compile options to  -O2 -Oy -GL, link options to -LTCG, in VC, 3200 ms
	// 2850 ms
	// 2650 ms
	{
		std::string code = R"(
			a = TestObject()
			for i = 1, 1000000 do
				a.addX(5)
			end
		)";

		BenchmarkTimer timer("Script binding");
		context.doString(code.c_str());
	}

	// 80 ms
	{
		std::string code = R"(
			x = 0
			function myAddX(a)
				x = x + a
			end
			for i = 1, 1000000 do
				myAddX(5)
			end
		)";

		BenchmarkTimer timer("Pure Lua");
		context.doString(code.c_str());
	}

	// 3650 ms
	{
		std::string code = R"(
			a = TestObject()
			for i = 1, 1000000 do
				a.add5(5.0, 6.0, 7, 8, 9)
			end
		)";

		BenchmarkTimer timer("Script binding 5 parameters");
		context.doString(code.c_str());
	}

	{
		std::string code = R"(
			x = 0
			function myAdd5(a, b, c, d, e)
				x = x + a + b + c + d + e
			end
			for i = 1, 1000000 do
				myAdd5(5.0, 6.0, 7, 8, 9)
			end
		)";

		BenchmarkTimer timer("Pure Lua 5 parameters");
		context.doString(code.c_str());
	}

	//  ms
	{
		std::string code = R"(
			for i = 1, 1000000 do
				math.abs(5)
			end
		)";

		BenchmarkTimer timer("Lua abs");
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
		._method("add5", &TestObject::add5)
		._field("x", &TestObject::x)
	;
}

