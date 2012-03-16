#include <stdio.h>
//#include <tchar.h>
//#include <conio.h>

#include <iostream>

#include "cpgf/gmetaextendtype.h"

#include "unittestbase.h"

using namespace std;
using namespace cpgf;

void testSer();

void testBoost();

template <typename T>
void test(T &)
{
	GMetaExtendType type;
	type = createMetaExtendType<T>();
	cout << "Array size: " << type.data.arraySize << endl;
}

int main(int argc, char * argv[])
{
	(void)argc;
	(void)argv;

	int xxx[] = { 5, 6, 7 };
	test(xxx);

	int yyy[5];
	test(yyy);

//	testBoost();

	UnitTest::RunAllTests();
	testSer();

//	_getch();
	printf("\n");

	return 0;
}

