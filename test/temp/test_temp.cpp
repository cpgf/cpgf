#include "../unittestbase.h"
#include "luabind_common.h"

#include "cpgf/gmetaclass.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

#include <iostream>
#include <memory>

namespace Test_Temp { namespace {

using namespace cpgf;
using namespace std;

class TestObject
{
public:
	TestObject() : value(0) {
	}
	
	TestObject(int value) : value(value) {
	}
	
	TestObject(const TestObject & other) : value(other.value) {
	}

#define M(OP, RET) \
	RET operator OP (int other) const {	return this->value OP other; } \
	RET operator OP (const TestObject & other) const { return this->value OP other.value; }
	
	M(+, TestObject)
	M(-, TestObject)
	M(*, TestObject)
	M(/, TestObject)
	M(%, TestObject)

	M(==, bool)
	M(<, bool)
	M(<=, bool)

#undef M

	TestObject operator - () const {
		return -this->value;
	}

	TestObject operator () (const std::string & s, int n) const {
		return this->value + (int)s.length() + n * 2;
	}

	int operator () (const cpgf::GMetaVariadicParam * params) const {
		int total = this->value;
		for(size_t i = 0; i < params->paramCount; ++i) {
			total += cpgf::fromVariant<int>(*(params->params[i]));
		}

		return total;
	}
	
	GVariant getVar() const { return 38; }
	
	void testParam(int * & param) const {
		param = nullptr;
	}

	int testCallback(const GCallback<int (int a, int b)> & cb) {
		return cb(3, 8);
	}

public:
	int value;
};

void TestScriptBindMetaData3()
{
	GDefineMetaClass<TestObject>
		::define("testscript::TestObject")

		._constructor<void * (const TestObject &)>()
		._constructor<void * (int)>()
		
		._field("value", &TestObject::value)
		
		._method("getVar", &TestObject::getVar)
		._method("testParam", &TestObject::testParam)
//		._method("testCallback", &TestObject::testCallback)

		._operator<TestObject (const GMetaSelf)>(-mopHolder)
		._operator<TestObject (const std::string &, int)>(mopHolder(mopHolder), GMetaPolicyCopyAllConstReference())
		._operator<int (const GMetaVariadicParam *)>(mopHolder(mopHolder))
#define M(OP, RET) \
		._operator<RET (const GMetaSelf, int)>(mopHolder OP mopHolder) \
		._operator<RET (const GMetaSelf, const TestObject &)>(mopHolder OP mopHolder) \
//		._operator<RET (const GMetaSelf, const TestObject &)>(mopHolder OP mopHolder)
	
		M(+, TestObject)
		M(-, TestObject)
		M(*, TestObject)
		M(/, TestObject)
		M(%, TestObject)

		M(==, bool)
		M(<, bool)
		M(<=, bool)

#undef M
	;
}

typedef int (*&FuncPtr)();

int doIt(const std::shared_ptr<TestObject> & obj)
{
	cout << obj->value << endl;
	return obj->value;
}

GTEST(TestTemp)
{
	void * p = new TestObject(38);
	GVariant v(p);
//	doIt(fromVariant<std::shared_ptr<TestObject> >(v));
return;

	TestLuaContext context;
	GScopedInterface<IMetaClass> metaClass(context.getService()->findClassByName("testscript::TestObject"));
	context.getBinding()->setValue("TestObject", GScriptValue::fromClass(metaClass.get()));

	context.doString("a = TestObject(99)");
	context.doString("b = a.getVar()");
	context.doString("print(b)");
}



G_AUTO_RUN_BEFORE_MAIN()
{
	TestScriptBindMetaData3();
}



} }
