#include "test_reflection_common.h"

#define CLASS TestClass_Operator
#define NAME_CLASS GPP_STRINGIZE(CLASS)

#define CLASS2 TestClass_Operator_Extra
#define NAME_CLASS2 GPP_STRINGIZE(CLASS2)

#define OP(operand) pointerAssign(op, getOperator(operand, metaClass, 0))
#define OPI(operand, i) pointerAssign(op, getOperator(operand, metaClass, i))

#define OP2(operand) pointerAssign(op, getOperator(operand, metaClass2, 0))
#define OPI2(operand, i) pointerAssign(op, getOperator(operand, metaClass2, i))

using namespace std;
using namespace cpgf;

namespace Test_Operator { namespace {

class CLASS {
public:
	int value;

public:
	CLASS() : value(0) {
	}

	explicit CLASS(int value) : value(value) {
	}

	CLASS(const CLASS & other) : value(other.value) {
	}

	CLASS & operator = (const CLASS & other) {
		this->value = other.value;

		return *this;
	}

	void reset(int value) {
		this->value = value;
	}

	// arithmetic

	CLASS operator + (int n) const {
		return CLASS(this->value + n);
	}

	CLASS operator - (int n) const {
		return CLASS(this->value - n);
	}

	CLASS operator * (int n) const {
		return CLASS(this->value * n);
	}

	CLASS operator / (int n) const {
		return CLASS(this->value / n);
	}

	CLASS operator % (int n) const {
		return CLASS(this->value % n);
	}

	CLASS operator % (int n) {
		return CLASS(this->value % (n + 1));
	}


	// arithmetic assign

	CLASS & operator += (int n) {
		this->value += n;

		return *this;
	}

	CLASS & operator -= (int n) {
		this->value -= n;

		return *this;
	}

	CLASS & operator *= (int n) {
		this->value *= n;

		return *this;
	}

	CLASS & operator /= (int n) {
		this->value /= n;

		return *this;
	}

	CLASS & operator %= (int n) {
		this->value %= n;

		return *this;
	}

	// bitwise

	CLASS operator & (int n) const {
		return CLASS(this->value & n);
	}

	CLASS operator | (int n) const {
		return CLASS(this->value | n);
	}

	CLASS operator ^ (int n) const {
		return CLASS(this->value ^ n);
	}

	CLASS operator >> (int n) const {
		return CLASS(this->value >> n);
	}

	CLASS operator << (int n) const {
		return CLASS(this->value << n);
	}

	// bitwise assign

	CLASS & operator &= (int n) {
		this->value &= n;

		return *this;
	}

	CLASS & operator |= (int n) {
		this->value |= n;

		return *this;
	}

	CLASS & operator ^= (int n) {
		this->value ^= n;

		return *this;
	}

	CLASS & operator >>= (int n) {
		this->value &= n;

		return *this;
	}

	CLASS & operator <<= (int n) {
		this->value &= n;

		return *this;
	}

	// logic

	bool operator == (const CLASS & other) const {
		return this->value == other.value;
	}

	bool operator != (const CLASS & other) const {
		return this->value != other.value;
	}

	bool operator > (const CLASS & other) const {
		return this->value > other.value;
	}

	bool operator >= (const CLASS & other) const {
		return this->value >= other.value;
	}

	bool operator < (const CLASS & other) const {
		return this->value < other.value;
	}

	bool operator <= (const CLASS & other) const {
		return this->value <= other.value;
	}

	bool operator && (const CLASS & other) const {
		return this->value && other.value;
	}

	bool operator || (const CLASS & other) const {
		return this->value || other.value;
	}

	// unary

	CLASS operator + () const {
		return CLASS(this->value < 0 ? -this->value : this->value);
	}

	CLASS operator - () const {
		return CLASS(this->value < 0 ? this->value : -this->value);
	}

	CLASS operator ++ () {
		return CLASS(++this->value);
	}

	CLASS operator ++ (int) {
		CLASS t = CLASS(this->value);
		++this->value;
		return t;
	}

	CLASS operator -- () {
		return CLASS(--this->value);
	}

	CLASS operator -- (int) {
		CLASS t = CLASS(this->value);
		--this->value;
		return t;
	}

	CLASS operator ! () const {
		return CLASS(!this->value);
	}

