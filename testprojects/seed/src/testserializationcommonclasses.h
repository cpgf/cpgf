#ifndef __TESTSERIALIZATIONCOMMONCLASSES_H
#define __TESTSERIALIZATIONCOMMONCLASSES_H

#include "testserializationcommon.h"

#include "cpgf/gmetadefine.h"

#include <math.h>
#include <stdio.h>

#include <string>

class TestSerializeClass
{
public:
	TestSerializeClass() : pstr(NULL)
	{
		this->reset();
	}

	~TestSerializeClass()
	{
		this->reset();
	}

	bool operator == (const TestSerializeClass & other) const {
		return this->b == other.b
			&& this->c == other.c
			&& this->wc == other.wc
			&& this->sc == other.sc
			&& this->uc == other.uc
			&& this->si == other.si
			&& this->usi == other.usi
			&& this->i == other.i
			&& this->ui == other.ui
			&& this->l == other.l
			&& this->ul == other.ul
			&& this->ll == other.ll
			&& this->ull == other.ull
			&& FEQUAL(this->f, other.f)
			&& FEQUAL(this->df, other.df)
			&& FEQUAL(this->ldf, other.ldf)
			&& this->str == other.str
			&& (this->pstr == other.pstr || (this->pstr != NULL && other.pstr != NULL && *(this->pstr) == *(other.pstr)))
			&& (this->pself == &this->str && other.pself == &other.str)
			&& (this->pnull == NULL && other.pnull == NULL)
		;
	}

	// we are intended to verify all fields are not equal
	bool operator != (const TestSerializeClass & other) const {
		return this->b != other.b
			&& this->c != other.c
			&& this->wc != other.wc
			&& this->sc != other.sc
			&& this->uc != other.uc
			&& this->si != other.si
			&& this->usi != other.usi
			&& this->i != other.i
			&& this->ui != other.ui
			&& this->l != other.l
			&& this->ul != other.ul
			&& this->ll != other.ll
			&& this->ull != other.ull
			&& !FEQUAL(this->f, other.f)
			&& !FEQUAL(this->df, other.df)
			&& !FEQUAL(this->ldf, other.ldf)
			&& this->str != other.str
			&& !(this->pstr == other.pstr || (this->pstr != NULL && other.pstr != NULL && *(this->pstr) == *(other.pstr)))
			&& (this->pself == &this->str && other.pself == &other.str)
			&& (this->pnull == NULL && other.pnull == NULL)
		;
	}

	void reset()
	{
		this->b = false;
		this->c = 0;
		this->wc = 0;
		this->sc = 0;
		this->uc = 0;
		this->si = 0;
		this->usi = 0;
		this->i = 0;
		this->ui = 0;
		this->l = 0;
		this->ul = 0;
		this->ll = 0;
		this->ull = 0;
		this->f = 0;
		this->df = 0;
		this->ldf = 0;
		this->str = "";
		if(this->pstr != NULL) {
			delete this->pstr;
		}
		this->pstr = NULL;
		this->pself = &this->str;
		this->pnull = NULL;
	}

	void set(long long seed)
	{
		this->b = true;
		this->c = (char)(seed);
		this->wc = (wchar_t)(seed + 20);
		this->sc = -(signed char)(seed + 1);
		this->uc = (unsigned char)(seed + 2);
		this->si = (short)(seed + 3);
		this->usi = (unsigned short)(seed + 5);
		this->i = -(int)(seed + 10);
		this->ui = (unsigned int)(seed + 23);
		this->l = (long)(seed + 12345);
		this->ul = (unsigned long)(seed + 712345);
		this->ll = (long long)(seed + 1234578);
		this->ull = (unsigned long long)(seed + 12345789);
		this->f = (float)(seed * 1.2356);
		this->df = (double)(seed * 17.3);
		this->ldf = (long double)(seed * 109.58);

		char buffer[100];
		sprintf(buffer, "%d", (int)seed);
		std::string s = "StlString:";
		this->str = s + buffer;
		s = "PtrStlString:";
		this->setPstr(s + buffer);
	}

	void setPstr(const std::string & s)
	{
		if(this->pstr == NULL) {
			this->pstr = new std::string;
		}
		*(this->pstr) = s;
	}

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



#endif

