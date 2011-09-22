#include "test_reflection_common.h"

#define CLASS TestClass_Operator
#define NAME_CLASS GPP_STRINGIZE(CLASS)

#define OP(operand) pointerAssign(op, getOperator(operand, metaClass, 0))
#define OPI(operand, i) pointerAssign(op, getOperator(operand, metaClass, i))

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

	int * operator & () {
		return &this->value;
	}

	int operator * () const {
		return this->value * 2;
	}

	// other

	CLASS & operator , (int n) {
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

	int operator ->* (int CLASS::* p) {
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
	reflectOperator<int * (const GMetaSelf &)> (& mopHolder);
	reflectOperator<int (const GMetaSelf &)> (* mopHolder);

	// other

	reflectOperator<CLASS & (GMetaSelf, int)>((mopHolder , mopHolder));
	reflectOperator<std::string (GMetaSelf, int)>(mopHolder[0]);
	reflectOperator<int * (GMetaSelf)>(mopHolder->mopHolder);
	reflectOperator<int (GMetaSelf, int CLASS::*)>(mopHolder->*mopHolder);
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
	
	OP(& mopHolder);
	GCHECK(op);
	
	OP(* mopHolder);
	GCHECK(op);

	OP((mopHolder , mopHolder));
	GCHECK(op);

	OP(mopHolder[0]);
	GCHECK(op);

	OP(mopHolder -> mopHolder);
	GCHECK(op);

	OP(mopHolder ->* mopHolder);
	GCHECK(op);

	OPI(mopHolder(), 0);
	GCHECK(op);

	OPI(mopHolder(mopHolder), 1);
	GCHECK(op);

	OPI(mopHolder(), 0);
	GCHECK(op);

	OPI(mopHolder(mopHolder), 1);
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


GTEST(Lib_InvokeArithmeticAssign)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);
	
	const GMetaOperator * op;

	CLASS operand;
	CLASS * addr = getObjectAddress(operand);
	CLASS * presult = addr;
/*
	operand.reset(5);
	OP(mopHolder += mopHolder);
	presult = (CLASS *)(objectAddressFromVariant(op->invokeBinary(operand, 6)));
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






} }