	CLASS operator ~ () const {
		return CLASS(~this->value);
	}

};

CLASS operator + (int n, const CLASS & obj) {
	return CLASS(n + obj.value * 2);
}


GMETA_DEFINE_CLASS(CLASS, CLASS, NAME_CLASS) {
	// arithmetic

	reflectOperator<CLASS & (GMetaSelf, const CLASS &)>(mopHolder = mopHolder);
	reflectOperator<CLASS (const GMetaSelf &, int)>(mopHolder + mopHolder);
	reflectOperator<CLASS (int, const CLASS &)>(mopHolder + mopHolder);
	reflectOperator<CLASS (const GMetaSelf &, int)>(mopHolder - mopHolder);
	reflectOperator<CLASS (const GMetaSelf &, int)>(mopHolder * mopHolder);
	reflectOperator<CLASS (const GMetaSelf &, int)>(mopHolder / mopHolder);
	reflectOperator<CLASS (const GMetaSelf &, int)>(mopHolder % mopHolder);
	reflectOperator<CLASS (GMetaSelf, int)>(mopHolder % mopHolder);

	// arithmetic assign

	reflectOperator<CLASS (GMetaSelf, int)>(mopHolder += mopHolder);
	reflectOperator<CLASS (GMetaSelf, int)>(mopHolder -= mopHolder);
	reflectOperator<CLASS (GMetaSelf, int)>(mopHolder *= mopHolder);
	reflectOperator<CLASS (GMetaSelf, int)>(mopHolder /= mopHolder);
	reflectOperator<CLASS (GMetaSelf, int)>(mopHolder %= mopHolder);

	// bitwise

	reflectOperator<CLASS (const GMetaSelf &, int)>(mopHolder & mopHolder);
	reflectOperator<CLASS (const GMetaSelf &, int)>(mopHolder | mopHolder);
	reflectOperator<CLASS (const GMetaSelf &, int)>(mopHolder ^ mopHolder);
	reflectOperator<CLASS (const GMetaSelf &, int)>(mopHolder >> mopHolder);
	reflectOperator<CLASS (const GMetaSelf &, int)>(mopHolder << mopHolder);

	// bitwise assign

	reflectOperator<CLASS & (GMetaSelf, int)>(mopHolder &= mopHolder);
	reflectOperator<CLASS & (GMetaSelf, int)>(mopHolder |= mopHolder);
	reflectOperator<CLASS & (GMetaSelf, int)>(mopHolder ^= mopHolder);
	reflectOperator<CLASS & (GMetaSelf, int)>(mopHolder >>= mopHolder);
	reflectOperator<CLASS & (GMetaSelf, int)>(mopHolder <<= mopHolder);

	// logic

	reflectOperator<bool (const GMetaSelf &, const CLASS &)>(mopHolder == mopHolder);
	reflectOperator<bool (const GMetaSelf &, const CLASS &)>(mopHolder != mopHolder);
	reflectOperator<bool (const GMetaSelf &, const CLASS &)>(mopHolder > mopHolder);
	reflectOperator<bool (const GMetaSelf &, const CLASS &)>(mopHolder >= mopHolder);
	reflectOperator<bool (const GMetaSelf &, const CLASS &)>(mopHolder < mopHolder);
	reflectOperator<bool (const GMetaSelf &, const CLASS &)>(mopHolder <= mopHolder);
	reflectOperator<bool (const GMetaSelf &, const CLASS &)>(mopHolder && mopHolder);
	reflectOperator<bool (const GMetaSelf &, const CLASS &)>(mopHolder || mopHolder);

	// unary

	reflectOperator<CLASS (const GMetaSelf &)> (+ mopHolder);
	reflectOperator<CLASS (const GMetaSelf &)> (- mopHolder);
	reflectOperator<CLASS (GMetaSelf)> (++mopHolder);
	reflectOperator<CLASS (GMetaSelf)> (mopHolder++);
	reflectOperator<CLASS (GMetaSelf)> (--mopHolder);
	reflectOperator<CLASS (GMetaSelf)> (mopHolder--);
	reflectOperator<CLASS (const GMetaSelf &)> (! mopHolder);
	reflectOperator<CLASS (const GMetaSelf &)> (~ mopHolder);

}


class CLASS2 {
public:
	int value;

public:
	CLASS2() : value(0) {
	}

	explicit CLASS2(int value) : value(value) {
	}

	CLASS2(const CLASS2 & other) : value(other.value) {
	}

