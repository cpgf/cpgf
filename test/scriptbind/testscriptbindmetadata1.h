#ifndef CPGF_TESTSCRIPTBINDMETADATA1_H
#define CPGF_TESTSCRIPTBINDMETADATA1_H

#include "../testmetatraits.h"

#include "cpgf/gmetaclass.h"

#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/scriptbind/gscriptwrapper.h"

#include "testscriptbind.h"

#include <string.h>

#include <string>


namespace testscript {

// RawData is not reflected.
// It's used to test non-reflected object in script binding.
struct RawData
{
	int n;
	std::string s;
};

struct TestData
{
	TestData() : x(0), name("") {}
	~TestData() {}

	int x;
	std::string name;
};

class TestBase
{
public:
	virtual ~TestBase() {
	}

	virtual int getValue() const {
		return Magic1;
	}
};

class TestA : public TestBase
{
public:
	virtual int getValue() const {
		return Magic2;
	}
};

class TestB : public TestBase
{
public:
	virtual int getValue() const {
		return Magic3;
	}
};

class TestC : public TestB
{
public:
	virtual int getValue() const {
		return Magic1 + Magic2;
	}
};


class DeriveA
{
public:
	DeriveA() : value(0), a(0) {}

	virtual ~DeriveA() {
	}

	virtual const char * getA() const {
		return "DeriveA";
	}

	virtual void setData(int data) {
		this->a = data;
	}

public:
	int value;
	int a;
};

class DeriveB : virtual public DeriveA
{
public:
	DeriveB() : b(0) {}

	virtual int getB() const {
		return Magic1;
	}
	
	virtual void setData(int data) {
		this->b = data * 2;

		DeriveA::setData(data);
	}

public:
	int dataB[2];
	int b;
};

class DeriveC : virtual public DeriveA
{
public:
	DeriveC() : c(0) {}

	virtual int getC() const {
		return Magic2;
	}

	virtual void setData(int data) {
		this->c = data * 3;

		DeriveA::setData(data);
	}

public:
	int dataC[5];
	int c;
};

class DeriveD : public DeriveB, public DeriveC
{
public:
	DeriveD() : d(0) {}

	virtual int getD() const {
		return Magic3;
	}

	virtual void setData(int data) {
		this->d = data * 4;

		DeriveB::setData(data);
		DeriveC::setData(data);
	}

public:
	int dataD[10];
	int d;
};

class DeriveE : public DeriveD
{
public:
	DeriveE() : e(0) {}

	virtual int getE() const {
		return Magic1 + Magic2 + Magic3;
	}

	virtual const char * getA() const {
		return "DeriveE";
	}

	virtual int getB() const {
		return Magic1 + 1;
	}

	virtual int getC() const {
		return Magic2 + 2;
	}

	virtual int getD() const {
		return Magic3 + 3;
	}

	static DeriveA * pretendA() {
		return new DeriveE;
	}

	virtual void setData(int data) {
		this->e = data * 5;

		DeriveD::setData(data);
	}

public:
	int e;
};



}


#endif
