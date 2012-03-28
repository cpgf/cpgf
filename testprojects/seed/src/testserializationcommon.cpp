#include "testserializationcommon.h"

#include <stdio.h>

using namespace std;
using namespace cpgf;

void initTestValue(bool & value, long long seed)
{
	value = ((seed % 2) != 0);
}

void initTestValue(char & value, long long seed)
{
	value = 'a' + (seed % 26);
}

void initTestValue(signed char & value, long long seed)
{
	value = 'a' + (seed % 26);
}

void initTestValue(unsigned char & value, long long seed)
{
	value = 'a' + (seed % 26);
}

void initTestValue(std::string & value, long long seed)
{
	char buffer[100];
	sprintf(buffer, "STL-string:%d", (int)seed);
	value = buffer;
}

long long getTestSeed(int n)
{
	long long seed = 0x739521LL;
	
	seed <<= (n % 32);
	seed += n;

	return seed;
}
