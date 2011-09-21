#ifndef __GTESTUTIL_H
#define __GTESTUTIL_H


#include "cpgf/gcontainer.h"
#include "cpgf/gpp.h"

#include <string>
#include <vector>
#include <stdexcept>

#include <stdlib.h>
#include <time.h>
#include <assert.h>


namespace cpgf {

template <typename KeyType>
class GTestKeyCount {
private:
	typedef GWiseMap<KeyType, int> MapType;
	typedef typename MapType::iterator IteratorType;

public:
	int incKey(const KeyType key) {
		return ++this->keyMap[key];
	}

	int decKey(const KeyType key) {
		IteratorType it = this->keyMap.find(key);

		if(it == this->keyMap.end()) {
			return 0;
		}
		else {
			--it->second;
			if(it->second <= 0) {
				this->keyMap.erase(it);
				return 0;
			}
			else {
				return it->second;
			}
		}
	}

	int getCount(const KeyType key) const {
		IteratorType it = this->keyMap.find(key);

		if(it == this->keyMap.end()) {
			return 0;
		}
		else {
			return it->second;
		}
	}

	bool empty() const {
		return this->keyMap.empty();
	}

private:
	MapType keyMap;
};

class GTestRegister
{
private:
	typedef void (*TestCaseFunc)();
	typedef std::vector<TestCaseFunc> ListType;

public:
	GTestRegister(
		TestCaseFunc p0,
		TestCaseFunc p1 = NULL, TestCaseFunc p2 = NULL, TestCaseFunc p3 = NULL,
		TestCaseFunc p4 = NULL, TestCaseFunc p5 = NULL, TestCaseFunc p6 = NULL,
		TestCaseFunc p7 = NULL, TestCaseFunc p8 = NULL, TestCaseFunc p9 = NULL,
		TestCaseFunc p10 = NULL,
		TestCaseFunc p11 = NULL, TestCaseFunc p12 = NULL, TestCaseFunc p13 = NULL,
		TestCaseFunc p14 = NULL, TestCaseFunc p15 = NULL, TestCaseFunc p16 = NULL,
		TestCaseFunc p17 = NULL, TestCaseFunc p18 = NULL, TestCaseFunc p19 = NULL
	) {
#define REG(N) if(p ## N) registerTest(p ## N)
		REG(0);
		REG(1); REG(2); REG(3);
		REG(4); REG(5); REG(6);
		REG(7); REG(8); REG(9);
		REG(10);
		REG(11); REG(12); REG(13);
		REG(14); REG(15); REG(16);
		REG(17); REG(18); REG(19);
#undef REG
	}

	static void registerTest(TestCaseFunc func) {
		if(func == NULL) {
			return;
		}

		ListType * testList = getList();

		for(ListType::iterator it = testList->begin(); it != testList->end(); ++it) {
			if(*it == func) {
				return;
			}
		}

		testList->push_back(func);
	}

	static void runAllTest() {
		ListType * testList = getList();

		for(ListType::iterator it = testList->begin(); it != testList->end(); ++it) {
			(*it)();
		}
	}

private:
	static ListType * getList() {
		static ListType testList;

		return &testList;
	}
};

#define gTestCase(...) cpgf::GTestRegister GPP_CONCAT(testReg, __LINE__)(__VA_ARGS__)

// return [minValue, maxValue)
inline int testRand(int minValue, int maxValue)
{
	static bool initialized = false;

	if(!initialized) {
		srand(static_cast<unsigned int>(time(NULL)));

		initialized = true;
	}

	if(minValue >= maxValue) {
		return minValue;
	}

	long long r = rand() * rand();

	return r % (maxValue - minValue) + minValue;
}


inline void testCheckAssert(bool b)
{
	if(!b) {
		assert(false);

		throw std::runtime_error("testCheckAssert false");
	}
}

template <typename A, typename B>
void testCheckEqual(A a, B b)
{
	if(!(a == b)) {
		testCheckAssert(false);
	}
}

template <typename A, typename B>
void testCheckStringEqual(A a, B b)
{
	testCheckEqual(std::string(a), std::string(b));
}

#define testBeginException try {
#define testEndException(e) testCheckAssert(false); } catch(e) {}


} // namespace cpgf


#endif
