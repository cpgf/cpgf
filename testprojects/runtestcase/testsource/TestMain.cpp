#if defined(_MSC_VER)
// If you don't have VLD, comment out of remove below line
#include "vld.h"
#endif

//#define RUN_BENCHMARK

#include <iostream>

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>

#include <iostream>

#include <assert.h>
#include <conio.h>

#include "cpgf/gtestutil.h"
#include "cpgf/testcase/benchmark/benchmarkcallback.h"
#include "cpgf/testcase/tutorial_callback.h"

#include "cpgf/testcase/TestCallback.h"
#include "cpgf/gfunctiontraits.h"
#include "cpgf/gplaincallback.h"

//#include "sigc++/functors/slot.h"
//#include "boost/signals2.hpp"
//#include "boost/function.hpp"

#include <windows.h>
#include <stdlib.h>

using namespace std;
using namespace cpgf;


typedef int Compare(const void *, const void *);
// below also works
//typedef int (*Compare)(const void *, const void *);

struct CompareObject {
	int operator() (const void * a, const void * b) const {
		return (*(int*)a - *(int*)b);
	}
};

struct RevertObject {
	int revertCompare(const void * a, const void * b) {
		return (*(int*)b - *(int*)a);
	}
};

//extern GCallback<Compare> cb;
GCallback<Compare> cb; // can be extern, but can't be static (internal linkage)

CompareObject co; // same as cb, co must have external linkage

void testPlainCallback() {
	int values[] = { 6, 2, 1, 3, 5, 4 };
	const int count = sizeof(values) / sizeof(values[0]);

	qsort(values, count, sizeof(values[0]), &GPlainFunction<Compare, CompareObject, co>::invoke);
	cout << "Normal qsort: \t\t";
	for(int i = 0; i < count; ++i) {
		cout << values[i] << " ";
	}
	cout << endl;

	RevertObject ro;
	cb = GCallback<Compare>(&ro, &RevertObject::revertCompare);
	qsort(values, count, sizeof(values[0]), &GPlainCallback<Compare, cb>::invoke);
	cout << "Reverted qsort: \t";
	for(int i = 0; i < count; ++i) {
		cout << values[i] << " ";
	}
	cout << endl;
}

template<int ** pp, typename>
struct AAA
{
	static int get() {
		static int n = **pp;
		return n;
	}

	static void test() {
		cout << get() << endl;
	}
};

int p = 5;

const int & fff()
{
	cout << "ffffffffffffffff" << endl;
	return p;
}

int runTestMain()
{
#ifdef RUN_BENCHMARK
    runBenchmarkCallback();
//    cout << "press any key..." << endl;
//    _getch();
    return 0;
#else
	runTutorials();

	cpgf::GTestRegister::runAllTest();

	int ret = CxxTest::ErrorPrinter().run();

	_getch();

	return ret;
#endif
}


