#include "cpgf/accessor/gaccessor.h"

#include "test_accessor_common.h"


using namespace cpgf;

namespace {

int globalA;

GTEST(InstanceAccessor_RawGetter_RawSetter)
{
	globalA = 0;

	auto accessor = createInstanceAccessor(&globalA, &globalA);
	GEQUAL(&globalA, accessor.getAddress(NULL));
	GEQUAL(0, accessor.get(NULL));

	accessor.set(NULL, 5);
	GEQUAL(5, globalA);
	GEQUAL(5, accessor.get(NULL));
}

GTEST(InstanceAccessor_Getter_Setter)
{
	globalA = 0;

	auto accessor = createInstanceAccessor(createInstanceGetter(&globalA), createInstanceSetter(&globalA));
	GEQUAL(&globalA, accessor.getAddress(NULL));
	GEQUAL(0, accessor.get(NULL));

	accessor.set(NULL, 5);
	GEQUAL(5, globalA);
	GEQUAL(5, accessor.get(NULL));
}

GTEST(Accessor_RawGetter_RawSetter)
{
	globalA = 0;

	auto accessor = createAccessor(NULL, &globalA, &globalA);
	GEQUAL(&globalA, accessor.getAddress());
	GEQUAL(0, accessor.get());

	accessor.set(5);
	GEQUAL(5, globalA);
	GEQUAL(5, accessor.get());
}

GTEST(Accessor_Getter_Setter)
{
	globalA = 0;

	auto accessor = createAccessor(createGetter(nullptr, &globalA), createSetter(nullptr, &globalA));
	GEQUAL(&globalA, accessor.getAddress());
	GEQUAL(0, accessor.get());

	accessor.set(5);
	GEQUAL(5, globalA);
	GEQUAL(5, accessor.get());
}


GTEST(InstanceAccessor_Getter_NoSetter)
{
	globalA = 0;

	auto accessor = createInstanceAccessor(&globalA, 0);
	GEQUAL(&globalA, accessor.getAddress(NULL));
	GEQUAL(0, accessor.get(NULL));

	globalA = 5;
	GEQUAL(5, accessor.get(NULL));
}

GTEST(Accessor_Getter_NoSetter)
{
	globalA = 0;

	auto accessor = createAccessor(NULL, &globalA, 0);
	GEQUAL(&globalA, accessor.getAddress());
	GEQUAL(0, accessor.get());

	globalA = 5;
	GEQUAL(5, accessor.get());
}


GTEST(InstanceAccessor_NoGetter_Setter)
{
	globalA = 0;

	auto accessor = createInstanceAccessor(0, &globalA);
	GEQUAL(&globalA, accessor.getAddress(NULL));

	accessor.set(NULL, 5);
	GEQUAL(5, globalA);
}

GTEST(Accessor_NoGetter_Setter)
{
	globalA = 0;

	auto accessor = createAccessor(NULL, 0, &globalA);
	GEQUAL(&globalA, accessor.getAddress());

	accessor.set(5);
	GEQUAL(5, globalA);
}



} // unnamed namespace

