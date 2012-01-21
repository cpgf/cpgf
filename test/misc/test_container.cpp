#include "test_misc_common.h"

#include <string>
#include <ctime>
#include <math.h>
#include <stdarg.h>

#include "cpgf/gcontainer.h"
#include "cpgf/gpp.h"

using namespace cpgf;
using namespace std;

namespace Test_Container { namespace {

#define callTest(t) t;

inline double getTestRandom()
{
	static bool randIntialized = false;

	if(!randIntialized) {
		randIntialized = true;
		srand((unsigned int)time(NULL));
	}

	return (double)rand() / ((double)RAND_MAX + 1.0);
}

inline int getRandInt()
{
	return static_cast<int>(getTestRandom() * 1000.0);
}

// [first, last)
template <typename T>
inline void generateRandomArray(T * buffer, const T & first, const T & last)
{
	T count = last - first;
	T it;

	for(it = T(0); it < count; ++it) {
		buffer[it] = first + it;
	}

	for(int i = 0; i < 10000; ++i) {
		T p1 = static_cast<int>(getTestRandom() * count);
		T p2 = static_cast<int>(getTestRandom() * count);

		if(p1 != p2) {
			std::swap(buffer[p1], buffer[p2]);
		}
	}
}

const int lastValue = -199999;

template <typename ContainerType>
inline bool checkContainerSame(const ContainerType & c1, const ContainerType & c2) {
	typename ContainerType::const_iterator it1 = c1.begin();
	typename ContainerType::const_iterator it2 = c2.begin();

	while(it1 != c1.end() && it2 != c2.end()) {
		if(*it1 != *it2) {
			return false;
		}

		++it1;
		++it2;
	}

	if(it1 != c1.end() || it1 != c1.end()) {
		return false;
	}

	return true;
}

template <typename ContainerType, typename CompareType>
inline void checkInOrder(ContainerType & c, CompareType compare) {
	typedef typename ContainerType::iterator IteratorType;

	IteratorType it;

	for(it = c.begin(); it != c.end(); ++it) {
		IteratorType itNext = it;

		++itNext;
		if(itNext != c.end()) {
			GCHECK(compare(*it, *itNext));
		}
	}
}

template <typename ContainerType>
inline bool checkContainerSwap(const ContainerType & c1, const ContainerType & c2) {
	ContainerType t1 = c1;
	ContainerType t2;
	t2 = c2;
	swap(t1, t2);

	if(!checkContainerSame(c1, t2)) {
		return false;
	}

	if(!checkContainerSame(c2, t1)) {
		return false;
	}

	return true;
}

template <typename ContainerType>
inline void doTestBeginEnd(const ContainerType & container) {
	typedef typename ContainerType::iterator IteratorType;

	ContainerType & c = const_cast<ContainerType &>(container);

	c.clear();

	IteratorType itBegin = c.begin();
	IteratorType itEnd;
	itEnd = c.end();

	GEQUAL(itBegin, itEnd);
	++itBegin;
	GEQUAL(itBegin, itEnd);

	// maybe not standard?
	++itEnd;
	GEQUAL(itBegin, itEnd);
}

template <typename ContainerType>
inline void checkContainerValues(const ContainerType * container, ...)
{
	va_list v;
	va_start(v, container);

	typename ContainerType::const_iterator it = container->begin();
	while(true) {
		int n = va_arg(v, int);

		if(n == lastValue) {
			break;
		}

		GDIFF(it, container->end());
		GEQUAL(n, *it);

		++it;
	}
	GEQUAL(it, container->end());

	va_end(v);
}

// not really test, but to check the disassembly code to see how iterator increment is optimized.
template <typename ContainerType>
inline void checkContainerIteratorIncrement(ContainerType & container)
{
	typename ContainerType::iterator it;
	it = container.begin();

//__asm int 3;

	it++;
	++it;
	if(it == container.end()) {
		it = container.begin();
	}
}

	GTEST(test_BeginEnd)
	{
		callTest(doTestBeginEnd(GWiseList<int>()));
		callTest(doTestBeginEnd(GWiseSkipList<int>()));
		callTest(doTestBeginEnd(GWiseMap<int, GWiseList<int> >()));
		GWiseQuadNode<int> quad;
		callTest(doTestBeginEnd(quad));
	}


