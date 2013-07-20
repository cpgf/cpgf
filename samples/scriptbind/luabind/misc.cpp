#include "cpgf/metatraits/gmetaconverter_string.h"

#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

#include "cpgf/gtestutil.h"
#include "cpgf/scriptbind/gluabind.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/gscopedinterface.h"

#include <iostream>
#include <string>


using namespace std;
using namespace cpgf;

namespace {

struct TestData {
	int x;
	string tag;
};

class TestObject
{
public:
	enum MyEnum {
		a = 1
	};

	class InnerClass {
	public:
		string get() const {
			return "This is inner class";
		}
	};

public:
	TestObject()
		:	width(10),
			name("what's the name"),
			data()
	{
		this->created();
	}
	
	explicit TestObject(int width)
		:	width(width),
			name("what's the name"),
			data()
	{
		this->created();
	}

	TestObject(const TestObject & other)
		:	width(other.width),
			name(other.name),
			data(other.data)
	{
		this->created();
	}

	void created() {
		if(this->width > 0) {
			this->width = this->width;
		}
	}

	~TestObject() {
		if(this->width > 0) {
			this->width = this->width;
		}
	}

	TestObject & operator = (const TestObject & other) {
		this->width = other.width;
		this->name = other.name;
		this->data = other.data;

		return *this;
	}

	bool operator == (const TestObject & other) const {
		(void)other;

		return true;
//		return this->width == other.width && this->name == other.name;
	}

	int operator + (const TestObject & other) const {
		return this->width + other.width;
	}

	int operator () (int a, int b) const {
		return this->width + a * b;
	}

	TestObject * newTestObject(int n) const {
		return new TestObject(n);
	}

	TestObject cloneTestObject(int n) const {
		return TestObject(n);
	}

	int getWidth() const {
		return this->width;
	}

	MyEnum getWidth(int a) const {
		
		return (MyEnum)(this->width + a);
	}

	MyEnum getWidth(int a, int b) const {
		
		return (MyEnum)(this->width + a + b);
	}

	string incWidth(int d, const std::string & s) {
		this->width += d + static_cast<int>(s.length());

		return s + " -- returned";
	}

	TestData G_API_CC calcData(int a, long b, const std::string & s) {
		TestData result(this->data);

		result.x += a + b;
		result.tag = result.tag + s;

		return result;
	}

	void * getVoid() {
		return (void *)0x38;
	}

	void  setVoid(void * p) {
		testCheckAssert(p == (void *)0x38);
	}

	int sum(const GMetaVariadicParam * params) {
		int total = 0;
		for(size_t i = 0; i < params->paramCount; ++i) {
			total += fromVariant<int>(*(params->params[i]));
		}

		return total;
	}

public:
	int width;
	std::string name;
	TestData data;
};


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<TestObject>
		::define("method::TestObject")

		._class(
			GDefineMetaClass<TestObject::InnerClass>
				::declare("InnerClass")
				._method("get", &TestObject::InnerClass::get)
		)

		._constructor<void *(int)>()

		._enum<TestObject::MyEnum>("MyEnum")
			._element("a", TestObject::a)

		._method("newTestObject", &TestObject::newTestObject, GMetaPolicyTransferResultOwnership())
		._method("cloneTestObject", &TestObject::cloneTestObject)

		._method("getWidth", (int (TestObject::*)() const)&TestObject::getWidth)
		._method("getWidth", (TestObject::MyEnum (TestObject::*)(int) const)&TestObject::getWidth)
		._method("getWidth", (TestObject::MyEnum (TestObject::*)(int, int) const)&TestObject::getWidth)

		._method("incWidth", &TestObject::incWidth, GMetaPolicyCopyAllConstReference())
		._method("calcData", &TestObject::calcData, GMetaPolicyCopyAllConstReference())
	
		._method("getVoid", &TestObject::getVoid)
		._method("setVoid", &TestObject::setVoid)
	
		._method("sum", &TestObject::sum)

		._field("width", &TestObject::width)

		._operator<bool (const GMetaSelf &, const TestObject &)>(mopHolder == mopHolder)
		._operator<int (const GMetaSelf &, const TestObject &)>(mopHolder + mopHolder)
		._operator<int (int, int)>(mopHolder(mopHolder))
	;
}

int addNumber(int n, int delta)
{
	return n + delta;
}

