#ifndef __TESTSERIALIZATIONCOMMON_H
#define __TESTSERIALIZATIONCOMMON_H

#include "cpgf/metatraits/gmetaserializer_string.h"
#include "cpgf/metatraits/gmetaserializer_array.h"

#include "unittestbase.h"

#include <string>


#define FIELD(cls, n) ._field(# n, &cls::n)

template <typename STREAM>
class TestArchiveStream
{
public:
	explicit TestArchiveStream(STREAM & stream) : stream(stream) {}
	
	void rewind() const {
		this->stream.seekg(0);
	}

private:
	TestArchiveStream & operator = (const TestArchiveStream &);
	
private:
	STREAM & stream;
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

