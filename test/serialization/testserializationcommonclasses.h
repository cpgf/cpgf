#ifndef CPGF_TESTSERIALIZATIONCOMMONCLASSES_H
#define CPGF_TESTSERIALIZATIONCOMMONCLASSES_H

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

	// not serializable
	void ** ppvoid;
	std::string ** ppstring;
};

class TestSerializeArray
{
private:
	enum {
		A1 = 3, A2 = 5,
		B1 = 5, B2 = 3, B3 = 2,
	};

public:
	TestSerializeArray();
	~TestSerializeArray();

	bool operator == (const TestSerializeArray & other) const;

	void reset();
	void init();

public:
	int i[A1][A2];
	std::string s[B1][B2][B3];
	std::string * ps[B1][B2][B3];
	TestSerializeClass o[B1][B2][B3];
	TestSerializeClass * po[B1][B2][B3];
	TestSerializeClass * npo[B1][B2][B3];
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

		F(ppvoid)
		F(ppstring)
	;

	define._class(classDefine);
}

#undef F

template <typename D>
void register_TestSerializeArray(D define)
{
	cpgf::GDefineMetaClass<TestSerializeArray> classDefine = cpgf::GDefineMetaClass<TestSerializeArray>::declare("TestSerializeArray");
	
	classDefine
		FIELD(TestSerializeArray, i)
		FIELD(TestSerializeArray, s)
		FIELD(TestSerializeArray, ps)
		FIELD(TestSerializeArray, o)
		FIELD(TestSerializeArray, po)
		FIELD(TestSerializeArray, npo)
	;

	define._class(classDefine);
}


void initTestValue(TestSerializeClass & value, long long seed);

#endif

