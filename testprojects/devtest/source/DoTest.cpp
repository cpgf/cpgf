#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <list>

#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <time.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "cpgf/game/gcore.h"
#include "cpgf/game/gentity.h"
#include "cpgf/game/gcomponent.h"
#include "cpgf/game/gmath.h"
#include "cpgf/game/gsubsystem.h"

#include "cpgf/gcontainer.h"
#include "cpgf/gcallback.h"
#include "cpgf/gpp.h"

using namespace std;

using namespace cpgf;

void doTestGameSource();

void TestTrace(const char *msg, ...)
{
	char buffer[2058];
	va_list args;
	va_start(args, msg);
	vsprintf(buffer, msg, args);
	int len = strlen(buffer);
	buffer[len++] = '\n';
	buffer[len++] = 0;
	OutputDebugStringA(buffer);
	va_end (args);
}

void TestAssert(bool b, const char *msg, ...)
{
	if(b) {
		return;
	}

	char buffer[2058];
	va_list args;
	va_start(args, msg);
	vsprintf(buffer, msg, args);
	debugTrace(buffer);
	va_end (args);

	assert(b);
}


template <typename T>
void doTestObject()
{
	T *obj = new T;
	obj->doTest();
	delete obj;
}


class TestMatrix
{
public:
	void doTest() {
	}
};


class TestQuadNode
{
private:
	class TestGQuadNode : public GWiseQuadNode<int>
	{
	public:
		TestGQuadNode(int n) : GWiseQuadNode<int>(n) {
		}
	};

	void testNodeCallback(TestGQuadNode *node) {
		if(node->value < 100 && node->value % 2 == 0) {
//			node->setParentNode(NULL);
//			delete node;

			return;
		}

		char s[10];
		sprintf(s, "%d", node->value);
		debugTrace(s);
	}

	typedef TestGQuadNode * NodePtr;
	void fillNode(TestGQuadNode *root) {
		const int count = 10;
		NodePtr *nodeList = new NodePtr[count];

		int i;

		for(i = 0; i < count; ++i) {
			nodeList[i] = new TestGQuadNode(i);
			nodeList[i]->setParentNode(root);
		}

		for(i = 0; i < count / 2; ++i) {
			nodeList[i + count / 2]->setParentNode(nodeList[i]);
		}

		if(count > 200) {
			nodeList[1]->insertNodeAfterSibling(nodeList[2]);
			nodeList[3]->setParentNode(nodeList[0]);
			nodeList[1]->insertNodeBeforeSibling(nodeList[0]);
			nodeList[0]->setParentNode(nodeList[4]);
		}

		delete[] nodeList;
	}

	void enumNode(TestGQuadNode *node) {
		for(TestGQuadNode::iterator it = node->begin(); it != node->end(); ++it) {
			TestGQuadNode *n = static_cast<TestGQuadNode *>(it.node);
			testNodeCallback(n);

			enumNode(n);
		}
	}

public:
	void doTest()
	{

		TestGQuadNode *root = new TestGQuadNode(100);
		fillNode(root);

		debugTrace("forward");
		enumNode(root);

		root->setParentNode(NULL);
	}

};

#define TestList_Type GWiseList
//#define TestList_Type list

void intXXX(int a) {}

class TestLoopSafeList
{
private:
	typedef TestList_Type<int> ListType;
	typedef ListType::iterator IteratorType;
	typedef ListType::reverse_iterator ReverseIteratorType;

public:
	ListType st;

	typedef GWiseList<int>::iterator WiseIterator;
	typedef GWiseList<int>::reverse_iterator WiseReverseIterator;

	void testMore()
	{
		GWiseList<int> wise;
		int i;

		for(i = 0; i < 1; ++i) {
			wise.push_back(i);
		}

		for(WiseIterator it1 = wise.begin(); it1 != wise.end(); ++it1) {
			if(*it1 % 3 == 0) {
				it1 = wise.erase(it1);
			}
			for(WiseReverseIterator it2 = wise.rbegin(); it2 != wise.rend(); ++it2) {
				int n2 = *it2;
				if(n2 % 5 == 0) {
//					wise.erase(--(it2.base()));
					wise.erase(it2);
				}
				for(WiseIterator it3 = wise.begin(); it3 != wise.end(); ++it3) {
					if(*it3 % 6 == 0) {
//						wise.erase(it3);
					}
				}
			}
		}
	}

