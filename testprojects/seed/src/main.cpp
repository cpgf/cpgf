#include <stdio.h>
//#include <tchar.h>
//#include <conio.h>

#include <iostream>
#include <string>

#include "unittestbase.h"

#include "cpgf/gvariant.h"
#include "cpgf/gmetadefine.h"

//#include "wx/string.h"

using namespace cpgf;


class A
{
public:
	A() {}
	A(void *) {}
	A(char *) {}
	
	operator void * () { return NULL; }
	operator char * () { return NULL; }

private:
	A & operator = (int);
	explicit A(int);
};

void abc(const A &)
{
}

void test()
{
	A s;
	GVariant v(s);
//	copyVariantFromCopyable(());

	GDefineMetaGlobal define;
	define
		._method("abc", &abc, GMetaPolicyCopyAllConstReference())
	;
}


int main(int /*argc*/, char * /*argv*/[])
{
	test();
	
	UnitTest::RunAllTests();

//	_getch();
	printf("\n");

	return 0;
}

