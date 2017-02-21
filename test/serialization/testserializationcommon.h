#ifndef CPGF_TESTSERIALIZATIONCOMMON_H
#define CPGF_TESTSERIALIZATIONCOMMON_H

#include "../testmetatraits.h"

#include "cpgf/serialization/gmetaarchivereader.h"
#include "cpgf/serialization/gmetaarchivewriter.h"

#include "cpgf/serialization/gmetastorage_textstream.h"
#include "cpgf/serialization/gmetastorage_xml.h"
#include "cpgf/serialization/gmetastorage_json.h"

#include "../unittestbase.h"

#include <string>
#include <sstream>

#include <math.h>


#define FIELD(cls, n) ._field(# n, &cls::n)


template <typename T>
struct TestArchiveTraits
{
	static void rewind(const T &) {
	}
};

template <>
struct TestArchiveTraits <std::stringstream>
{
	static void rewind(const std::stringstream & stream) {
		const_cast<std::stringstream &>(stream).seekg(0);
	}
};


template <typename T>
void initTestValue(T & value, long long seed)
{
	value = (T)(seed);
}

void initTestValue(bool & value, long long seed);
void initTestValue(char & value, long long seed);
void initTestValue(wchar_t & value, long long seed);
void initTestValue(signed char & value, long long seed);
void initTestValue(unsigned char & value, long long seed);
void initTestValue(float & value, long long seed);
void initTestValue(double & value, long long seed);
void initTestValue(long double & value, long long seed);
void initTestValue(std::string & value, long long seed);

long long getTestSeed(int n);


#endif

