#ifndef CPGF_METAGEN_OPERATOR_INNER_CLASS_H
#define CPGF_METAGEN_OPERATOR_INNER_CLASS_H

#include <string>

class MetagenOperatorInnerClass
{
public:
	class MetagenOperatorInner
	{
	public:
		MetagenOperatorInner() : value(0) {}
		explicit MetagenOperatorInner(int value) : value(value) {}
	
		MetagenOperatorInner operator + (int n) const { return MetagenOperatorInner(value + n); }
		MetagenOperatorInner operator + (const MetagenOperatorInner & other) const { return MetagenOperatorInner(value + other.value); }
		MetagenOperatorInner operator + (const std::string & s) const { return MetagenOperatorInner(value + s.length()); }
	
		MetagenOperatorInner operator - (int n) { return MetagenOperatorInner(value - n); }
		MetagenOperatorInner operator - (const MetagenOperatorInner & other) { return MetagenOperatorInner(value - other.value); }
		MetagenOperatorInner operator - (const std::string & s) { return MetagenOperatorInner(value - s.length()); }
	
		MetagenOperatorInner operator * (int n) { return MetagenOperatorInner(value * n); }
		MetagenOperatorInner operator * (const MetagenOperatorInner & other) { return MetagenOperatorInner(value * other.value); }
		MetagenOperatorInner operator * (const std::string & s) { return MetagenOperatorInner(value * s.length()); }
	
		MetagenOperatorInner operator / (int n) { return MetagenOperatorInner(value / n); }
		MetagenOperatorInner operator / (const MetagenOperatorInner & other) { return MetagenOperatorInner(value / other.value); }
		MetagenOperatorInner operator / (const std::string & s) { return MetagenOperatorInner(value / s.length()); }
	
		MetagenOperatorInner operator % (int n) { return MetagenOperatorInner(value % n); }
		MetagenOperatorInner operator % (const MetagenOperatorInner & other) { return MetagenOperatorInner(value % other.value); }
		MetagenOperatorInner operator % (const std::string & s) { return MetagenOperatorInner(value % s.length()); }
	
	public:
		int value;
	};
};



#endif
