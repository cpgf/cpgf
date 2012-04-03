#include <stdio.h>
//#include <tchar.h>
//#include <conio.h>

#include "cpgf/gifelse.h"
#include "cpgf/gmetaextendtype.h"

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
struct X
{
	static void doIt() {
		cout << "X1" << endl;
	}
};

template <typename T>
void test(const T & a)
{
	abc(a);
	X<T>::doIt();
}

template <>
void abc(const string &)
{
	cout << "2" << endl;
}

template <>
struct X <string>
{
	static void doIt() {
		cout << "X2" << endl;
	}
};


int main(int argc, char * argv[])
{
	(void)argc;
	(void)argv;

#error "TODO: XML storage should not require unique node name. This can be fixed by maintainning an internal counter.";
//	string s;
//	test(s);
	
//	testBoost();
//	testSer();

	UnitTest::RunAllTests();

//	_getch();
	printf("\n");

	return 0;
}