	GTEST(testList_Basic)
	{
		typedef GWiseList<int> IntList;

		IntList st;
		GWiseList<int>::iterator it;

		st.push_back(1);
		callTest(checkContainerValues<IntList>(&st, 1, lastValue));

		st.push_front(2);
		callTest(checkContainerValues<IntList>(&st, 2, 1, lastValue));

		st.insert(st.end(), 3);
		callTest(checkContainerValues<IntList>(&st, 2, 1, 3, lastValue));

		it = st.begin();
		std::advance(it, 2); // point to 3
		st.insert(it, 4);
		callTest(checkContainerValues<IntList>(&st, 2, 1, 4, 3, lastValue));

		--it; // point to 4
		st.insertBefore(it, 6);
		callTest(checkContainerValues<IntList>(&st, 2, 1, 6, 4, 3, lastValue));
		st.insertAfter(it, 5);
		callTest(checkContainerValues<IntList>(&st, 2, 1, 6, 4, 5, 3, lastValue));

		typedef GWiseList<std::string> StringList;
		StringList * stringList = new StringList;

		stringList->push_back("def");
		GEQUAL(*(stringList->begin()), "def");
		stringList->push_front("abc");
		GEQUAL(*(stringList->begin()), "abc");
		GEQUAL(*(++stringList->begin()), "def");

		delete stringList;
	}

    GTEST(testList_Int0)
    {
		typedef GWiseList<int> IntList;

		IntList st;
		const int ItemCount = 100;

		int i;

		for(i = 0; i < ItemCount; ++i) {
			st.push_back(i);
		}

		IntList temp = st;
		st = temp;
		GCHECK(checkContainerSame(temp, st));

		temp.push_back(2);
		GCHECK(checkContainerSwap(temp, st));

		IntList temp2;
		temp2.push_back(5);
		temp = temp2;
		GCHECK(checkContainerSame(temp, temp2));

		for(IntList::iterator it1 = st.begin(); it1 != st.end(); ++it1) {
			for(IntList::iterator it2 = st.begin(); it2 != st.end(); ++it2) {
				if(*it2 % 2 != 0) {
					st.erase(it2);
				}
			}
		}
		GEQUAL((int)st.size(), (ItemCount + 1) / 2);

		st.clear();
		for(i = 0; i < ItemCount; ++i) {
			st.push_back(i);
		}

		for(IntList::iterator it1 = st.begin(); it1 != st.end(); ++it1) {
			for(IntList::reverse_iterator it2 = st.rbegin(); it2 != st.rend(); ++it2) {
				if(*it2 % 2 != 0) {
					st.erase(it2);
				}
			}
		}
		GEQUAL((int)st.size(), (ItemCount + 1) / 2);
	}

    GTEST(testList_Int)
    {
		typedef GWiseList<int> IntList;

		IntList st;

		GEQUAL(st.size(), 0u);
		GCHECK(st.empty());

		const int ItemCount = 100;

		int i;
		int count;

		for(i = 0; i < ItemCount; ++i) {
			st.push_back(i);
		}
		GEQUAL((int)st.size(), ItemCount);
		GCHECK(!st.empty());

		count = 0;
		for(IntList::iterator it = st.begin(); it != st.end(); ++it) {
			GEQUAL(*it, count);
			++count;
		}
		GEQUAL(count, ItemCount);

		for(IntList::iterator it1 = st.begin(); it1 != st.end(); ++it1) {
			for(IntList::iterator it2 = st.begin(); it2 != st.end(); ++it2) {
				if(*it2 % 3 == 0 || *it2 % 3 == 1) {
					st.erase(it2);
				}
			}
			for(IntList::reverse_iterator it3 = st.rbegin(); it3 != st.rend(); ++it3) {
				if(*it3 % 7 == 0) {
					st.erase(it3);
				}
			}
		}
		for(IntList::iterator it = st.begin(); it != st.end(); ++it) {
			GCHECK(*it % 3 != 0);
			GCHECK(*it % 7 != 0);
		}

		checkContainerIteratorIncrement(st);

		st.clear();
		GEQUAL(st.size(), 0u);
		GCHECK(st.empty());
	}

    GTEST(testList_IntReverse)
    {
		typedef GWiseList<int> IntList;

		IntList st;

		const int ItemCount = 100;

		int i;

		for(i = 0; i < ItemCount; ++i) {
			st.push_back(i);
		}

		st.reverse();

		i = ItemCount - 1;
		for(IntList::iterator it = st.begin(); it != st.end(); ++it) {
			GEQUAL(*it, i--);
		}
	}


