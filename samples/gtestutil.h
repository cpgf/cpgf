#ifndef CPGF_GTESTUTIL_H
#define CPGF_GTESTUTIL_H

#include "cpgf/gpp.h"

#include <string>
#include <vector>
#include <stdexcept>

#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <map>

namespace cpgf {

template <typename KeyType>
class GTestKeyCount {
private:
	typedef std::map<KeyType, int> MapType;
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
		TestCaseFunc p1 = nullptr, TestCaseFunc p2 = nullptr, TestCaseFunc p3 = nullptr,
		TestCaseFunc p4 = nullptr, TestCaseFunc p5 = nullptr, TestCaseFunc p6 = nullptr,
		TestCaseFunc p7 = nullptr, TestCaseFunc p8 = nullptr, TestCaseFunc p9 = nullptr,
		TestCaseFunc p10 = nullptr,
		TestCaseFunc p11 = nullptr, TestCaseFunc p12 = nullptr, TestCaseFunc p13 = nullptr,
		TestCaseFunc p14 = nullptr, TestCaseFunc p15 = nullptr, TestCaseFunc p16 = nullptr,
		TestCaseFunc p17 = nullptr, TestCaseFunc p18 = nullptr, TestCaseFunc p19 = nullptr
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
		if(func == nullptr) {
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
		srand(static_cast<unsigned int>(time(nullptr)));

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
