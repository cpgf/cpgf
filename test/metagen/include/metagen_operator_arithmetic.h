#ifndef CPGF_METAGEN_OPERATOR_ARITHMETIC_H
#define CPGF_METAGEN_OPERATOR_ARITHMETIC_H

#include <string>

class MetagenOperatorArithmetic
{
public:
	MetagenOperatorArithmetic() : value(0) {}
	explicit MetagenOperatorArithmetic(int value) : value(value) {}
	
	MetagenOperatorArithmetic operator + (int n) const { return MetagenOperatorArithmetic(value + n); }
	MetagenOperatorArithmetic operator + (const MetagenOperatorArithmetic & other) const { return MetagenOperatorArithmetic(value + other.value); }
	MetagenOperatorArithmetic operator + (const std::string & s) const { return MetagenOperatorArithmetic(value + s.length()); }
	
	MetagenOperatorArithmetic operator - (int n) { return MetagenOperatorArithmetic(value - n); }
	MetagenOperatorArithmetic operator - (const MetagenOperatorArithmetic & other) { return MetagenOperatorArithmetic(value - other.value); }
	MetagenOperatorArithmetic operator - (const std::string & s) { return MetagenOperatorArithmetic(value - s.length()); }
	
	MetagenOperatorArithmetic operator * (int n) { return MetagenOperatorArithmetic(value * n); }
	MetagenOperatorArithmetic operator * (const MetagenOperatorArithmetic & other) { return MetagenOperatorArithmetic(value * other.value); }
	MetagenOperatorArithmetic operator * (const std::string & s) { return MetagenOperatorArithmetic(value * s.length()); }
	
	MetagenOperatorArithmetic operator / (int n) { return MetagenOperatorArithmetic(value / n); }
	MetagenOperatorArithmetic operator / (const MetagenOperatorArithmetic & other) { return MetagenOperatorArithmetic(value / other.value); }
	MetagenOperatorArithmetic operator / (const std::string & s) { return MetagenOperatorArithmetic(value / s.length()); }
	
	MetagenOperatorArithmetic operator % (int n) { return MetagenOperatorArithmetic(value % n); }
	MetagenOperatorArithmetic operator % (const MetagenOperatorArithmetic & other) { return MetagenOperatorArithmetic(value % other.value); }
	MetagenOperatorArithmetic operator % (const std::string & s) { return MetagenOperatorArithmetic(value % s.length()); }
	
public:
	int value;
};



#endif