	void doTest()
	{
		testMore();

		IteratorType it;
		int i;
		const int itemCount = 8;

		for(i = 0; i < itemCount; ++i) {
			st.push_back(i);
		}

		for(ReverseIteratorType rit = st.rbegin(); rit != st.rend(); ++rit) {
			TestTrace("List reverse value  %d", *rit);
		}

		it = st.begin();
//		for(it = st.begin(); it != st.end(); ++it)
		for(i = 0; i < itemCount - 1; ++i)
		{
			TestTrace("List value  %d", *it);
			if(*it == 3) {
				st.insertAfter(it, 10);
			}
			if(*it == 4) {
				st.insertBefore(it, 11);
			}
//			st.erase(it);
//			st.erase(skip(it, 1, true));
			++it;
		}

//		st.clear();

		for(it = st.begin(); it != st.end(); ++it)
		{
			TestTrace("222 List value  %d", *it);
		}

		TestTrace("List size %d", st.size());
		st.clear();
		TestTrace("List size %d", st.size());

		std::for_each(st.begin(), st.end(), intXXX);
	}

private:
	IteratorType skip(IteratorType it, int steps, bool forward) {
		while(steps > 0) {
			steps--;

			if(forward)
				++it;
			else
				it--;
		}

		return it;
	}
};

class TestSkipList
{
private:
	typedef GWiseSkipList<int> ListType;
	typedef ListType::iterator IteratorType;

public:
	ListType st;

	void doTest() {
		const int itemValues[] = {
			9, 2, 5, 1, 7, 8, 6, 2, 8, 7
		};

		int i;
		IteratorType it;

		for(i = 0; i < sizeOfArray(itemValues); ++i) {
			st.insert(itemValues[i], false);
		}

		for(IteratorType it = st.begin(); it != st.end(); ++it) {
			TestTrace("Skip List value  %d", *it);

			if(*it % 2 != 0) {
				st.erase(it);
			}
		}

		for(it = st.begin(); it != st.end(); ++it) {
			TestTrace("222 Skip List value  %d", *it);
		}
		st.contains(2);

		TestTrace("Skip list size %d", st.size());
		st.clear();
		TestTrace("Skip list size %d", st.size());
	}
};


class TestMap
{
public:
	template<typename MapType>
	void doTestMap() {
		MapType m;
		int i;
		const int Count = 100000 * 0;

		for(i = 0; i < Count; ++i) {
			m[i] = true;
		}

		for(i = 0; i < Count; ++i) {
			m[i] = true;
		}
	}

//	typedef GCallbackList<void, void *> CallbackListType;
	typedef GWiseMap<int, GWiseList<int> > XMapType;
	void doTest() {
		XMapType aaa;

		clock_t startTime;
		int i;
		int size;
		const int Count = 1000000 * 1 / 1000;
		const int magic = 1999;

		GWiseMap<int, int> m1(10, 30);
		std::map<int, int> m2;

		startTime = clock();
		for(i = Count - 1; i >= 0; i--) {
			m1.insert(i, 0);
		}
		size = m1.size();
		for(i = 0; i < Count; ++i) {
			m1[i] = magic;
		}
		for(i = 0; i < Count; ++i) {
			m1.find(i);
			m1[i] = i + 500;
			m1.remove(i);
		}
		TestTrace("Map GWiseMap time %d   size %d", (int)(clock() - startTime), size);

		startTime = clock();
		for(i = Count - 1; i >= 0; i--) {
			m2.insert(std::pair<int, int>(i, 0));
		}
		size = m2.size();
		for(i = 0; i < Count; ++i) {
			m2[i] = magic;
		}
		for(i = 0; i < Count; ++i) {
			m2.find(i);
			m1[i] = i + 500;
			m2.erase(i);
		}
		TestTrace("Map std::map time %d  size %d", (int)(clock() - startTime), size);
	}
};


#define TraceObjSize(cls) TestTrace("Object size of " #cls " is %d", sizeof(cls))

void doTestMisc()
{
	TraceObjSize(std::list<int>);
	TraceObjSize(std::vector<int>);
	TraceObjSize(std::set<int>);

	TraceObjSize(GObject);
	TraceObjSize(GComponent);
	TraceObjSize(GEntity);
	TraceObjSize(GMatrix4);
	TraceObjSize(GVector2);

	TraceObjSize(GWiseList<int>);
	TraceObjSize(GWiseSkipList<int>);
	TraceObjSize(GWiseMap<int GPP_COMMA() int>);
	TraceObjSize(GQuadNode<int>);
	TraceObjSize(GWiseQuadNode<int>);
}

void doTestGameSource()
{
	doTestMisc();

	doTestObject<TestQuadNode>();
	doTestObject<TestMatrix>();
	doTestObject<TestLoopSafeList>();
	doTestObject<TestSkipList>();
	doTestObject<TestMap>();
}
