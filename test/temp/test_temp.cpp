#include "../unittestbase.h"
#include "luabind_common.h"

#include "cpgf/gmetaclass.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

#include <iostream>

namespace Test_Temp { namespace {

using namespace cpgf;
using namespace std;

class TestOperator
{
public:
	TestOperator() : value(0) {
	}
	
	TestOperator(int value) : value(value) {
	}
	
	TestOperator(const TestOperator & other) : value(other.value) {
	}

#define M(OP, RET) \
	RET operator OP (int other) const {	return this->value OP other; } \
	RET operator OP (const TestOperator & other) const { return this->value OP other.value; }
	
	M(+, TestOperator)
	M(-, TestOperator)
	M(*, TestOperator)
	M(/, TestOperator)
	M(%, TestOperator)

	M(==, bool)
	M(<, bool)
	M(<=, bool)

#undef M

	TestOperator operator - () const {
		return -this->value;
	}

	TestOperator operator () (const std::string & s, int n) const {
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

public:
	int value;
};

void TestScriptBindMetaData3()
{
	GDefineMetaClass<TestOperator>
		::define("testscript::TestOperator")

		._constructor<void * (const TestOperator &)>()
		._constructor<void * (int)>()
		
		._field("value", &TestOperator::value)
		
		._method("getVar", &TestOperator::getVar)
		
		._operator<TestOperator (const GMetaSelf)>(-mopHolder)
		._operator<TestOperator (const std::string &, int)>(mopHolder(mopHolder), GMetaPolicyCopyAllConstReference())
		._operator<int (const GMetaVariadicParam *)>(mopHolder(mopHolder))
#define M(OP, RET) \
		._operator<RET (const GMetaSelf, int)>(mopHolder OP mopHolder) \
		._operator<RET (const GMetaSelf, const TestOperator &)>(mopHolder OP mopHolder) \
//		._operator<RET (const GMetaSelf, const TestObject &)>(mopHolder OP mopHolder)
	
		M(+, TestOperator)
		M(-, TestOperator)
		M(*, TestOperator)
		M(/, TestOperator)
		M(%, TestOperator)

		M(==, bool)
		M(<, bool)
		M(<=, bool)

#undef M
	;
}


GTEST(TestTemp)
{
GVariant a(10);
GVariant b(fromVariant<GVariant>(a));
//cout << b.refData().typeData.vt << "  " << b.refData().valueInt << endl;
//return;

	TestLuaContext context;
	GScopedInterface<IMetaClass> metaClass(context.getService()->findClassByName("testscript::TestOperator"));
	context.getBinding()->setValue("TestOperator", GScriptValue::fromClass(metaClass.get()));

	context.doString("a = TestOperator(99)");
	context.doString("b = a.getVar()");
	context.doString("print(b)");
}



G_AUTO_RUN_BEFORE_MAIN()
{
	TestScriptBindMetaData3();
}



} }
