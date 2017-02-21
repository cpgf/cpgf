#ifndef CPGF_METAGEN_OPERATOR_BITWISE_H
#define CPGF_METAGEN_OPERATOR_BITWISE_H

#include <string>

class MetagenOperatorBitwise
{
public:
	MetagenOperatorBitwise() : value(0) {}
	explicit MetagenOperatorBitwise(int value) : value(value) {}
	
	MetagenOperatorBitwise operator & (int n) const { return MetagenOperatorBitwise(value & n); }
	MetagenOperatorBitwise operator & (const MetagenOperatorBitwise & other) const { return MetagenOperatorBitwise(value & other.value); }
	MetagenOperatorBitwise operator & (const std::string & s) const { return MetagenOperatorBitwise(value & s.length()); }
	
	MetagenOperatorBitwise operator | (int n) { return MetagenOperatorBitwise(value | n); }
	MetagenOperatorBitwise operator | (const MetagenOperatorBitwise & other) { return MetagenOperatorBitwise(value | other.value); }
	MetagenOperatorBitwise operator | (const std::string & s) { return MetagenOperatorBitwise(value | s.length()); }
	
	MetagenOperatorBitwise operator ^ (int n) { return MetagenOperatorBitwise(value ^ n); }
	MetagenOperatorBitwise operator ^ (const MetagenOperatorBitwise & other) { return MetagenOperatorBitwise(value ^ other.value); }
	MetagenOperatorBitwise operator ^ (const std::string & s) { return MetagenOperatorBitwise(value ^ s.length()); }
	
	MetagenOperatorBitwise operator << (int n) { return MetagenOperatorBitwise(value << n); }
	MetagenOperatorBitwise operator << (const MetagenOperatorBitwise & other) { return MetagenOperatorBitwise(value << other.value); }
	MetagenOperatorBitwise operator << (const std::string & s) { return MetagenOperatorBitwise(value << s.length()); }
	
	MetagenOperatorBitwise operator >> (int n) { return MetagenOperatorBitwise(value >> n); }
	MetagenOperatorBitwise operator >> (const MetagenOperatorBitwise & other) { return MetagenOperatorBitwise(value >> other.value); }
	MetagenOperatorBitwise operator >> (const std::string & s) { return MetagenOperatorBitwise(value >> s.length()); }
	
	MetagenOperatorBitwise operator ~ () { return MetagenOperatorBitwise(~value); }
	
public:
	int value;
};



#endif
