#include <stdio.h>
//#include <tchar.h>
//#include <conio.h>

#include <iostream>
#include <string>

#include "cpgf/gmetaextendtype.h"
#include "cpgf/metatraits/gmetaconverter_string.h"

#include "unittestbase.h"

using namespace std;
using namespace cpgf;

void testSer();

void testBoost();

namespace ttt {

template <typename T>
void test(T &)
{
	GMetaExtendType type;
	type = createMetaExtendType<T>(GExtendTypeCreateFlag_Converter);
	cout << "Array size: " << type.getArraySize() << endl;
	cout << "Converter: " << type.getConverter() << endl;
}

}

using namespace ttt;

int main(int argc, char * argv[])
{
	(void)argc;
	(void)argv;

//	testBoost();

	UnitTest::RunAllTests();
	testSer();

//	_getch();
	printf("\n");

	return 0;
}

