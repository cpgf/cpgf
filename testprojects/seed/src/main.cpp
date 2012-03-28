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
	cout << "Trapped all." << endl;
}

template <typename T, int N>
void abc(const T (&x) [N])
{
	(void)x;
	cout << "Array." << endl;
}

template <typename T>
void test(T &)
{
	GMetaExtendType type;
	type = createMetaExtendType<T>(GExtendTypeCreateFlag_Converter | GExtendTypeCreateFlag_Serializer);
	cout << "Array size: " << type.getArraySize() << endl;
	cout << "Converter: " << type.getConverter() << endl;
	cout << "Serializer: " << type.getSerializer() << endl;
}

int main(int argc, char * argv[])
{
	(void)argc;
	(void)argv;

	int a[5];
//	test(a);
	
//	testSer();

	UnitTest::RunAllTests();

//	_getch();
	printf("\n");

	return 0;
}