	CLASS2 & operator = (const CLASS2 & other) {
		this->value = other.value;

		return *this;
	}

	void reset(int value) {
		this->value = value;
	}


	// other

	int * operator & () {
		return &this->value;
	}

	int operator * () const {
		return this->value * 2;
	}

	CLASS2 & operator , (int n) {
		this->value += n;

		return *this;
	}

	std::string operator [] (int n) const {
		std::string s = "";

		while(n > 0) {
			--n;
			s = s + "a";
		}

		return s;
	}

	int * operator -> () {
		return &this->value;
	}

	int operator ->* (int CLASS2::* p) {
		return this->*p;
	}

	operator std::string () const {
		return "abc";
	}

	int operator () (const std::string & s, int n) const {
		return this->value + (int)s.length() + n * 2;
	}

	int operator () (const cpgf::GMetaVariadicParam * params) const {
		int total = 0;
		for(size_t i = 0; i < params->paramCount; ++i) {
			total += cpgf::fromVariant<int>(*(params->params[i]));
		}

		return total;
	}

};


GMETA_DEFINE_CLASS(CLASS2, CLASS2, NAME_CLASS2) {
	// other

	reflectOperator<int * (const GMetaSelf &)> (& mopHolder);
	reflectOperator<int (const GMetaSelf &)> (* mopHolder);
	reflectOperator<CLASS2 & (GMetaSelf, int)>((mopHolder , mopHolder));
	reflectOperator<std::string (GMetaSelf, int)>(mopHolder[0]);
	reflectOperator<int * (GMetaSelf)>(mopHolder->mopHolder);
	reflectOperator<int (GMetaSelf, int CLASS2::*)>(mopHolder->*mopHolder);
	reflectOperator<std::string (GMetaSelf)>(mopHolder());
	reflectOperator<int (const std::string &, int)>(mopHolder(mopHolder));
	reflectOperator<int (const cpgf::GMetaVariadicParam *)>(mopHolder(mopHolder));
}

GTEST(Lib_Exists)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);
	
	const GMetaOperator * op;

	OP(mopHolder + mopHolder);
	GCHECK(op);

	OP(mopHolder - mopHolder);
	GCHECK(op);
	
	OP(mopHolder * mopHolder);
	GCHECK(op);
	
	OP(mopHolder / mopHolder);
	GCHECK(op);
	
	OPI(mopHolder % mopHolder, 0);
	GCHECK(op);
	
	OPI(mopHolder % mopHolder, 1);
	GCHECK(op);

	OP(mopHolder += mopHolder);
	GCHECK(op);

	OP(mopHolder -= mopHolder);
	GCHECK(op);
	
	OP(mopHolder *= mopHolder);
	GCHECK(op);
	
	OP(mopHolder /= mopHolder);
	GCHECK(op);
	
	OP(mopHolder %= mopHolder);
	GCHECK(op);

	OP(mopHolder & mopHolder);
	GCHECK(op);

	OP(mopHolder | mopHolder);
	GCHECK(op);
	
	OP(mopHolder ^ mopHolder);
	GCHECK(op);
	
	OP(mopHolder << mopHolder);
	GCHECK(op);
	
	OP(mopHolder >> mopHolder);
	GCHECK(op);

	OP(mopHolder == mopHolder);
	GCHECK(op);

	OP(mopHolder != mopHolder);
	GCHECK(op);
	
	OP(mopHolder > mopHolder);
	GCHECK(op);
	
	OP(mopHolder >= mopHolder);
	GCHECK(op);
	
	OP(mopHolder < mopHolder);
	GCHECK(op);
	
	OP(mopHolder <= mopHolder);
	GCHECK(op);
	
	OP(mopHolder && mopHolder);
	GCHECK(op);
	
	OP(mopHolder || mopHolder);
	GCHECK(op);
	
	OP(+ mopHolder);
	GCHECK(op);
	
	OP(- mopHolder);
	GCHECK(op);
	
	OP(++ mopHolder);
	GCHECK(op);
	
	OP( mopHolder ++);
	GCHECK(op);
	
	OP(-- mopHolder);
	GCHECK(op);
	
	OP(mopHolder --);
	GCHECK(op);
	
	OP(! mopHolder);
	GCHECK(op);
	
	OP(~ mopHolder);
	GCHECK(op);
	
