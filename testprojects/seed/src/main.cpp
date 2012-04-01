#include <stdio.h>
//#include <tchar.h>
//#include <conio.h>

#include "cpgf/gifelse.h"
#include "cpgf/gmetaextendtype.h"

#include "cpgf/metatraits/gmetaconverter_string.h"
#include "cpgf/metatraits/gmetaserializer_string.h"
#include "cpgf/metatraits/gmetaserializer_array.h"

#include <iostream>
#include <string>

#include "unittestbase.h"

using namespace std;
using namespace cpgf;

void testSer();

void testBoost();


using namespace cpgf;

template <typename T>
void abc(const T &)
{
	cout << "1" << endl;
}

template <typename T>
void test(const T & a)
{
	abc(a);
}

void abc(const string &)
{
	cout << "2" << endl;
}

int main(int argc, char * argv[])
{
	(void)argc;
	(void)argv;

//	string s;
//	test(s);
	
//	testSer();

	UnitTest::RunAllTests();

//	_getch();
	printf("\n");

	return 0;
}

