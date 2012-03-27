#ifndef __TESTSERIALIZATIONCOMMONCLASSES_H
#define __TESTSERIALIZATIONCOMMONCLASSES_H

#include "testserializationcommon.h"

#include "cpgf/gmetadefine.h"

#include <string>

class TestSerializeClass
{
public:
	TestSerializeClass();
	~TestSerializeClass();

	bool operator == (const TestSerializeClass & other) const;
	bool operator != (const TestSerializeClass & other) const;

	void reset();
	void set(long long seed);
	void setPstr(const std::string & s);

public:
	bool b;
	char c;
	wchar_t wc;
	signed char sc;
	unsigned char uc;
	short si;
	unsigned short usi;
	int i;
	unsigned int ui;
	long l;
	unsigned long ul;
	long long ll;
	unsigned long long ull;
	float f;
	double df;
	long double ldf;
	std::string str;
	std::string * pstr;
	std::string * pself;
	std::string * pnull;
};

#define F(f) FIELD(TestSerializeClass, f)

template <typename D>
void register_TestSerializeClass(D define)
{
	cpgf::GDefineMetaClass<TestSerializeClass> classDefine = cpgf::GDefineMetaClass<TestSerializeClass>::declare("TestSerializeClass");
	
	classDefine
		F(b)
		F(c)
		F(wc)
		F(sc)
		F(uc)
		F(si)
		F(usi)
		F(i)
		F(ui)
		F(l)
		F(ul)
		F(ll)
		F(ull)
		F(f)
		F(df)
		F(ldf)
		F(str)
		F(pstr)
		F(pself)
		F(pnull)
	;

	define._class(classDefine);
}

#undef F


void initTestValue(TestSerializeClass & value, long long seed);

#endif