	const GMetaClass * metaClass2 = findMetaClass(NAME_CLASS2);
	GCHECK(metaClass2);
	
	OP2(& mopHolder);
	GCHECK(op);
	
	OP2(* mopHolder);
	GCHECK(op);

	OP2((mopHolder , mopHolder));
	GCHECK(op);

	OP2(mopHolder[0]);
	GCHECK(op);

	OP2(mopHolder -> mopHolder);
	GCHECK(op);

	OP2(mopHolder ->* mopHolder);
	GCHECK(op);

	OPI2(mopHolder(), 0);
	GCHECK(op);

	OPI2(mopHolder(mopHolder), 1);
	GCHECK(op);

	OPI2(mopHolder(), 0);
	GCHECK(op);

	OPI2(mopHolder(mopHolder), 1);
	GCHECK(op);
}


GTEST(API_Exists)
{
	GMetaScopedPointer<IMetaService> service(createMetaService());
	GCHECK(service);

	GMetaScopedPointer<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GMetaScopedPointer<IMetaOperator> op;

}


GTEST(Lib_InvokeArithmetic)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);
	
	const GMetaOperator * op;

	CLASS operand;
	CLASS result;

	operand.reset(5);
	OP(mopHolder + mopHolder);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 6));
	GEQUAL(result.value, 5 + 6);

	OPI(mopHolder + mopHolder, 1);
	result = fromVariant<CLASS>(op->invokeBinary(3, CLASS(7)));
	GEQUAL(result.value, 3 + 7 * 2);

	operand.reset(5);
	OP(mopHolder - mopHolder);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 6));
	GEQUAL(result.value, 5 - 6);

	operand.reset(5);
	OP(mopHolder * mopHolder);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 6));
	GEQUAL(result.value, 5 * 6);

	operand.reset(25);
	OP(mopHolder / mopHolder);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 6));
	GEQUAL(result.value, 25 / 6);

	operand.reset(25);
	OP(mopHolder % mopHolder);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 6));
	GEQUAL(result.value, 25 % 6);

	operand.reset(25);
	OPI(mopHolder % mopHolder, 1);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 6));
	GEQUAL(result.value, 25 % (6 + 1));

}


GTEST(API_InvokeArithmetic)
{
	GMetaScopedPointer<IMetaService> service(createMetaService());
	GCHECK(service);

	GMetaScopedPointer<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GMetaScopedPointer<IMetaOperator> op;

	CLASS operand;
	CLASS result;

	operand.reset(5);
	OP(mopHolder + mopHolder);
	result = fromVariant<CLASS>(metaCallOperatorBinary(op, operand, 6));
	GEQUAL(result.value, 5 + 6);

	OPI(mopHolder + mopHolder, 1);
	result = fromVariant<CLASS>(metaCallOperatorBinary(op, 3, CLASS(7)));
	GEQUAL(result.value, 3 + 7 * 2);

	operand.reset(5);
	OP(mopHolder - mopHolder);
	result = fromVariant<CLASS>(metaCallOperatorBinary(op, operand, 6));
	GEQUAL(result.value, 5 - 6);

	operand.reset(5);
	OP(mopHolder * mopHolder);
	result = fromVariant<CLASS>(metaCallOperatorBinary(op, operand, 6));
	GEQUAL(result.value, 5 * 6);

	operand.reset(25);
	OP(mopHolder / mopHolder);
	result = fromVariant<CLASS>(metaCallOperatorBinary(op, operand, 6));
	GEQUAL(result.value, 25 / 6);

	operand.reset(25);
	OP(mopHolder % mopHolder);
	result = fromVariant<CLASS>(metaCallOperatorBinary(op, operand, 6));
	GEQUAL(result.value, 25 % 6);

	operand.reset(25);
	OPI(mopHolder % mopHolder, 1);
	result = fromVariant<CLASS>(metaCallOperatorBinary(op, operand, 6));
	GEQUAL(result.value, 25 % (6 + 1));

}


GTEST(Lib_InvokeArithmeticAssign)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);
	
	const GMetaOperator * op;

	CLASS operand;
	CLASS * addr = &operand;
	CLASS * presult = addr;
