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
		this->value >>= n;

		return *this;
	}

	CLASS & operator <<= (int n) {
		this->value <<= n;

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


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<CLASS>
		::define(NAME_CLASS)

		// arithmetic

		._operator<CLASS & (*)(GMetaSelf, const CLASS &)>(mopHolder = mopHolder)
		._operator<CLASS (const GMetaSelf &, int)>(mopHolder + mopHolder)
		._operator<CLASS (int, const CLASS &)>(mopHolder + mopHolder)
		._operator<CLASS (const GMetaSelf &, int)>(mopHolder - mopHolder)
		._operator<CLASS (const GMetaSelf &, int)>(mopHolder * mopHolder)
		._operator<CLASS (const GMetaSelf &, int)>(mopHolder / mopHolder)
		._operator<CLASS (const GMetaSelf &, int)>(mopHolder % mopHolder)
		._operator<CLASS (GMetaSelf, int)>(mopHolder % mopHolder)

		// arithmetic assign

		._operator<CLASS & (*)(GMetaSelf, int)>(mopHolder += mopHolder)
		._operator<CLASS & (*)(GMetaSelf, int)>(mopHolder -= mopHolder)
		._operator<CLASS & (*)(GMetaSelf, int)>(mopHolder *= mopHolder)
		._operator<CLASS & (*)(GMetaSelf, int)>(mopHolder /= mopHolder)
		._operator<CLASS & (*)(GMetaSelf, int)>(mopHolder %= mopHolder)

		// bitwise

		._operator<CLASS (const GMetaSelf &, int)>(mopHolder & mopHolder)
		._operator<CLASS (const GMetaSelf &, int)>(mopHolder | mopHolder)
		._operator<CLASS (const GMetaSelf &, int)>(mopHolder ^ mopHolder)
		._operator<CLASS (const GMetaSelf &, int)>(mopHolder >> mopHolder)
		._operator<CLASS (const GMetaSelf &, int)>(mopHolder << mopHolder)

		// bitwise assign

		._operator<CLASS & (*)(GMetaSelf, int)>(mopHolder &= mopHolder)
		._operator<CLASS & (*)(GMetaSelf, int)>(mopHolder |= mopHolder)
		._operator<CLASS & (*)(GMetaSelf, int)>(mopHolder ^= mopHolder)
		._operator<CLASS & (*)(GMetaSelf, int)>(mopHolder >>= mopHolder)
		._operator<CLASS & (*)(GMetaSelf, int)>(mopHolder <<= mopHolder)

		// logic

		._operator<bool (const GMetaSelf &, const CLASS &)>(mopHolder == mopHolder)
		._operator<bool (const GMetaSelf &, const CLASS &)>(mopHolder != mopHolder)
		._operator<bool (const GMetaSelf &, const CLASS &)>(mopHolder > mopHolder)
		._operator<bool (const GMetaSelf &, const CLASS &)>(mopHolder >= mopHolder)
		._operator<bool (const GMetaSelf &, const CLASS &)>(mopHolder < mopHolder)
		._operator<bool (const GMetaSelf &, const CLASS &)>(mopHolder <= mopHolder)
		._operator<bool (const GMetaSelf &, const CLASS &)>(mopHolder && mopHolder)
		._operator<bool (const GMetaSelf &, const CLASS &)>(mopHolder || mopHolder)

		// unary

		._operator<CLASS (const GMetaSelf &)> (+ mopHolder)
		._operator<CLASS (const GMetaSelf &)> (- mopHolder)
		._operator<CLASS (GMetaSelf)> (++mopHolder)
		._operator<CLASS (GMetaSelf)> (mopHolder++)
		._operator<CLASS (GMetaSelf)> (--mopHolder)
		._operator<CLASS (GMetaSelf)> (mopHolder--)
		._operator<CLASS (const GMetaSelf &)> (! mopHolder)
		._operator<CLASS (const GMetaSelf &)> (~ mopHolder)
	;
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


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<CLASS2> define = GDefineMetaClass<CLASS2>::define(NAME_CLASS2);

	// other

	define._operator<int * (GMetaSelf)> (& mopHolder);
	define._operator<int (const GMetaSelf &)> (* mopHolder);
	define._operator<CLASS2 & (*)(GMetaSelf, int)>((mopHolder , mopHolder));
	define._operator<std::string (GMetaSelf, int)>(mopHolder[0]);
	define._operator<int * (*)(GMetaSelf)>(mopHolder->mopHolder);
	define._operator<int (*)(GMetaSelf, int CLASS2::*)>(mopHolder->*mopHolder);
	define._operator<std::string (GMetaSelf)>(mopHolder());
	define._operator<int (const std::string &, int)>(mopHolder(mopHolder), GMetaPolicyCopyAllConstReference());
	define._operator<int (const GMetaVariadicParam *)>(mopHolder(mopHolder));
}


