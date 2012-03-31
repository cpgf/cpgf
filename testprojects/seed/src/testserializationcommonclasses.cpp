#include "testserializationcommonclasses.h"

#include <stdio.h>

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
