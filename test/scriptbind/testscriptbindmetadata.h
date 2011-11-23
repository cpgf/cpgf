#ifndef __TESTSCRIPTBINDMETADATA_H
#define __TESTSCRIPTBINDMETADATA_H


#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptbindapi.h"


#include <string.h>

#include <string>


namespace testscript {

const int Magic1 = 0x1999;
const int Magic2 = 0xbeef38;
const int Magic3 = 0xf00d;

struct TestData
{
	int x;
	std::string name;
};

class TestBase
{
public:
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




class TestObject
{
public:
	TestObject() : value(Magic1) {
	}
	
	TestObject(const TestObject & other) : value(other.value) {
		(void)other;
	}

	TestObject(int value) : value(value) {
	}
	
	TestObject(int a, const std::string & s) : value(static_cast<int>(a + s.length())) {
	}
	
	TestObject & operator = (const TestObject & other) {
		(void)other;

		this->value = Magic3;

		return *this;
	}
	
	TestObject * self() {
		return this;
	}
	
	const TestObject * selfConst() {
		return this;
	}
	
	volatile TestObject * selfVolatile() {
		return this;
	}
	
	const volatile TestObject * selfConstVolatile() {
		return this;
	}
	
	int methodConst() {
		return 0;
	}
	
	int methodConst() const {
		return 1;
	}
	
	int methodVolatile() {
		return 0;
	}
	
	int methodVolatile() volatile {
		return 1;
	}
	
	int methodConstVolatile() {
		return 0;
	}
	
	int methodConstVolatile() const volatile {
		return 1;
	}
	
	int methodOverload(const TestObject & obj, int n) const {
		return obj.value + n;
	}
	
	int methodOverload(int n, const TestObject & obj) const {
		return obj.value + n + 1;
	}
	
	int methodOverload(int a, int b) const {
		return a * b;
	}
	
	int methodOverload(const char * s, int n) const {
		return static_cast<int>(strlen(s) + n);
	}
	
	int methodOverload(int n, const char * s) const {
		return static_cast<int>(strlen(s) + n + 1);
	}
	
	int methodOverload(const std::string & s, int n) const {
		return static_cast<int>(s.length() * n);
	}
	
	int methodOverload(int n, const std::string & s) const {
		return static_cast<int>(s.length() * n + 1);
	}
	
	int methodOverloadObject(const TestBase * obj) const {
		(void)obj;

		return TestBase().getValue();
	}
	
	int methodOverloadObject(const TestA * obj) const {
		(void)obj;

		return TestA().getValue();
	}
	
	int methodOverloadObject(const TestB * obj) const {
		(void)obj;

		return TestB().getValue();
	}
	
	int methodOverloadObject(const TestC * obj) const {
		(void)obj;

		return TestC().getValue();
	}

	int add(int n) const {
		return this->value + n;
	}
	
public:
	int	value;
};


class TestOperator
{
public:
	TestOperator() : value(0) {
	}
	
	TestOperator(int value) : value(value) {
	}
	
	TestOperator(const TestOperator & other) : value(other.value) {
	}

#define M(OP, RET) \
	RET operator OP (int other) const {	return this->value OP other; } \
	RET operator OP (const TestOperator & other) const { return this->value OP other.value; } \
	RET operator OP (const TestObject & other) const { return this->value OP other.value; }
	
	M(+, TestOperator)
	M(-, TestOperator)
	M(*, TestOperator)
	M(/, TestOperator)
	M(%, TestOperator)

	M(==, bool)
	M(<, bool)
	M(<=, bool)

#undef M

	TestOperator operator - () const {
		return -this->value;
	}

	TestOperator operator () (const std::string & s, int n) const {
		return this->value + (int)s.length() + n * 2;
	}

	int operator () (const cpgf::GMetaVariadicParam * params) const {
		int total = this->value;
		for(size_t i = 0; i < params->paramCount; ++i) {
			total += cpgf::fromVariant<int>(*(params->params[i]));
		}

		return total;
	}

public:
	int value;
};


void bindBasicData(cpgf::GScriptObject * script, cpgf::IMetaService * service);
void bindBasicData(cpgf::IScriptObject * script, cpgf::IMetaService * service);

void scriptAssert(bool b);



}


#endif