#ifndef G_COMPILER_CPPBUILDER
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
#endif


#ifndef G_COMPILER_CPPBUILDER
GTEST(API_Exists)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaOperator> op;

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
	
	GScopedInterface<IMetaClass> metaClass2(service->findClassByName(NAME_CLASS2));
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
#endif


#ifndef G_COMPILER_CPPBUILDER
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
#endif


#ifndef G_COMPILER_CPPBUILDER
GTEST(API_InvokeArithmetic)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaOperator> op;

	CLASS operand;
	CLASS result;

	operand.reset(5);
	OP(mopHolder + mopHolder);
	result = fromVariant<CLASS>(metaInvokeOperatorBinary(op.get(), operand, 6));
	GEQUAL(result.value, 5 + 6);

	OPI(mopHolder + mopHolder, 1);
	result = fromVariant<CLASS>(metaInvokeOperatorBinary(op.get(), 3, CLASS(7)));
	GEQUAL(result.value, 3 + 7 * 2);

	operand.reset(5);
	OP(mopHolder - mopHolder);
	result = fromVariant<CLASS>(metaInvokeOperatorBinary(op.get(), operand, 6));
	GEQUAL(result.value, 5 - 6);

	operand.reset(5);
	OP(mopHolder * mopHolder);
	result = fromVariant<CLASS>(metaInvokeOperatorBinary(op.get(), operand, 6));
	GEQUAL(result.value, 5 * 6);

	operand.reset(25);
	OP(mopHolder / mopHolder);
	result = fromVariant<CLASS>(metaInvokeOperatorBinary(op.get(), operand, 6));
	GEQUAL(result.value, 25 / 6);

	operand.reset(25);
	OP(mopHolder % mopHolder);
	result = fromVariant<CLASS>(metaInvokeOperatorBinary(op.get(), operand, 6));
	GEQUAL(result.value, 25 % 6);

	operand.reset(25);
	OPI(mopHolder % mopHolder, 1);
	result = fromVariant<CLASS>(metaInvokeOperatorBinary(op.get(), operand, 6));
	GEQUAL(result.value, 25 % (6 + 1));

}
#endif


#ifndef G_COMPILER_CPPBUILDER
GTEST(Lib_InvokeArithmeticAssign)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);

	const GMetaOperator * op;

	CLASS operand;
	CLASS * addr = &operand;
	CLASS * presult = addr;

	operand.reset(5);
	OP(mopHolder += mopHolder);
	presult = &(fromVariant<CLASS &>(op->invokeBinary(operand, 6)));
	GEQUAL(presult, addr);
	GEQUAL(presult->value, 5 + 6);

	operand.reset(5);
	OP(mopHolder -= mopHolder);
	presult = &(fromVariant<CLASS &>(op->invokeBinary(operand, 6)));
	GEQUAL(presult, addr);
	GEQUAL(presult->value, 5 - 6);

	operand.reset(5);
	OP(mopHolder *= mopHolder);
	presult = &(fromVariant<CLASS &>(op->invokeBinary(operand, 6)));
	GEQUAL(presult, addr);
	GEQUAL(presult->value, 5 * 6);

	operand.reset(50);
	OP(mopHolder /= mopHolder);
	presult = &(fromVariant<CLASS &>(op->invokeBinary(operand, 6)));
	GEQUAL(presult, addr);
	GEQUAL(presult->value, 50 / 6);

	operand.reset(50);
	OP(mopHolder %= mopHolder);
	presult = &(fromVariant<CLASS &>(op->invokeBinary(operand, 6)));
	GEQUAL(presult, addr);
	GEQUAL(presult->value, 50 % 6);

}
#endif


