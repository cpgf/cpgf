#ifndef CPGF_METAGEN_OPERATOR_ARITHMETIC_ASSIGN_H
#define CPGF_METAGEN_OPERATOR_ARITHMETIC_ASSIGN_H

#include <string>

class MetagenOperatorArithmeticAssign
{
public:
	MetagenOperatorArithmeticAssign() : value(0) {}
	explicit MetagenOperatorArithmeticAssign(int value) : value(value) {}
	
	MetagenOperatorArithmeticAssign & operator += (int n) { value += n; return *this; }
	MetagenOperatorArithmeticAssign & operator += (const MetagenOperatorArithmeticAssign & other) { value += other.value; return *this; }
	MetagenOperatorArithmeticAssign & operator += (const std::string & s) { value += s.length(); return *this; }
	
	MetagenOperatorArithmeticAssign & operator -= (int n) { value -= n; return *this; }
	MetagenOperatorArithmeticAssign & operator -= (const MetagenOperatorArithmeticAssign & other) { value -= other.value; return *this; }
	MetagenOperatorArithmeticAssign & operator -= (const std::string & s) { value -= s.length(); return *this; }
	
	MetagenOperatorArithmeticAssign & operator *= (int n) { value *= n; return *this; }
	MetagenOperatorArithmeticAssign & operator *= (const MetagenOperatorArithmeticAssign & other) { value *= other.value; return *this; }
	MetagenOperatorArithmeticAssign & operator *= (const std::string & s) { value *= s.length(); return *this; }
	
	MetagenOperatorArithmeticAssign & operator /= (int n) { value /= n; return *this; }
	MetagenOperatorArithmeticAssign & operator /= (const MetagenOperatorArithmeticAssign & other) { value /= other.value; return *this; }
	MetagenOperatorArithmeticAssign & operator /= (const std::string & s) { value /= s.length(); return *this; }
	
	MetagenOperatorArithmeticAssign & operator %= (int n) { value %= n; return *this; }
	MetagenOperatorArithmeticAssign & operator %= (const MetagenOperatorArithmeticAssign & other) { value %= other.value; return *this; }
	MetagenOperatorArithmeticAssign & operator %= (const std::string & s) { value %= s.length(); return *this; }
	
public:
	int value;
};



#endif
