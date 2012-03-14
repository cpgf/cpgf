#include <stdio.h>
//#include <tchar.h>
//#include <conio.h>

#include <iostream>

#include "unittestbase.h"

using namespace std;

void testSer();

void testBoost();

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