#ifndef G_COMPILER_CPPBUILDER
GTEST(API_InvokeArithmeticAssign)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaOperator> op;

	CLASS operand;
	CLASS * addr = &operand;
	CLASS * presult = addr;

	operand.reset(5);
	OP(mopHolder += mopHolder);
	presult = &(fromVariant<CLASS &>(metaInvokeOperatorBinary(op.get(), operand, 6)));
	GEQUAL(presult, addr);
	GEQUAL(presult->value, 5 + 6);

	operand.reset(5);
	OP(mopHolder -= mopHolder);
	presult = &(fromVariant<CLASS &>(metaInvokeOperatorBinary(op.get(), operand, 6)));
	GEQUAL(presult, addr);
	GEQUAL(presult->value, 5 - 6);

	operand.reset(5);
	OP(mopHolder *= mopHolder);
	presult = &(fromVariant<CLASS &>(metaInvokeOperatorBinary(op.get(), operand, 6)));
	GEQUAL(presult, addr);
	GEQUAL(presult->value, 5 * 6);

	operand.reset(50);
	OP(mopHolder /= mopHolder);
	presult = &(fromVariant<CLASS &>(metaInvokeOperatorBinary(op.get(), operand, 6)));
	GEQUAL(presult, addr);
	GEQUAL(presult->value, 50 / 6);

	operand.reset(50);
	OP(mopHolder %= mopHolder);
	presult = &(fromVariant<CLASS &>(metaInvokeOperatorBinary(op.get(), operand, 6)));
	GEQUAL(presult, addr);
	GEQUAL(presult->value, 50 % 6);

}
#endif


#ifndef G_COMPILER_CPPBUILDER
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
	GEQUAL(result.value, (0x57 | 0x3f));

	operand.reset(0x57);
	OP(mopHolder ^ mopHolder);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 0x3f));
	GEQUAL(result.value, (0x57 ^ 0x3f));

	operand.reset(0x57);
	OP(mopHolder << mopHolder);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 3));
	GEQUAL(result.value, (0x57 << 3));

	operand.reset(0x57);
	OP(mopHolder >> mopHolder);
	result = fromVariant<CLASS>(op->invokeBinary(operand, 3));
	GEQUAL(result.value, (0x57 >> 3));

}
#endif


#ifndef G_COMPILER_CPPBUILDER
GTEST(API_InvokeBitwise)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaOperator> op;

	CLASS operand;
	CLASS result;

	operand.reset(0x57);
	OP(mopHolder & mopHolder);
	result = fromVariant<CLASS>(metaInvokeOperatorBinary(op.get(), operand, 0x3f));
	GEQUAL(result.value, (0x57 & 0x3f));

	operand.reset(0x57);
	OP(mopHolder | mopHolder);
	result = fromVariant<CLASS>(metaInvokeOperatorBinary(op.get(), operand, 0x3f));
	GEQUAL(result.value, (0x57 | 0x3f));

	operand.reset(0x57);
	OP(mopHolder ^ mopHolder);
	result = fromVariant<CLASS>(metaInvokeOperatorBinary(op.get(), operand, 0x3f));
	GEQUAL(result.value, (0x57 ^ 0x3f));

	operand.reset(0x57);
	OP(mopHolder << mopHolder);
	result = fromVariant<CLASS>(metaInvokeOperatorBinary(op.get(), operand, 3));
	GEQUAL(result.value, (0x57 << 3));

	operand.reset(0x57);
	OP(mopHolder >> mopHolder);
	result = fromVariant<CLASS>(metaInvokeOperatorBinary(op.get(), operand, 3));
	GEQUAL(result.value, (0x57 >> 3));

}
#endif


#ifndef G_COMPILER_CPPBUILDER
GTEST(Lib_InvokeBitwiseAssign)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);
	
	const GMetaOperator * op;

	CLASS operand;
	CLASS * presult;

	operand.reset(0x57);
	OP(mopHolder &= mopHolder);
	presult = &fromVariant<CLASS &>(op->invokeBinary(operand, 0x3f));
	GEQUAL(presult, &operand);
	GEQUAL(presult->value, (0x57 & 0x3f));

	operand.reset(0x57);
	OP(mopHolder |= mopHolder);
	presult = &fromVariant<CLASS &>(op->invokeBinary(operand, 0x3f));
	GEQUAL(presult, &operand);
	GEQUAL(presult->value, (0x57 | 0x3f));

	operand.reset(0x57);
	OP(mopHolder ^= mopHolder);
	presult = &fromVariant<CLASS &>(op->invokeBinary(operand, 0x3f));
	GEQUAL(presult, &operand);
	GEQUAL(presult->value, (0x57 ^ 0x3f));

	operand.reset(0x57);
	OP(mopHolder <<= mopHolder);
	presult = &fromVariant<CLASS &>(op->invokeBinary(operand, 3));
	GEQUAL(presult, &operand);
	GEQUAL(presult->value, (0x57 << 3));

	operand.reset(0x57);
	OP(mopHolder >>= mopHolder);
	presult = &fromVariant<CLASS &>(op->invokeBinary(operand, 3));
	GEQUAL(presult, &operand);
	GEQUAL(presult->value, (0x57 >> 3));

}
#endif