/*
	operand.reset(5);
	OP(mopHolder += mopHolder);
	presult = &(fromVariant<CLASS &>(op->invokeBinary(operand, 6)));
	GEQUAL(presult, addr);
	GEQUAL(presult->value, 5 + 6);

	operand.reset(3);
	OPI(mopHolder + mopHolder, 1);
	result = fromVariant<CLASS>(op->invokeBinary(operand, CLASS(7)));
	GEQUAL(result.value, 3 + 7 * 2);

	operand.reset(5);
	OP(mopHolder - mopHolder);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 6));
	GEQUAL(result.value, 5 - 6);

	operand.reset(5);
	OP(mopHolder * mopHolder);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 6));
	GEQUAL(result.value, 5 * 6);

	operand.reset(25);
	OP(mopHolder / mopHolder);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 6));
	GEQUAL(result.value, 25 / 6);

	operand.reset(25);
	OP(mopHolder % mopHolder);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 6));
	GEQUAL(result.value, 25 % 6);

	operand.reset(25);
	OPI(mopHolder % mopHolder, 1);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 6));
	GEQUAL(result.value, 25 % (6 + 1));
*/
}


GTEST(Lib_InvokeBitwise)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);
	
	const GMetaOperator * op;

	CLASS operand;
	CLASS result;

	operand.reset(0x57);
	OP(mopHolder & mopHolder);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 0x3f));
	GEQUAL(result.value, (0x57 & 0x3f));

	operand.reset(0x57);
	OP(mopHolder | mopHolder);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 0x3f));
	GEQUAL(result.value, 0x57 | 0x3f);

	operand.reset(0x57);
	OP(mopHolder ^ mopHolder);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 0x3f));
	GEQUAL(result.value, 0x57 ^ 0x3f);

	operand.reset(0x57);
	OP(mopHolder << mopHolder);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 3));
	GEQUAL(result.value, 0x57 << 3);

	operand.reset(0x57);
	OP(mopHolder >> mopHolder);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 3));
	GEQUAL(result.value, 0x57 >> 3);

}


GTEST(API_InvokeBitwise)
{
	GMetaScopedPointer<IMetaService> service(createMetaService());
	GCHECK(service);

	GMetaScopedPointer<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GMetaScopedPointer<IMetaOperator> op;

	CLASS operand;
	CLASS result;

	operand.reset(0x57);
	OP(mopHolder & mopHolder);
	result = fromVariant<CLASS>(metaCallOperatorBinary(op, operand, 0x3f));
	GEQUAL(result.value, (0x57 & 0x3f));

	operand.reset(0x57);
	OP(mopHolder | mopHolder);
	result = fromVariant<CLASS>(metaCallOperatorBinary(op, operand, 0x3f));
	GEQUAL(result.value, 0x57 | 0x3f);

	operand.reset(0x57);
	OP(mopHolder ^ mopHolder);
	result = fromVariant<CLASS>(metaCallOperatorBinary(op, operand, 0x3f));
	GEQUAL(result.value, 0x57 ^ 0x3f);

	operand.reset(0x57);
	OP(mopHolder << mopHolder);
	result = fromVariant<CLASS>(metaCallOperatorBinary(op, operand, 3));
	GEQUAL(result.value, 0x57 << 3);

	operand.reset(0x57);
	OP(mopHolder >> mopHolder);
	result = fromVariant<CLASS>(metaCallOperatorBinary(op, operand, 3));
	GEQUAL(result.value, 0x57 >> 3);

}


GTEST(Lib_InvokeBitwiseAssign)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);
	
	const GMetaOperator * op;

	CLASS operand;
	CLASS result;

	operand.reset(0x57);
	OP(mopHolder &= mopHolder);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 0x3f));
	GEQUAL(result.value, (0x57 & 0x3f));

}


