#include "cpgf/accessor/gsetter.h"

#include "test_accessor_common.h"


using namespace cpgf;

namespace {

int valueA;
TestAccessorObject object;

void setA(int value)
{
	valueA = value;
}

void setObject(const TestAccessorObject & value)
{
	object = value;
}

void setByExplicitThis(TestAccessorObject * self, int value)
{
	self->n = value;
}

class SetterData
{
public:
	SetterData() : n(0) {
	}

	void setN(int value) {
		this->n = value;
	}

public:
	int n;
};


GTEST(Setter_Global_Function_setA)
{
	valueA = 0;

	GInstanceSetter<void (*)(int)> ptr = createInstanceSetter(&setA);
	GEQUAL(NULL, ptr.getAddress(NULL));
	ptr.set(NULL, 1999);
	GEQUAL(1999, valueA);

	GInstanceSetter<void (int)> func = createInstanceSetter(setA);
	GEQUAL(NULL, func.getAddress(NULL));
	func.set(NULL, 38);
	GEQUAL(38, valueA);
}

GTEST(InstanceSetter_Global_Function_setA)
{
	valueA = 0;

	GSetter<void (*)(int)> ptr = createSetter(NULL, &setA);
	GEQUAL(NULL, ptr.getAddress());
	ptr.set(1999);
	GEQUAL(1999, valueA);
	ptr(1998);
	GEQUAL(1998, valueA);
	ptr = 1997;
	GEQUAL(1997, valueA);

	GSetter<void (int)> func = createSetter(NULL, setA);
	GEQUAL(NULL, func.getAddress());
	func.set(1);
	GEQUAL(1, valueA);
	func(2);
	GEQUAL(2, valueA);
	func = 3;
	GEQUAL(3, valueA);
}


GTEST(Setter_Global_Function_setObject)
{
	object.n = 0;

	GInstanceSetter<void (const TestAccessorObject &)> setter = createInstanceSetter(setObject);
	GEQUAL(NULL, setter.getAddress(NULL));
	setter.set(NULL, TestAccessorObject(38));
	GEQUAL(38, object.n);
}

GTEST(InstanceSetter_Global_Function_setObject)
{
	object.n = 0;

	GSetter<void (const TestAccessorObject &)> setter = createSetter(NULL, setObject);
	GEQUAL(NULL, setter.getAddress());

	setter.set(TestAccessorObject(38));
	GEQUAL(38, object.n);
	setter(TestAccessorObject(9));
	GEQUAL(9, object.n);
	setter = TestAccessorObject(1);
	GEQUAL(1, object.n);
}


GTEST(Setter_Global_Function_setByExplicitThis)
{
	TestAccessorObject a;
	a.n = 0;
	GInstanceSetter<void (TestAccessorObject *, int), GMetaPolicyExplicitThis> setter = createInstanceSetter(setByExplicitThis, GMetaPolicyExplicitThis());
	GEQUAL(NULL, setter.getAddress(&a));

	setter.set(&a, 38);
	GEQUAL(38, a.n);

	setter.set(&a, 55);
	GEQUAL(55, a.n);
}

GTEST(InstanceSetter_Global_Function_setByExplicitThis)
{
	TestAccessorObject a;
	a.n = 0;
	GSetter<void (TestAccessorObject *, int), GMetaPolicyExplicitThis> setter = createSetter(&a, setByExplicitThis, GMetaPolicyExplicitThis());
	GEQUAL(NULL, setter.getAddress());

	setter.set(1);
	GEQUAL(1, a.n);
	setter(2);
	GEQUAL(2, a.n);
	setter = 3;
	GEQUAL(3, a.n);
}


GTEST(Setter_Member_Function_setN)
{
	SetterData a;
	a.n = 0;
	GInstanceSetter<void (SetterData::*)(int)> setter = createInstanceSetter(&SetterData::setN);
	GEQUAL(NULL, setter.getAddress(&a));

	setter.set(&a, 1);
	GEQUAL(1, a.n);

	setter.set(&a, 38);
	GEQUAL(38, a.n);
}

GTEST(InstanceSetter_Member_Function_setN)
{
	SetterData a;
	a.n = 0;
	GSetter<void (SetterData::*)(int)> setter = createSetter(&a, &SetterData::setN);
	GEQUAL(NULL, setter.getAddress());

	setter.set(1);
	GEQUAL(1, a.n);
	setter(2);
	GEQUAL(2, a.n);
	setter = 3;
	GEQUAL(3, a.n);
}



} // unnamed namespace

