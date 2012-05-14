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

//#include "boost/type_traits/is_convertible.hpp"
#include "SFML/Graphics.hpp"

//#include "wx/string.h"

using namespace std;
using namespace cpgf;


class A
{
public:
	A() {}
	A(char) {}
	A(wchar_t *) {}
	
	operator void * () { return NULL; }
	operator char * () { return NULL; }

private:
	A & operator = (int);
//	explicit A(int);
};

class B
{
protected:
	B() {}
	
private:
	B(const B &);
	B & operator = (const B &);
};

class C : public B
{
};

B& operator >>(B& Stream, B& Address)
{
	return Stream;
}

std::istream & abc(std::istream & a)
{
//	cout << IsConvertible<char *, sf::RenderWindow &>::Result;
//	cout << boost::is_convertible<char *, sf::RenderWindow &>::value;
	return a;
}

void test()
{
	sf::Font::GetDefaultFont();

	A s;
	GVariant v(s);
//	copyVariantFromCopyable(());

	GDefineMetaGlobal define;
	define
		._method("abc", &abc) //, MakePolicy<GMetaRuleParamNoncopyable<-1>, GMetaRuleParamNoncopyable<0> >())
    	._operator<B & (*)(B &, B &)>(mopHolder >> mopHolder, MakePolicy<GMetaRuleParamNoncopyable<-1>, GMetaRuleParamNoncopyable<0>, GMetaRuleParamNoncopyable<1> >())
	;
}


void samplemain();
int main(int /*argc*/, char * /*argv*/[])
{
	samplemain();

	test();
	
	UnitTest::RunAllTests();

//	_getch();
	printf("\n");

	return 0;
}

