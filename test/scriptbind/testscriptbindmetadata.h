#ifndef __TESTSCRIPTBINDMETADATA_H
#define __TESTSCRIPTBINDMETADATA_H


#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/scriptbind/gscriptbindutil.h"


#include <string.h>

#include <string>


#define REG_NAME_TestObject "testscript::TestObject"
#define REG_NAME_TestData "testscript::TestData"
#define REG_NAME_TestEnum "testscript::TestEnum"
#define REG_NAME_BasicA "testscript::BasicA"


namespace testscript {

const int Magic1 = 0x1999;
const int Magic2 = 0xbeef38;
const int Magic3 = 0xf00d;

void bindBasicData(cpgf::GScriptObject * script, cpgf::IMetaService * service);
void bindBasicData(cpgf::IScriptObject * script, cpgf::IMetaService * service);

void scriptAssert(bool b);

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




class TestObject
{
public:
	TestObject() : value(Magic1) {
		this->rawPointer = &this->raw;
	}
	
	TestObject(const TestObject & other) : value(other.value) {
		(void)other;
		
		this->rawPointer = &this->raw;
	}

	TestObject(int value) : value(value) {
		this->rawPointer = &this->raw;
	}
	
	TestObject(int a, const std::string & s) : value(static_cast<int>(a + s.length())) {
		this->rawPointer = &this->raw;
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

	static int incStaticValue() {
		++staticValue;
		return staticValue;
	}

	RawData * pointerRaw() {
		return &this->raw;
	}
	
	RawData & refRaw() {
		return this->raw;
	}

	bool isRawPointer(const RawData * p) const {
		return p == &this->raw;
	}
	
	bool isRawRef(const RawData & r) const {
		return &r == &this->raw;
	}
	
	void setRaw(RawData * p, int n, const char * s) {
		p->n = n;
		p->s = s;
	}

	TestData & refData() {
		return this->data;
	}
	
	TestData * pointerData() {
		return &this->data;
	}
	
	const TestData & constRefData() {
		return this->data;
	}
	
	const TestData * constPointerData() {
		return &this->data;
	}
	
public:
	int	value;
	RawData raw;
	RawData * rawPointer;
	TestData data;

	static int staticValue;
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

class DeriveA
{
public:
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


class BasicA
{
public:
	enum BasicEnum {
		a = 1, b = 2, c = 3
	};

public:
	class Inner {
	public:
		Inner() : x(5) {
		}
		
		int add() {
			++x;
			return x;
		}
		
	public:
		int x;

	};

public:
};

enum TestEnum {
	teCpp = 1, teLua = 2, teV8 = 3
};

const char * const testString = "TestingScript!";
const int testInt = 1978;
const int testObjValue = 2012;

inline int testAdd2(int a, int b) {
	return 2 + a + b;
}

inline int testAddN(const cpgf::GMetaVariadicParam * params) {
	int total = 3;
	for(size_t i = 0; i < params->paramCount; ++i) {
		total += cpgf::fromVariant<int>(*(params->params[i]));
	}

	return total;
}


}


#endif