#ifndef G_COMPILER_CPPBUILDER
GTEST(API_InvokeBitwiseAssign)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaOperator> op;

	CLASS operand;
	CLASS * presult;

	operand.reset(0x57);
	OP(mopHolder &= mopHolder);
	presult = &fromVariant<CLASS &>(metaInvokeOperatorBinary(op.get(), operand, 0x3f));
	GEQUAL(presult, &operand);
	GEQUAL(presult->value, (0x57 & 0x3f));

	operand.reset(0x57);
	OP(mopHolder |= mopHolder);
	presult = &fromVariant<CLASS &>(metaInvokeOperatorBinary(op.get(), operand, 0x3f));
	GEQUAL(presult, &operand);
	GEQUAL(presult->value, (0x57 | 0x3f));

	operand.reset(0x57);
	OP(mopHolder ^= mopHolder);
	presult = &fromVariant<CLASS &>(metaInvokeOperatorBinary(op.get(), operand, 0x3f));
	GEQUAL(presult, &operand);
	GEQUAL(presult->value, (0x57 ^ 0x3f));

	operand.reset(0x57);
	OP(mopHolder <<= mopHolder);
	presult = &fromVariant<CLASS &>(metaInvokeOperatorBinary(op.get(), operand, 3));
	GEQUAL(presult, &operand);
	GEQUAL(presult->value, (0x57 << 3));

	operand.reset(0x57);
	OP(mopHolder >>= mopHolder);
	presult = &fromVariant<CLASS &>(metaInvokeOperatorBinary(op.get(), operand, 3));
	GEQUAL(presult, &operand);
	GEQUAL(presult->value, (0x57 >> 3));

}
#endif


#ifndef G_COMPILER_CPPBUILDER
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
#endif


#ifndef G_COMPILER_CPPBUILDER
GTEST(API_InvokeLogic)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaOperator> op;

	CLASS operand;

	operand.reset(3);
	OP(mopHolder == mopHolder);
	GCHECK(fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(3))));
	GCHECK(! fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(5))));

	operand.reset(3);
	OP(mopHolder != mopHolder);
	GCHECK(fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(5))));
	GCHECK(! fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(3))));

	operand.reset(6);
	OP(mopHolder > mopHolder);
	GCHECK(fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(3))));
	GCHECK(! fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(18))));

	operand.reset(6);
	OP(mopHolder >= mopHolder);
	GCHECK(fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(3))));
	GCHECK(fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(6))));
	GCHECK(! fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(18))));

	operand.reset(6);
	OP(mopHolder < mopHolder);
	GCHECK(fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(18))));
	GCHECK(! fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(3))));

	operand.reset(6);
	OP(mopHolder <= mopHolder);
	GCHECK(fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(18))));
	GCHECK(fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(6))));
	GCHECK(! fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(3))));

	operand.reset(6);
	OP(mopHolder && mopHolder);
	GCHECK(fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(3))));
	GCHECK(! fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(0))));
	operand.reset(0);
	GCHECK(! fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(3))));
	GCHECK(! fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(0))));

	operand.reset(6);
	OP(mopHolder || mopHolder);
	GCHECK(fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(3))));
	GCHECK(fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(0))));
	operand.reset(0);
	GCHECK(fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(3))));
	GCHECK(! fromVariant<bool>(metaInvokeOperatorBinary(op.get(), operand, CLASS(0))));

}
#endif


#ifndef G_COMPILER_CPPBUILDER
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
#endif