GTEST(Lib_InvokeLogic)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);
	
	const GMetaOperator * op;

	CLASS operand;

	operand.reset(3);
	OP(mopHolder == mopHolder);
	GCHECK(fromVariant<bool>(op->invokeBinary(operand, CLASS(3))));
	GCHECK(! fromVariant<bool>(op->invokeBinary(operand, CLASS(5))));

	operand.reset(3);
	OP(mopHolder != mopHolder);
	GCHECK(fromVariant<bool>(op->invokeBinary(operand, CLASS(5))));
	GCHECK(! fromVariant<bool>(op->invokeBinary(operand, CLASS(3))));

	operand.reset(6);
	OP(mopHolder > mopHolder);
	GCHECK(fromVariant<bool>(op->invokeBinary(operand, CLASS(3))));
	GCHECK(! fromVariant<bool>(op->invokeBinary(operand, CLASS(18))));

	operand.reset(6);
	OP(mopHolder >= mopHolder);
	GCHECK(fromVariant<bool>(op->invokeBinary(operand, CLASS(3))));
	GCHECK(fromVariant<bool>(op->invokeBinary(operand, CLASS(6))));
	GCHECK(! fromVariant<bool>(op->invokeBinary(operand, CLASS(18))));

	operand.reset(6);
	OP(mopHolder < mopHolder);
	GCHECK(fromVariant<bool>(op->invokeBinary(operand, CLASS(18))));
	GCHECK(! fromVariant<bool>(op->invokeBinary(operand, CLASS(3))));

	operand.reset(6);
	OP(mopHolder <= mopHolder);
	GCHECK(fromVariant<bool>(op->invokeBinary(operand, CLASS(18))));
	GCHECK(fromVariant<bool>(op->invokeBinary(operand, CLASS(6))));
	GCHECK(! fromVariant<bool>(op->invokeBinary(operand, CLASS(3))));

	operand.reset(6);
	OP(mopHolder && mopHolder);
	GCHECK(fromVariant<bool>(op->invokeBinary(operand, CLASS(3))));
	GCHECK(! fromVariant<bool>(op->invokeBinary(operand, CLASS(0))));
	operand.reset(0);
	GCHECK(! fromVariant<bool>(op->invokeBinary(operand, CLASS(3))));
	GCHECK(! fromVariant<bool>(op->invokeBinary(operand, CLASS(0))));

	operand.reset(6);
	OP(mopHolder || mopHolder);
	GCHECK(fromVariant<bool>(op->invokeBinary(operand, CLASS(3))));
	GCHECK(fromVariant<bool>(op->invokeBinary(operand, CLASS(0))));
	operand.reset(0);
	GCHECK(fromVariant<bool>(op->invokeBinary(operand, CLASS(3))));
	GCHECK(! fromVariant<bool>(op->invokeBinary(operand, CLASS(0))));

}


GTEST(API_InvokeLogic)
{
	GMetaScopedPointer<IMetaService> service(createMetaService());
	GCHECK(service);

	GMetaScopedPointer<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GMetaScopedPointer<IMetaOperator> op;

	CLASS operand;

	operand.reset(3);
	OP(mopHolder == mopHolder);
	GCHECK(fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(3))));
	GCHECK(! fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(5))));

	operand.reset(3);
	OP(mopHolder != mopHolder);
	GCHECK(fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(5))));
	GCHECK(! fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(3))));

	operand.reset(6);
	OP(mopHolder > mopHolder);
	GCHECK(fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(3))));
	GCHECK(! fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(18))));

	operand.reset(6);
	OP(mopHolder >= mopHolder);
	GCHECK(fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(3))));
	GCHECK(fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(6))));
	GCHECK(! fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(18))));

	operand.reset(6);
	OP(mopHolder < mopHolder);
	GCHECK(fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(18))));
	GCHECK(! fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(3))));

	operand.reset(6);
	OP(mopHolder <= mopHolder);
	GCHECK(fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(18))));
	GCHECK(fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(6))));
	GCHECK(! fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(3))));

	operand.reset(6);
	OP(mopHolder && mopHolder);
	GCHECK(fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(3))));
	GCHECK(! fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(0))));
	operand.reset(0);
	GCHECK(! fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(3))));
	GCHECK(! fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(0))));

	operand.reset(6);
	OP(mopHolder || mopHolder);
	GCHECK(fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(3))));
	GCHECK(fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(0))));
	operand.reset(0);
	GCHECK(fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(3))));
	GCHECK(! fromVariant<bool>(metaCallOperatorBinary(op, operand, CLASS(0))));

}


