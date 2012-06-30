#include <stdio.h>
//#include <tchar.h>
//#include <conio.h>

#include <iostream>
#include <string>

#include "unittestbase.h"

#include "cpgf/gvariant.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/gtypetraits.h"
#include "cpgf/gmetapolicy.h"

#include "cpgf/gmemorypool.h"

#include <time.h>

using namespace std;
using namespace cpgf;
using namespace cpgf::memorypool_internal;

clock_t getTime()
{
	return clock();
}

template <typename T>
void doTest()
{
	clock_t t;

	GObjectPool<T> pool;
	const int times = 1000 * 1000;
	GScopedArray<T *> pointers(new T *[times]);

	t = getTime();
	for(int i = 0; i < times; ++i) {
		pointers[i] = pool.allocate();
	}
	for(int i = 0; i < times; ++i) {
		pool.free(pointers[i]);
	}
	cout << "Time0: " << (getTime() - t) << endl;

	t = getTime();
	for(int i = 0; i < times; ++i) {
		pointers[i] = new T;
	}
	for(int i = 0; i < times; ++i) {
		delete pointers[i];
	}
	cout << "Time1: " << (getTime() - t) << endl;
}

void test()
{
	doTest<int>();
}


void doTestPythonBind();
int main(int /*argc*/, char * /*argv*/[])
{
//	test();
//	doTestPythonBind();
	
	UnitTest::RunAllTests();

//	_getch();
	printf("\n");

	return 0;
}