	GTEST(testSkipList_Basic)
	{
		typedef GWiseSkipList<int> IntList;

		IntList st;
		GWiseSkipList<int>::iterator it;
		GWiseSkipList<int>::reverse_iterator rit;

		st.insert(20);
		callTest(checkContainerValues<IntList>(&st, 20, lastValue));

		st.insert(10);
		callTest(checkContainerValues<IntList>(&st, 10, 20, lastValue));

		st.insert(30);
		callTest(checkContainerValues<IntList>(&st, 10, 20, 30, lastValue));

		st.remove(20);
		callTest(checkContainerValues<IntList>(&st, 10, 30, lastValue));

		it = st.find(10);
		GDIFF(it, st.end());

		st.replace(it, 50);
		callTest(checkContainerValues<IntList>(&st, 30, 50, lastValue));

		rit = st.rbegin();
		st.replace(rit, 10);
		callTest(checkContainerValues<IntList>(&st, 10, 30, lastValue));

		typedef GWiseSkipList<std::string> StringList;
		StringList * stringList = new StringList;

		stringList->insert("def");
		GEQUAL(*(stringList->begin()), "def");
		stringList->insert("abc");
		GEQUAL(*(stringList->begin()), "abc");
		GEQUAL(*(++stringList->begin()), "def");

		delete stringList;
	}

	GTEST(testSkipList_Order)
	{
		typedef GWiseSkipList<int> IntList;

		IntList st;
		const unsigned int ItemCount = 100;

		unsigned int i;

		for(i = 0; i < ItemCount; ++i) {
			st.insert(getRandInt());
		}

		GCHECK(st.size() <= ItemCount);

		// may endless dead loop
		for(IntList::iterator it = st.begin(); it != st.end(); ++it) {
			st.replace(it, *it + getRandInt());
		}
		callTest(checkInOrder(st, std::less<int>()));
	}

	GTEST(testSkipList_Int0)
	{
		typedef GWiseSkipList<int> IntList;

		IntList st;
		const int ItemCount = 100;
		int buffer[ItemCount];

		int i;

		generateRandomArray(buffer, 0, ItemCount);

		for(i = 0; i < ItemCount; ++i) {
			st.insert(buffer[i]);
		}

		IntList temp = st;
		IntList temp2;
		temp2 = temp;
		GCHECK(checkContainerSame(temp2, st));

		temp.insert(6);
		GCHECK(checkContainerSwap(temp, st));

		int current;

		const IntList & clist1 = st;

		current = 0;
		for(IntList::const_iterator it = clist1.begin(); it != clist1.end(); ++it) {
			GEQUAL(*it, current);
			++current;
		}
		GEQUAL(current, ItemCount);

		current = ItemCount - 1;
		for(IntList::const_reverse_iterator it = clist1.rbegin(); it != clist1.rend(); ++it) {
			GEQUAL(*it, current);
			--current;
		}
		GEQUAL(current, -1);
	}

	GTEST(testSkipList_Int1)
	{
		typedef GWiseSkipList<int> IntList;

		IntList st;
		const int ItemCount = 10;

		int i;

		for(i = 0; i < ItemCount; ++i) {
			st.insert(i);
		}

		for(IntList::iterator it1 = st.begin(); it1 != st.end(); ++it1) {
			for(IntList::iterator it2 = st.begin(); it2 != st.end(); ++it2) {
				if(*it2 % 2 != 0) {
					st.erase(it2);
				}
			}
		}
		GEQUAL((int)st.size(), (ItemCount + 1) / 2);

		st.clear();
		for(i = 0; i < ItemCount; ++i) {
			st.insert(i);
		}

		for(IntList::iterator it1 = st.begin(); it1 != st.end(); ++it1) {
			for(IntList::reverse_iterator it2 = st.rbegin(); it2 != st.rend(); ++it2) {
				if(*it2 % 2 != 0) {
					st.erase(it2);
				}
			}
		}
		GEQUAL((int)st.size(), (ItemCount + 1) / 2);

		checkContainerIteratorIncrement(st);
	}

	GTEST(testSkipList_UniqueInt)
	{
		typedef GWiseSkipList<int> IntList;

		IntList st;

		GEQUAL(st.size(), 0u);
		GCHECK(st.empty());

		const int ItemCount = 100;

		int i;
		int count;

		for(i = ItemCount - 1; i >= 0; i--) {
			st.insert(i);
		}
		for(i = 0; i < ItemCount; ++i) {
			st.insert(i);
		}
		GEQUAL((int)st.size(), ItemCount);
		GCHECK(!st.empty());

		count = 0;
		for(IntList::iterator it = st.begin(); it != st.end(); ++it) {
			GEQUAL(*it, count);
			++count;
		}
		GEQUAL(count, ItemCount);

		for(i = ItemCount - 1; i >= 0; i--) {
			GCHECK(st.find(i) != st.end());
		}

		for(IntList::iterator it1 = st.begin(); it1 != st.end(); ++it1) {
			for(IntList::iterator it2 = st.begin(); it2 != st.end(); ++it2) {
				if(*it2 % 3 == 0) {
					st.erase(it2);
				}
			}
		}
		for(IntList::iterator it = st.begin(); it != st.end(); ++it) {
			GCHECK(*it % 3 != 0);
		}

		st.clear();
		GEQUAL(st.size(), 0u);
		GCHECK(st.empty());
	}