GTEST(Lib_InvokeUnary)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);
	
	const GMetaOperator * op;

	CLASS operand;
	CLASS result;

	operand.reset(-3);
	OP(+ mopHolder);
	GEQUAL(fromVariant<CLASS>(op->invokeUnary(operand)).value, 3);
	operand.reset(5);
	GEQUAL(fromVariant<CLASS>(op->invokeUnary(operand)).value, 5);

	operand.reset(-3);
	OP(- mopHolder);
	GEQUAL(fromVariant<CLASS>(op->invokeUnary(operand)).value, -3);
	operand.reset(5);
	GEQUAL(fromVariant<CLASS>(op->invokeUnary(operand)).value, -5);

	operand.reset(6);
	OP(++mopHolder);
	result = fromVariant<CLASS>(op->invokeUnary(operand));
	GEQUAL(result.value, 6 + 1);
	GEQUAL(operand.value, 6 + 1);

	operand.reset(6);
	OP(mopHolder++);
	result = fromVariant<CLASS>(op->invokeUnary(operand));
	GEQUAL(result.value, 6);
	GEQUAL(operand.value, 6 + 1);

	operand.reset(6);
	OP(--mopHolder);
	result = fromVariant<CLASS>(op->invokeUnary(operand));
	GEQUAL(result.value, 6 - 1);
	GEQUAL(operand.value, 6 - 1);

	operand.reset(6);
	OP(mopHolder--);
	result = fromVariant<CLASS>(op->invokeUnary(operand));
	GEQUAL(result.value, 6);
	GEQUAL(operand.value, 6 - 1);

	operand.reset(3);
	OP(! mopHolder);
	GCHECK(! fromVariant<CLASS>(op->invokeUnary(operand)).value);
	operand.reset(0);
	GCHECK(fromVariant<CLASS>(op->invokeUnary(operand)).value);

	operand.reset(3);
	OP(~ mopHolder);
	GEQUAL(fromVariant<CLASS>(op->invokeUnary(operand)).value, ~3);
	operand.reset(0);
	GEQUAL(fromVariant<CLASS>(op->invokeUnary(operand)).value, ~0);

}


GTEST(API_InvokeUnary)
{
	GMetaScopedPointer<IMetaService> service(createMetaService());
	GCHECK(service);

	GMetaScopedPointer<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GMetaScopedPointer<IMetaOperator> op;

	CLASS operand;
	CLASS result;

	operand.reset(-3);
	OP(+ mopHolder);
	GEQUAL(fromVariant<CLASS>(metaCallOperatorUnary(op, operand)).value, 3);
	operand.reset(5);
	GEQUAL(fromVariant<CLASS>(metaCallOperatorUnary(op, operand)).value, 5);

	operand.reset(-3);
	OP(- mopHolder);
	GEQUAL(fromVariant<CLASS>(metaCallOperatorUnary(op, operand)).value, -3);
	operand.reset(5);
	GEQUAL(fromVariant<CLASS>(metaCallOperatorUnary(op, operand)).value, -5);

	operand.reset(6);
	OP(++mopHolder);
	result = fromVariant<CLASS>(metaCallOperatorUnary(op, operand));
	GEQUAL(result.value, 6 + 1);
	GEQUAL(operand.value, 6 + 1);

	operand.reset(6);
	OP(mopHolder++);
	result = fromVariant<CLASS>(metaCallOperatorUnary(op, operand));
	GEQUAL(result.value, 6);
	GEQUAL(operand.value, 6 + 1);

	operand.reset(6);
	OP(--mopHolder);
	result = fromVariant<CLASS>(metaCallOperatorUnary(op, operand));
	GEQUAL(result.value, 6 - 1);
	GEQUAL(operand.value, 6 - 1);

	operand.reset(6);
	OP(mopHolder--);
	result = fromVariant<CLASS>(metaCallOperatorUnary(op, operand));
	GEQUAL(result.value, 6);
	GEQUAL(operand.value, 6 - 1);

	operand.reset(3);
	OP(! mopHolder);
	GCHECK(! fromVariant<CLASS>(metaCallOperatorUnary(op, operand)).value);
	operand.reset(0);
	GCHECK(fromVariant<CLASS>(metaCallOperatorUnary(op, operand)).value);

	operand.reset(3);
	OP(~ mopHolder);
	GEQUAL(fromVariant<CLASS>(metaCallOperatorUnary(op, operand)).value, ~3);
	operand.reset(0);
	GEQUAL(fromVariant<CLASS>(metaCallOperatorUnary(op, operand)).value, ~0);

}


