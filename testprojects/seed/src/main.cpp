#include <stdio.h>
//#include <tchar.h>
//#include <conio.h>

#include <iostream>
#include <string>

#include "unittestbase.h"

#include "cpgf/gvariant.h"

//#include "wx/string.h"

using namespace cpgf;


class A
{
public:
	A() {}
	
	operator void * () { return NULL; }
	operator char * () { return NULL; }

private:
	A(int);
};

void test()
{
	A s;
	GVariant v(s);
//	copyVariantFromCopyable(());
}


int main(int /*argc*/, char * /*argv*/[])
{
//	test();
	
	UnitTest::RunAllTests();

//	_getch();
	printf("\n");

	return 0;
}

