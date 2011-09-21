#ifndef __TESTVARIANT_H
#define __TESTVARIANT_H

#include "cpgf/gvariant.h"
#include "cpgf/gtestutil.h"

#include <string>
#include <iostream>

using namespace cpgf;
using namespace std;


namespace {

class TestVariantObject
{
public:
	TestVariantObject() : unused(10000), value(1999) {
	}

	TestVariantObject(const TestVariantObject & other) : unused(other.unused), value(other.value) {
	}

	TestVariantObject & operator = (const TestVariantObject & other) {
		if(this != &other) {
			this->unused = other.unused;
			this->value = other.value;
		}

		return *this;
	}

	virtual string getName() const {
		return "none";
	}

public:
	long unused;
	int value;
};

class TestVariantDerived : public TestVariantObject
{
private:
	typedef TestVariantObject super;

public:
	TestVariantDerived() : super(), str("Derived") {
	}

	virtual string getName() const {
		return this->str;
	}

public:
	string str;
};


void doTestVariant_Convert()
{
	GVariant var;

	{
		TestVariantDerived derObj = TestVariantDerived();
//		const TestVariantDerived & ooo = TestVariantDerived();
		var = TestVariantDerived();
		var = derObj;
		TestVariantObject & obj = fromVariant<TestVariantObject &>(var);
		string s = obj.getName();
		TestVariantDerived der = fromVariant<TestVariantDerived>(var);
//		int n = fromVariant<int>(var);
//		string s = fromVariant<string>(var);
	}
}

void doTestVariant_CanCast_Primary(const GVariant & var)
{
	testCheckAssert(canFromVariant<bool>(var));
	testCheckAssert(canFromVariant<wchar_t>(var));
	testCheckAssert(canFromVariant<char>(var));
	testCheckAssert(canFromVariant<signed char>(var));
	testCheckAssert(canFromVariant<unsigned char>(var));
	testCheckAssert(canFromVariant<short>(var));
	testCheckAssert(canFromVariant<unsigned short>(var));
	testCheckAssert(canFromVariant<int>(var));
	testCheckAssert(canFromVariant<unsigned int>(var));
	testCheckAssert(canFromVariant<long>(var));
	testCheckAssert(canFromVariant<unsigned long>(var));
	testCheckAssert(canFromVariant<long long>(var));
	testCheckAssert(canFromVariant<unsigned long long>(var));
	testCheckAssert(canFromVariant<float>(var));
	testCheckAssert(canFromVariant<double>(var));
	testCheckAssert(canFromVariant<long double>(var));
	
	testCheckAssert(! canFromVariant<void *>(var));
	testCheckAssert(! canFromVariant<int *>(var));
}

void doTestVariant_CanCast()
{
	using namespace cpgf;
	
	GVariant primaryVariants[] = {
		GVariant((bool)38),
		GVariant((wchar_t)38),
		GVariant((char)38),
		GVariant((signed char)38),
		GVariant((unsigned char)38),
		GVariant((short)38),
		GVariant((unsigned short)38),
		GVariant((int)0),
		GVariant((unsigned int)38),
		GVariant((long)38),
		GVariant((unsigned long)38),
		GVariant((long long)38),
		GVariant((unsigned long long)38),
		GVariant((float)38),
		GVariant((double)38),
		GVariant((long double)38),

		GVariant(),
	};

	int index = 0;
	while(!primaryVariants[index].isEmpty()) {
		doTestVariant_CanCast_Primary(primaryVariants[index]);
		++index;
	}

	GVariant var;

	TestVariantObject obj;
	var = &obj;

	testCheckEqual(fromVariant<void *>(var), &obj);
	testCheckEqual(fromVariant<TestVariantObject *>(var), &obj);
	testCheckAssert(! canFromVariant<int>(var));
}

void testVariant()
{
	std::cout << "testVariant" << std::endl;
	
	doTestVariant_Convert();
	doTestVariant_CanCast();
}


}


gTestCase(testVariant);


#endif