GTEST(Lib_InvokeOther)
{
	const GMetaClass * metaClass2 = findMetaClass(NAME_CLASS2);
	GCHECK(metaClass2);
	
	const GMetaOperator * op;

	CLASS operand;
	CLASS result;

	operand.reset(3);
	OP2(& mopHolder);
	GEQUAL(*fromVariant<int *>(op->invokeUnary(operand)), 3);
	*fromVariant<int *>(op->invokeUnary(operand)) = 5;
	GEQUAL(operand.value, 5);

	operand.reset(3);
	OP2(* mopHolder);
	GEQUAL(fromVariant<int>(op->invokeUnary(operand)), 6);

	operand.reset(3);
	OP2((mopHolder, mopHolder));
	GEQUAL(fromVariant<int>(op->invokeBinary(operand, 2)), 5);

	OP2((mopHolder[mopHolder]));
	GEQUAL(fromVariant<string>(op->invokeBinary(operand, 5)), "aaaaa");

	operand.reset(3);
	OP2(mopHolder->mopHolder);
	GEQUAL(*fromVariant<int *>(op->invokeUnary(operand)), 3);
	*fromVariant<int *>(op->invokeUnary(operand)) = 5;
	GEQUAL(operand.value, 5);

	operand.reset(8);
	OP2(mopHolder->*mopHolder);
	int CLASS2::* memPtr = &CLASS2::value;
	GEQUAL(fromVariant<int>(op->invokeBinary(operand, memPtr)), 8);

	OP2(mopHolder());
	GEQUAL(fromVariant<string>(op->invokeUnary(operand)), "abc");

	operand.reset(8);
	OPI2(mopHolder(mopHolder), 0);
	GEQUAL(fromVariant<int>(op->invokeFunctor(&operand, "abc", 5)), 8 + 3 + 5 * 2);

	OPI2(mopHolder(mopHolder), 1);
	GEQUAL(fromVariant<int>(op->invokeFunctor(&operand)), 0);
	GEQUAL(fromVariant<int>(op->invokeFunctor(&operand, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12)),
		1 + 2 + 3 + 5 + 6 + 7 + 8 + 9 + 10 + 11 + 12);

}


GTEST(API_InvokeOther)
{
	GMetaScopedPointer<IMetaService> service(createMetaService());
	GCHECK(service);

	GMetaScopedPointer<IMetaClass> metaClass2(service->findClassByName(NAME_CLASS2));
	GCHECK(metaClass2);

	GMetaScopedPointer<IMetaOperator> op;

	CLASS operand;
	CLASS result;

	operand.reset(3);
	OP2(& mopHolder);
	GEQUAL(*fromVariant<int *>(metaCallOperatorUnary(op, operand)), 3);
	*fromVariant<int *>(metaCallOperatorUnary(op, operand)) = 5;
	GEQUAL(operand.value, 5);

	operand.reset(3);
	OP2(* mopHolder);
	GEQUAL(fromVariant<int>(metaCallOperatorUnary(op, operand)), 6);

	operand.reset(3);
	OP2((mopHolder, mopHolder));
	GEQUAL(fromVariant<int>(metaCallOperatorBinary(op, operand, 2)), 5);

	OP2((mopHolder[mopHolder]));
	GEQUAL(fromVariant<string>(metaCallOperatorBinary(op, operand, 5)), "aaaaa");

	operand.reset(3);
	OP2(mopHolder->mopHolder);
	GEQUAL(*fromVariant<int *>(metaCallOperatorUnary(op, operand)), 3);
	*fromVariant<int *>(metaCallOperatorUnary(op, operand)) = 5;
	GEQUAL(operand.value, 5);

	operand.reset(8);
	OP2(mopHolder->*mopHolder);
	int CLASS2::* memPtr = &CLASS2::value;
	GEQUAL(fromVariant<int>(metaCallOperatorBinary(op, operand, memPtr)), 8);

	OP2(mopHolder());
	GEQUAL(fromVariant<string>(metaCallOperatorUnary(op, operand)), "abc");

	operand.reset(8);
	OPI2(mopHolder(mopHolder), 0);
	GEQUAL(fromVariant<int>(metaCallOperatorFunctor(op, &operand, "abc", 5)), 8 + 3 + 5 * 2);

	OPI2(mopHolder(mopHolder), 1);
	GEQUAL(fromVariant<int>(metaCallOperatorFunctor(op, &operand)), 0);
	GEQUAL(fromVariant<int>(metaCallOperatorFunctor(op, &operand, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12)),
		1 + 2 + 3 + 5 + 6 + 7 + 8 + 9 + 10 + 11 + 12);

}




} }


