#ifndef CPGF_REFLECT_COMMON_H
#define CPGF_REFLECT_COMMON_H

#include "cpgf/gtestutil.h"

#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

#include "cpgf/gmetaapiutil.h"
#include "cpgf/gscopedinterface.h"

#include <iostream>
#include <string>


#if defined(_MSC_VER)
#pragma warning(push)
// 'class1' : inherits 'class2::member' via dominance
#pragma warning(disable:4250)
// 'sprintf': This function or variable may be unsafe
#pragma warning(disable:4996)
#endif


class TestData
{
public:
	TestData()
		:	x(1999),
			tag("a tag")
	{
	}

	TestData(int x, std::string tag)
		:	x(x),
			tag(tag)
	{
	}

	TestData(const TestData & other)
		:	x(other.x),
			tag(other.tag)
	{
	}

	TestData & operator = (const TestData & other) {
		this->x = other.x;
		this->tag = other.tag;

		return *this;
	}

	bool operator == (const TestData & other) const {
		return this->x == other.x
			&& this->tag == other.tag;
	}

public:
	int x;
	std::string tag;
};


class TestNoncopyable
{
public:
	TestNoncopyable()
		:	x(1999),
			tag("a tag")
	{
	}

	TestNoncopyable(int x, const std::string & tag)
		:	x(x),
			tag(tag)
	{
	}

	bool operator == (const TestNoncopyable & other) const {
		return this->x == other.x
			&& this->tag == other.tag;
	}

private:
	TestNoncopyable(const TestNoncopyable & other);
	TestNoncopyable & operator = (const TestNoncopyable & other);

public:
	int x;
	std::string tag;
};


/*
	check the enum is in form or
	begin = startValue,
	prefix0 = begin + 0,
	prefix1 = begin + step * 1,
	prefix2 = begin + step * 2,
	prefixN = begin + step * N,
	end = prefixN
*/
inline void checkEnum(const cpgf::GMetaEnum * metaEnum, const char * prefix)
{
	using namespace cpgf;

	testCheckAssert(metaEnum != NULL);

	size_t valueCount = metaEnum->getCount();

	testCheckAssert(valueCount > 0);

	int startValue = fromVariant<int>(metaEnum->getValue(0));
	int endValue = fromVariant<int>(metaEnum->getValue(valueCount - 1));
	size_t step = valueCount == 1 ? 0 : (endValue - startValue) / (valueCount - 1);
	testCheckEqual(fromVariant<int>(metaEnum->getValue(valueCount - 1)), static_cast<int>(startValue + (valueCount - 1) * step));

	char name[100];
	for(size_t i = 0; i < valueCount; ++i) {
		sprintf(name, "%s%d", prefix, static_cast<int>(i));
		testCheckStringEqual(metaEnum->getKey(i), name);
		testCheckEqual(fromVariant<int>(metaEnum->getValue(i)), static_cast<int>(startValue + i * step));
	}
}

template <typename Meta>
inline void checkEnum(Meta & metaEnum, const char * prefix)
{
	using namespace cpgf;

	testCheckAssert(metaEnum);

	size_t valueCount = metaEnum->getCount();

	testCheckAssert(valueCount > 0);

	int startValue = fromVariant<int>(metaGetEnumValue(metaEnum.get(), 0));
	int endValue = fromVariant<int>(metaGetEnumValue(metaEnum.get(), valueCount - 1));
	size_t step = valueCount == 1 ? 0 : (endValue - startValue) / (valueCount - 1);
	testCheckEqual(fromVariant<int>(metaGetEnumValue(metaEnum.get(), valueCount - 1)), static_cast<int>(startValue + (valueCount - 1) * step));

	char name[100];
	for(unsigned int i = 0; i < valueCount; ++i) {
		sprintf(name, "%s%d", prefix, i);
		testCheckStringEqual(metaEnum->getKey(i), name);
		testCheckEqual(fromVariant<int>(metaGetEnumValue(metaEnum.get(), i)), static_cast<int>(startValue + i * step));
	}
}



#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif
