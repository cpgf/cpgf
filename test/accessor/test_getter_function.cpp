#include "cpgf/accessor/ggetter.h"

#include "test_accessor_common.h"


using namespace cpgf;

namespace {

int getA()
{
	return 1999;
}

TestAccessorObject getObject()
{
	TestAccessorObject o;
	o.n = 38;
	return o;
}

int getByExplicitThis(TestAccessorObject * self)
{
	return self->n;
}

class GetterData
{
public:
	GetterData() : n(0) {
	}

	int getN() const {
		return n;
	}

	int & getRefN() {
		return n;
	}

public:
	int n;
};


GTEST(Getter_Global_Function_getA)
{
	GInstanceGetter<int (*)()> ptr = createInstanceGetter(&getA);
	GEQUAL(NULL, ptr.getAddress(NULL));

	GEQUAL(1999, ptr.get(NULL));

	GInstanceGetter<int ()> func = createInstanceGetter(getA);
	GEQUAL(NULL, func.getAddress(NULL));

	GEQUAL(1999, func.get(NULL));
}

GTEST(InstanceGetter_Global_Function_getA)
{
	GGetter<int (*)()> ptr = createGetter(NULL, &getA);
	GEQUAL(NULL, ptr.getAddress());

	GEQUAL(1999, ptr.get());
	GEQUAL(1999, ptr());
	GEQUAL(1999, ptr);

	GGetter<int ()> func = createGetter(NULL, getA);
	GEQUAL(NULL, func.getAddress());

	GEQUAL(1999, func.get());
	GEQUAL(1999, func());
	GEQUAL(1999, func);
}


GTEST(Getter_Global_Function_getObject)
{
	GInstanceGetter<TestAccessorObject ()> getter = createInstanceGetter(getObject);
	GEQUAL(NULL, getter.getAddress(NULL));

	GEQUAL(38, getter.get(NULL).n);
}

GTEST(InstanceGetter_Global_Function_getObject)
{
	GGetter<TestAccessorObject ()> getter = createGetter(NULL, getObject);
	GEQUAL(NULL, getter.getAddress());

	GEQUAL(38, getter.get().n);
	GEQUAL(38, getter().n);
}


GTEST(Getter_Global_Function_getByExplicitThis)
{
	TestAccessorObject a;
	GInstanceGetter<int (TestAccessorObject *), GMetaPolicyExplicitThis> getter = createInstanceGetter(getByExplicitThis, GMetaPolicyExplicitThis());
	GEQUAL(NULL, getter.getAddress(NULL));

	a.n = 38;
	GEQUAL(38, getter.get(&a));

	a.n = 55;
	GEQUAL(55, getter.get(&a));
}

GTEST(InstanceGetter_Global_Function_getByExplicitThis)
{
	TestAccessorObject a;
	GGetter<int (TestAccessorObject *), GMetaPolicyExplicitThis> getter = createGetter(&a, getByExplicitThis, GMetaPolicyExplicitThis());
	GEQUAL(NULL, getter.getAddress());

	a.n = 38;
	GEQUAL(38, getter.get());
	GEQUAL(38, getter());
	GEQUAL(38, getter);

	a.n = 55;
	GEQUAL(55, getter.get());
	GEQUAL(55, getter());
	GEQUAL(55, getter);
}


GTEST(Getter_Member_Function_getN)
{
	GetterData a;
	GInstanceGetter<int (GetterData::*)() const> getter = createInstanceGetter(&GetterData::getN);
	GEQUAL(NULL, getter.getAddress(NULL));

	a.n = 1;
	GEQUAL(1, getter.get(&a));

	a.n = 38;
	GEQUAL(38, getter.get(&a));
}

GTEST(InstanceGetter_Member_Function_getN)
{
	GetterData a;
	GGetter<int (GetterData::*)() const> getter = createGetter(&a, &GetterData::getN);
	GEQUAL(NULL, getter.getAddress());

	a.n = 1;
	GEQUAL(1, getter.get());
	GEQUAL(1, getter());
	GEQUAL(1, getter);

	a.n = 38;
	GEQUAL(38, getter.get());
	GEQUAL(38, getter());
	GEQUAL(38, getter);
}


GTEST(Getter_Member_Function_getRefN)
{
	GetterData a;
	GInstanceGetter<int & (GetterData::*)()> getter = createInstanceGetter(&GetterData::getRefN);
	GEQUAL(NULL, getter.getAddress(NULL));

	a.n = 1;
	GEQUAL(1, getter.get(&a));

	a.n = 38;
	GEQUAL(38, getter.get(&a));

	getter.get(&a) = 5;
	GEQUAL(5, a.n);
}

GTEST(InstanceGetter_Member_Function_getRefN)
{
	GetterData a;
	GGetter<int & (GetterData::*)()> getter = createGetter(&a, &GetterData::getRefN);
	GEQUAL(NULL, getter.getAddress());

	a.n = 1;
	GEQUAL(1, getter.get());
	GEQUAL(1, getter());
	GEQUAL(1, getter);

	a.n = 38;
	GEQUAL(38, getter.get());
	GEQUAL(38, getter());
	GEQUAL(38, getter);

	getter.get() = 5;
	GEQUAL(5, a.n);
	getter() = 3;
	GEQUAL(3, a.n);
	(int &)getter = 3;
	GEQUAL(3, a.n);
}

GTEST(Getter_Callback)
{
	auto getter = createInstanceGetter(GCallback<int ()>(
		[]() { return 38; }
	));
	GEQUAL(NULL, getter.getAddress(NULL));

	GEQUAL(38, getter.get(NULL));
}



} // unnamed namespace