	GTEST(testSkipList_DuplicatedInt)
	{
		typedef GWiseSkipList<int> IntList;

		IntList st;

		GEQUAL(st.size(), 0u);
		GCHECK(st.empty());

		const int ItemCount = 1;

		int i;
		int count;

		for(i = ItemCount - 1; i >= 0; i--) {
			st.insert(i, true);
		}
		for(i = 0; i < ItemCount; ++i) {
			st.insert(i, true);
		}
		GEQUAL((int)st.size(), ItemCount * 2);
		GCHECK(!st.empty());

		count = 0;
		for(IntList::iterator it = st.begin(); it != st.end(); ++it) {
			GEQUAL(*it, count / 2);
			++count;
		}
		GEQUAL(count, ItemCount * 2);

		for(i = ItemCount - 1; i >= 0; i--) {
			GCHECK(st.find(i) != st.end());
		}

		for(IntList::iterator it1 = st.begin(); it1 != st.end(); ++it1)	{
			for(IntList::iterator it2 = st.begin(); it2 != st.end(); ++it2) {
				if(*it2 % 3 == 0) {
					st.erase(it2);
				}
			}
		}
		for(IntList::iterator it = st.begin(); it != st.end(); ++it) {
			GCHECK(*it % 3 != 0);
		}

		st.clear();
		GEQUAL(st.size(), 0u);
		GCHECK(st.empty());
	}


	GTEST(testMap_int0)
	{
		typedef GWiseMap<int, int> MapType;

		const int ItemCount = 100;
		const int ValueDelta = 1999;

		int i;
		MapType m;

		for(i = ItemCount - 1; i >= 0; --i) {
			if(i % 2 == 0) {
				m[i] = i + ValueDelta;
			}
			else {
				m.insert(i, i + ValueDelta);
			}
		}

		GEQUAL((int)m.size(), ItemCount);

		MapType temp = m;
		m = temp;
		GCHECK(checkContainerSame(temp, m));

		temp.insert(3800, 6);
		GCHECK(checkContainerSwap(temp, m));

		int current;

		current = 0;
		for(MapType::iterator it = m.begin(); it != m.end(); ++it) {
			GEQUAL(it->first, current);
			GEQUAL(it->second, it->first + ValueDelta);

			++current;
		}

		current = ItemCount - 1;
		for(MapType::reverse_iterator it = m.rbegin(); it != m.rend(); ++it) {
			GEQUAL(it->first, current);
			GEQUAL(it->second, it->first + ValueDelta);

			--current;
		}

		for(MapType::iterator it1 = m.begin(); it1 != m.end(); ++it1) {
			for(MapType::iterator it2 = m.begin(); it2 != m.end(); ++it2) {
				if(it2->second % 2 != 0) {
					m.remove(it2->first);
				}
			}
		}
		for(MapType::iterator it1 = m.begin(); it1 != m.end(); ++it1) {
			GCHECK(m.contains(it1->first));
			GEQUAL(m.find(it1->first), it1);
			GEQUAL(it1->second % 2, 0);
			GEQUAL(m[it1->first], it1->first + ValueDelta);
		}
		GEQUAL((int)m.size(), ItemCount / 2);
	}


	GTEST(testQuadNode_Int0)
	{
		typedef GWiseQuadNode<long> NodeType;

		NodeType st;
		const int ItemCount = 100;

		int i;

		for(i = 0; i < ItemCount; ++i) {
			(new NodeType(i))->setParentNode(&st);
		}

		int current;

		current = 0;
		for(NodeType::iterator it = st.begin(); it != st.end(); ++it) {
			GEQUAL(*it, current);
			++current;
		}
		GEQUAL(current, ItemCount);

		current = ItemCount - 1;
		for(NodeType::reverse_iterator it = st.rbegin(); it != st.rend(); ++it) {
			GEQUAL(*it, current);
			--current;
		}
		GEQUAL(current, -1);

		for(NodeType::iterator it1 = st.begin(); it1 != st.end(); ++it1) {
			for(NodeType::reverse_iterator it2 = st.rbegin(); it2 != st.rend(); ++it2) {
				if(*it2 % 2 != 0) {
					st.erase(it2);
				}
			}
		}
	}


} }
