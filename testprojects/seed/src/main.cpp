#include <stdio.h>
//#include <tchar.h>
//#include <conio.h>

#include <iostream>
#include <string>

#include <time.h>

using namespace std;

namespace NSA {
struct MyClass {};
}

void test(int, NSA::MyClass *) {
	cout << "MyClass function" << endl;
}

namespace NSB {
void test(int, ...) {
	cout << "General function" << endl;
}
}

struct MyClass2 : public NSA::MyClass {};

namespace NSC {
void doIt()
{
	using namespace NSB;
	
	MyClass2 * b = 0;
	
	test(0, b);
}
}

void doTest()
{
	NSC::doIt();
};

void testSpidermonkey();
int main(int /*argc*/, char * /*argv*/[])
{
//	doTest();

	testSpidermonkey();
	
//	_getch();
	printf("\n");

	return 0;
}

