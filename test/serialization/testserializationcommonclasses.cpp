#include "testserializationcommonclasses.h"

#include <stdio.h>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif

TestSerializeClass::TestSerializeClass()
	: pstr(NULL)
{
	this->reset();
}

TestSerializeClass::~TestSerializeClass()
{
	this->reset();
}

bool TestSerializeClass::operator == (const TestSerializeClass & other) const
{
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
bool TestSerializeClass::operator != (const TestSerializeClass & other) const
{
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

void TestSerializeClass::reset()
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

void TestSerializeClass::set(long long seed)
{
	this->b = true;
	initTestValue(this->c, seed);
	initTestValue(this->wc, seed + 20);
	initTestValue(this->sc, seed + 1);
	initTestValue(this->uc, seed + 2);
	this->si = (short)(seed + 3);
	this->usi = (unsigned short)(seed + 5);
	this->i = -(int)(seed + 10);
	this->ui = (unsigned int)(seed + 23);
	this->l = (long)(seed + 12345);
	this->ul = (unsigned long)(seed + 712345);
	this->ll = (long long)(seed + 1234578);
	this->ull = (unsigned long long)(seed + 12345789);
	initTestValue(this->f, seed );
	initTestValue(this->df, seed);
	initTestValue(this->ldf, seed);

	char buffer[100];
	sprintf(buffer, "%d", (int)seed);
	std::string s = "StlString:";
	this->str = s + buffer;
	s = "PtrStlString:";
	this->setPstr(s + buffer);
}

void TestSerializeClass::setPstr(const std::string & s)
{
	if(this->pstr == NULL) {
		this->pstr = new std::string;
	}
	*(this->pstr) = s;
}

void initTestValue(TestSerializeClass & value, long long seed)
{
	value.set(seed);
}


#define LOOP2(d1, d2) for(int z1 = 0; z1 < d1; ++z1) for(int z2 = 0; z2 < d2; ++z2)
#define LOOP3(d1, d2, d3) for(int z1 = 0; z1 < d1; ++z1) for(int z2 = 0; z2 < d2; ++z2) for(int z3 = 0; z3 < d3; ++z3)

TestSerializeArray::TestSerializeArray()
{
	LOOP3(B1, B2, B3) npo[z1][z2][z3] = NULL;

	this->reset();
}

TestSerializeArray::~TestSerializeArray()
{
	this->reset();
}

bool TestSerializeArray::operator == (const TestSerializeArray & other) const
{
#define EQ2(v, d1, d2) LOOP2(d1, d2) if(v[z1][z2] != other. v[z1][z2]) return false;
#define EQ3(v, d1, d2, d3) LOOP3(d1, d2, d3) if(v[z1][z2][z3] != other. v[z1][z2][z3]) return false;

	EQ2(i, A1, A2)
	EQ3(s, B1, B2, B3)
	EQ3(o, B1, B2, B3)
	
#undef EQ2
#undef EQ3

	LOOP3(B1, B2, B3) if(z1 && z2 && z3 && ps[z1][z2][z3] != &s[z1][z2][z3]) return false;
	LOOP3(B1, B2, B3) if(z1 && z2 && z3 && other.ps[z1][z2][z3] != &other.s[z1][z2][z3]) return false;
	
	LOOP3(B1, B2, B3) if(z1 && z2 && z3 && po[z1][z2][z3] != &o[z1][z2][z3]) return false;
	LOOP3(B1, B2, B3) if(z1 && z2 && z3 && other.po[z1][z2][z3] != &other.o[z1][z2][z3]) return false;
	
	if(ps[0][0][0] != NULL) return false;
	if(other.ps[0][0][0] != NULL) return false;
	if(po[0][0][0] != NULL) return false;
	if(other.po[0][0][0] != NULL) return false;
	
	LOOP3(B1, B2, B3) if(*npo[z1][z2][z3] != *other.npo[z1][z2][z3]) return false;

	return true;
}

void TestSerializeArray::reset()
{
#define INIT2(v, d1, d2) LOOP2(d1, d2) initTestValue(v[z1][z2], getTestSeed(0));
#define INIT3(v, d1, d2, d3) LOOP3(d1, d2, d3) initTestValue(v[z1][z2][z3], getTestSeed(0));
	
	INIT2(i, A1, A2)
	INIT3(s, B1, B2, B3)
	INIT3(o, B1, B2, B3)

#undef INIT2
#undef INIT3

	LOOP3(B1, B2, B3) ps[z1][z2][z3] = NULL;
	LOOP3(B1, B2, B3) po[z1][z2][z3] = NULL;
	
	LOOP3(B1, B2, B3) {
		delete npo[z1][z2][z3];
		npo[z1][z2][z3] = NULL;
	}
}

void TestSerializeArray::init()
{
#define INIT2(v, d1, d2) LOOP2(d1, d2) initTestValue(v[z1][z2], getTestSeed(z1 * d2 + d2 + 1));
#define INIT3(v, d1, d2, d3) LOOP3(d1, d2, d3) initTestValue(v[z1][z2][z3], getTestSeed(z1 * d2 * d3 + z2 * d3 + z3 + 1));
	
	INIT2(i, A1, A2)
	INIT3(s, B1, B2, B3)
	INIT3(o, B1, B2, B3)

#undef INIT2
#undef INIT3

	LOOP3(B1, B2, B3) ps[z1][z2][z3] = &s[z1][z2][z3];
	LOOP3(B1, B2, B3) po[z1][z2][z3] = &o[z1][z2][z3];

	ps[0][0][0] = NULL;
	po[0][0][0] = NULL;

	LOOP3(B1, B2, B3) {
		npo[z1][z2][z3] = new TestSerializeClass();
		initTestValue(*npo[z1][z2][z3], getTestSeed(z1 * B2 * B3 + z2 * B3 + z3 + 1));
	}
}

