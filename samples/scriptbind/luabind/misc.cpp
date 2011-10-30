#include "cpgf/gtestutil.h"
#include "cpgf/scriptbind/gluabind.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/gmetaclass.h"

#include <iostream>
#include <string>

#if defined(_MSC_VER)
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif


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


GMETA_DEFINE_CLASS(TestObject::InnerClass, TestObjectInnerClass, "InnerClass") {
	using namespace cpgf;

	GMETA_METHOD(get);
}

GMETA_DEFINE_CLASS(TestObject, TestObject, "method::TestObject") {
	using namespace cpgf;

	GMETA_CLASS(InnerClass);
	
	reflectConstructor<void *(int)>();

	reflectEnum<TestObject::MyEnum>("MyEnum")
		("a", TestObject::a)
	;

	reflectMethod("newTestObject", &TestObject::newTestObject, GMetaPolicyTransferResultOwnership());
	reflectMethod("cloneTestObject", &TestObject::cloneTestObject);

	reflectMethod("getWidth", (int (TestObject::*)() const)&TestObject::getWidth);
	reflectMethod("getWidth", (TestObject::MyEnum (TestObject::*)(int) const)&TestObject::getWidth);
	reflectMethod("getWidth", (TestObject::MyEnum (TestObject::*)(int, int) const)&TestObject::getWidth);

	GMETA_METHOD(incWidth);
	GMETA_METHOD(calcData);
	
	GMETA_METHOD(getVoid);
	GMETA_METHOD(setVoid);
	
	GMETA_METHOD(sum);

	GMETA_FIELD(width);

	reflectOperator<bool (const GMetaSelf &, const TestObject &)>(mopHolder == mopHolder);
	reflectOperator<int (const GMetaSelf &, const TestObject &)>(mopHolder + mopHolder);
	reflectOperator<int (int, int)>(mopHolder(mopHolder));
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

GMETA_DEFINE_GLOBAL() {
	using namespace cpgf;

	reflectMethod("addNumber", (int (*)(int, int))&addNumber);
	reflectMethod("addNumber", (int (*)(int, int, int))&addNumber);

	GMETA_ENUM(GlobalEnum, geA, geB);
}


void doTest()
{
	lua_State * L = luaL_newstate();
	luaL_openlibs(L);

	GScopedInterface<IMetaService> service(createDefaultMetaService());
	testCheckAssert(service);

	GScopedInterface<IMetaClass> globalClass(metaGetGlobalMetaClass(service, 0));
	testCheckAssert(globalClass);

	GLuaScriptObject binding(service.get(), L, GScriptConfig());

	GScopedPointer<GLuaScriptObject> scope(static_cast<GLuaScriptObject *>(binding.createScriptObject("myscope")));

	GScopedInterface<IMetaMethod> method;

	method.reset(globalClass->getMethod("addNumber")); testCheckAssert(method);

	scope->bindMethod("addNumber", NULL, method.get());

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("method::TestObject"));
	testCheckAssert(metaClass);
	
	binding.bindClass("TestObject", metaClass.get());
	
	GScopedInterface<IMetaEnum> metaEnum(globalClass->getEnum("GlobalEnum"));
	binding.bindEnum(metaEnum->getName(), metaEnum.get());

	binding.bindString("ONE", "This is one");
	scope->bindString("TWO", "Second one");

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

//	luaL_dostring(L, code);
	luaL_loadstring(L, code); lua_call(L, 0, LUA_MULTRET);

	GMetaVariant result = invokeScriptFunction(&binding, "luaAdd", 8, 2);
	cout << "Result: " << fromVariant<int>(result.getValue()) << endl;
	cout << binding.getString("lss") << endl;
	{
	GScriptName nameNewObj("newObj");
	binding.cacheName(&nameNewObj);
	cout << static_cast<TestObject *>(binding.getObject(nameNewObj))->width << endl;
	IMetaTypedItem * item = NULL;
	binding.getType(nameNewObj, &item);
	GScopedInterface<IMetaClass> type(static_cast<IMetaClass *>(item));
	cout << type->getName() << endl;
	}

	GScriptName nameNewLss("newlss");
	binding.cacheName(&nameNewLss);
	binding.assignValue("lss", nameNewLss);
	binding.nullifyValue(nameNewLss);
	luaL_loadstring(L, "print(newlss)"); lua_call(L, 0, LUA_MULTRET);
	nameNewLss.uncache();
	
	lua_close(L);
}

}


gTestCase(doTest);

