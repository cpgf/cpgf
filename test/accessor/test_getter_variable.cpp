#include "cpgf/accessor/ggetter.h"

#include "test_accessor_common.h"


using namespace cpgf;

namespace {

int globalA;
TestAccessorObject globalObject;
const TestAccessorObject globalConstObject;


GTEST(InstanceGetter_Global_Variable_int)
{
	GInstanceGetter<int *> getter = createInstanceGetter(&globalA);
	GEQUAL(&globalA, getter.getAddress(nullptr));

	globalA = 1;
	GEQUAL(1, getter.get(nullptr));

	globalA = 38;
	GEQUAL(38, getter.get(nullptr));
}

GTEST(Getter_Global_Variable_int)
{
	GGetter<int *> getter = createGetter(nullptr, &globalA);
	GEQUAL(&globalA, getter.getAddress());

	globalA = 1;
	GEQUAL(1, getter.get());
	GEQUAL(1, getter());
	GEQUAL(1, getter);

	globalA = 38;
	GEQUAL(38, getter.get());
	GEQUAL(38, getter());
	GEQUAL(38, getter);
}


GTEST(InstanceGetter_Global_Variable_object)
{
	GInstanceGetter<TestAccessorObject *> getter = createInstanceGetter(&globalObject);
	GEQUAL(&globalObject, getter.getAddress(nullptr));

	globalObject.n = 1;
	GEQUAL(1, getter.get(nullptr).n);

	globalObject.n = 38;
	GEQUAL(38, getter.get(nullptr).n);
}

GTEST(Getter_Global_Variable_object)
{
	GGetter<TestAccessorObject *> getter = createGetter(nullptr, &globalObject);
	GEQUAL(&globalObject, getter.getAddress());

	globalObject.n = 1;
	GEQUAL(1, getter.get().n);
	GEQUAL(1, getter().n);

	globalObject.n = 38;
	GEQUAL(38, getter.get().n);
	GEQUAL(38, getter().n);
}


GTEST(InstanceGetter_Global_Variable_const_object)
{
	GInstanceGetter<const TestAccessorObject *> getter = createInstanceGetter(&globalConstObject);
	GEQUAL(&globalConstObject, getter.getAddress(nullptr));

	const_cast<TestAccessorObject *>(&globalConstObject)->n = 1;
	GEQUAL(1, getter.get(nullptr).n);

	const_cast<TestAccessorObject *>(&globalConstObject)->n = 38;
	GEQUAL(38, getter.get(nullptr).n);
}

GTEST(Getter_Global_Variable_const_object)
{
	GGetter<const TestAccessorObject *> getter = createGetter(nullptr, &globalConstObject);
	GEQUAL(&globalConstObject, getter.getAddress());

	const_cast<TestAccessorObject *>(&globalConstObject)->n = 1;
	GEQUAL(1, getter.get().n);
	GEQUAL(1, getter().n);

	const_cast<TestAccessorObject *>(&globalConstObject)->n = 38;
	GEQUAL(38, getter.get().n);
	GEQUAL(38, getter().n);
}

GTEST(InstanceGetter_Global_Variable_Array)
{
	int data[3] { 5, 38, 1999 };
	auto getter = createInstanceGetter(&data);
	GEQUAL(data, getter.get(nullptr));
}

GTEST(Getter_Global_Variable_Array)
{
	int data[3] { 5, 38, 1999 };
	auto getter = createGetter(nullptr, &data);
	GEQUAL(data, getter.get());
}


GTEST(InstanceGetter_Member_Variable_object)
{
	TestAccessorObject a;
	GInstanceGetter<int TestAccessorObject::*> getter = createInstanceGetter(&TestAccessorObject::n);
	GEQUAL(&a.n, getter.getAddress(&a));

	a.n = 1;
	GEQUAL(1, getter.get(&a));

	a.n = 38;
	GEQUAL(38, getter.get(&a));
}

GTEST(Getter_Member_Variable_object)
{
	TestAccessorObject a;
	GGetter<int TestAccessorObject::*> getter = createGetter(&a, &TestAccessorObject::n);
	GEQUAL(&a.n, getter.getAddress());

	a.n = 1;
	GEQUAL(1, getter.get());
	GEQUAL(1, getter());
	GEQUAL(1, getter);

	a.n = 38;
	GEQUAL(38, getter.get());
	GEQUAL(38, getter());
	GEQUAL(38, getter);
}


GTEST(InstanceGetter_Member_Variable_const_object)
{
	const TestAccessorObject a;
	GInstanceGetter<int TestAccessorObject::*> getter = createInstanceGetter(&TestAccessorObject::n);
	GEQUAL(&a.n, getter.getAddress(&a));

	const_cast<TestAccessorObject *>(&a)->n = 1;
	GEQUAL(1, getter.get(&a));

	const_cast<TestAccessorObject *>(&a)->n = 38;
	GEQUAL(38, getter.get(&a));
}

GTEST(Getter_Member_Variable_const_object)
{
	const TestAccessorObject a;
	GGetter<int TestAccessorObject::*> getter = createGetter(&a, &TestAccessorObject::n);
	GEQUAL(&a.n, getter.getAddress());

	const_cast<TestAccessorObject *>(&a)->n = 1;
	GEQUAL(1, getter.get());
	GEQUAL(1, getter());
	GEQUAL(1, getter);

	const_cast<TestAccessorObject *>(&a)->n = 38;
	GEQUAL(38, getter.get());
	GEQUAL(38, getter());
	GEQUAL(38, getter);
}

GTEST(InstanceGetter_Member_Variable_array)
{
	const TestAccessorObject a;
	auto getter = createInstanceGetter(&TestAccessorObject::arr);
	GEQUAL((void *)&a.arr, (void *)getter.getAddress(&a));

	GEQUAL((void *)&a.arr, (void *)getter.get(&a));
}


} // unnamed namespace