#ifndef G_COMPILER_CPPBUILDER
GTEST(API_InvokeUnary)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaOperator> op;

	CLASS operand;
	CLASS result;

	operand.reset(-3);
	OP(+ mopHolder);
	GEQUAL(fromVariant<CLASS>(metaInvokeOperatorUnary(op.get(), operand)).value, 3);
	operand.reset(5);
	GEQUAL(fromVariant<CLASS>(metaInvokeOperatorUnary(op.get(), operand)).value, 5);

	operand.reset(-3);
	OP(- mopHolder);
	GEQUAL(fromVariant<CLASS>(metaInvokeOperatorUnary(op.get(), operand)).value, -3);
	operand.reset(5);
	GEQUAL(fromVariant<CLASS>(metaInvokeOperatorUnary(op.get(), operand)).value, -5);

	operand.reset(6);
	OP(++mopHolder);
	result = fromVariant<CLASS>(metaInvokeOperatorUnary(op.get(), operand));
	GEQUAL(result.value, 6 + 1);
	GEQUAL(operand.value, 6 + 1);

	operand.reset(6);
	OP(mopHolder++);
	result = fromVariant<CLASS>(metaInvokeOperatorUnary(op.get(), operand));
	GEQUAL(result.value, 6);
	GEQUAL(operand.value, 6 + 1);

	operand.reset(6);
	OP(--mopHolder);
	result = fromVariant<CLASS>(metaInvokeOperatorUnary(op.get(), operand));
	GEQUAL(result.value, 6 - 1);
	GEQUAL(operand.value, 6 - 1);

	operand.reset(6);
	OP(mopHolder--);
	result = fromVariant<CLASS>(metaInvokeOperatorUnary(op.get(), operand));
	GEQUAL(result.value, 6);
	GEQUAL(operand.value, 6 - 1);

	operand.reset(3);
	OP(! mopHolder);
	GCHECK(! fromVariant<CLASS>(metaInvokeOperatorUnary(op.get(), operand)).value);
	operand.reset(0);
	GCHECK(fromVariant<CLASS>(metaInvokeOperatorUnary(op.get(), operand)).value);

	operand.reset(3);
	OP(~ mopHolder);
	GEQUAL(fromVariant<CLASS>(metaInvokeOperatorUnary(op.get(), operand)).value, ~3);
	operand.reset(0);
	GEQUAL(fromVariant<CLASS>(metaInvokeOperatorUnary(op.get(), operand)).value, ~0);

}
#endif


#ifndef G_COMPILER_CPPBUILDER
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
#endif


GTEST(API_InvokeOther)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass2(service->findClassByName(NAME_CLASS2));
	GCHECK(metaClass2);

	GScopedInterface<IMetaOperator> op;

	CLASS operand;
	CLASS result;

	operand.reset(3);
	OP2(& mopHolder);
	GEQUAL(*fromVariant<int *>(metaInvokeOperatorUnary(op.get(), operand)), 3);
	*fromVariant<int *>(metaInvokeOperatorUnary(op.get(), operand)) = 5;
	GEQUAL(operand.value, 5);

	operand.reset(3);
	OP2(* mopHolder);
	GEQUAL(fromVariant<int>(metaInvokeOperatorUnary(op.get(), operand)), 6);

	operand.reset(3);
	OP2((mopHolder, mopHolder));
	GEQUAL(fromVariant<int>(metaInvokeOperatorBinary(op.get(), operand, 2)), 5);

	OP2((mopHolder[mopHolder]));
	GEQUAL(fromVariant<string>(metaInvokeOperatorBinary(op.get(), operand, 5)), "aaaaa");

	operand.reset(3);
	OP2(mopHolder->mopHolder);
	GEQUAL(*fromVariant<int *>(metaInvokeOperatorUnary(op.get(), operand)), 3);
	*fromVariant<int *>(metaInvokeOperatorUnary(op.get(), operand)) = 5;
	GEQUAL(operand.value, 5);

	operand.reset(8);
	OP2(mopHolder->*mopHolder);
	int CLASS2::* memPtr = &CLASS2::value;
	GEQUAL(fromVariant<int>(metaInvokeOperatorBinary(op.get(), operand, memPtr)), 8);

	OP2(mopHolder());
	GEQUAL(fromVariant<string>(metaInvokeOperatorUnary(op.get(), operand)), "abc");

	operand.reset(8);
	OPI2(mopHolder(mopHolder), 0);
	GEQUAL(fromVariant<int>(metaInvokeOperatorFunctor(op.get(), &operand, "abc", 5)), 8 + 3 + 5 * 2);

	OPI2(mopHolder(mopHolder), 1);
	GEQUAL(fromVariant<int>(metaInvokeOperatorFunctor(op.get(), &operand)), 0);
	GEQUAL(fromVariant<int>(metaInvokeOperatorFunctor(op.get(), &operand, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12)),
		1 + 2 + 3 + 5 + 6 + 7 + 8 + 9 + 10 + 11 + 12);

}




} }