int addNumber(int n, int delta, int a)
{
	return n + delta + a;
}

enum GlobalEnum {
	geA = 5, geB = 6
};

G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaGlobal()
		._method("addNumber", (int (*)(int, int))&addNumber)
		._method("addNumber", (int (*)(int, int, int))&addNumber)
		._enum<GlobalEnum>("GlobalEnum")
			._element("geA", geA)
			._element("geB", geB)
	;
}


void doTest()
{
	lua_State * L = luaL_newstate();
	luaL_openlibs(L);

	GScopedInterface<IMetaService> service(createDefaultMetaService());
	testCheckAssert(service);

	GScopedInterface<IMetaClass> globalClass(metaGetGlobalMetaClass(service.get(), 0));
	testCheckAssert(globalClass);

	GScopedPointer<GScriptObject> binding(createLuaScriptObject(service.get(), L, GScriptConfig()));

	GScopedInterface<IScriptObject> scope(binding->createScriptObject("myscope").toScriptObject());

	GScopedInterface<IMetaMethod> method;

	method.reset(globalClass->getMethod("addNumber")); testCheckAssert(method);

	scriptSetValue(scope.get(), "addNumber", GScriptValue::fromMethod(NULL, method.get()));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("method::TestObject"));
	testCheckAssert(metaClass);
	
	scriptSetValue(binding.get(), "TestObject", GScriptValue::fromClass(metaClass.get()));
	
	GScopedInterface<IMetaEnum> metaEnum(globalClass->getEnum("GlobalEnum"));
	scriptSetValue(binding.get(), metaEnum->getName(), GScriptValue::fromEnum(metaEnum.get()));

	scriptSetValue(binding.get(), "ONE", GScriptValue::fromString("This is one"));
	scriptSetValue(scope.get(), "TWO", GScriptValue::fromString("Second one"));

	const char * code =
//		"for k,v in pairs(getmetatable(TestObject)) do print(k,v) end \n"
		"function luaAdd(a, b) return a + b end \n"
		"print(\"DoString\") \n"
		"print(myscope.addNumber(10, 12)) \n"
		"obj = TestObject(5) \n"
		"print(obj.getWidth()) \n"
		"print(obj.getWidth(3)) \n"
		"print(obj.getWidth(3, 5)) \n"
		"newObj = obj.cloneTestObject(18) \n"
		"print(newObj.getWidth(3)) \n"
		"print(newObj) \n"
		"newObj.width = newObj.width + 5 \n"
		"print(newObj.getWidth()) \n"
		"print(getmetatable(obj)) \n"
		"print(getmetatable(newObj)) \n"
		"print(obj == newObj) \n"
		"print(obj + newObj) \n"
		"print(obj(2, 3)) \n"
		"print(obj.width) \n"
		"print(obj.incWidth(3, \"ab\")) \n"
		"print(obj.getWidth()) \n"
		"print(TestObject.InnerClass().get()) \n"
		"print(obj.a) \n"
		"assert(obj.a == TestObject.MyEnum.a) \n"
		"print(GlobalEnum.geA) \n"
		"print(ONE) \n"
		"print(myscope.TWO) \n"
		"print(newObj.width) \n"
		"lss = \"A string in L\" \n"
		"print(newObj.getVoid()) \n"
		"obj.setVoid(obj.getVoid()) \n"
		"assert(obj.sum(1, 3, 5) == 1 + 3+ 5) \n"

	;

	luaL_loadstring(L, code); lua_call(L, 0, LUA_MULTRET);

	GVariant result = invokeScriptFunction(binding.get(), "luaAdd", 8, 2);
	cout << "Result: " << fromVariant<int>(result) << endl;
	cout << scriptGetValue(binding.get(), "lss").toString() << endl;
	{
		cout << static_cast<TestObject *>(scriptGetValue(binding.get(), "newObj").toObjectAddress(NULL, NULL))->width << endl;
	IMetaClass * item = NULL;
	scriptGetValue(binding.get(), "newObj").toObject(&item, NULL);
	GScopedInterface<IMetaClass> type(item);
	cout << type->getName() << endl;
	}

	binding->assignValue("lss", "newlss");
	binding->setValue("newlss", GScriptValue::fromNull());
	luaL_loadstring(L, "print(newlss)"); lua_call(L, 0, LUA_MULTRET);
	
	lua_close(L);
}

}


gTestCase(doTest);

