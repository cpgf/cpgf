#ifndef CPGF_METAGEN_OPERATOR_BITWISE_ASSIGN_H
#define CPGF_METAGEN_OPERATOR_BITWISE_ASSIGN_H

#include <string>

class MetagenOperatorBitwiseAssign
{
public:
	MetagenOperatorBitwiseAssign() : value(0) {}
	explicit MetagenOperatorBitwiseAssign(int value) : value(value) {}

	MetagenOperatorBitwiseAssign & operator &= (int n) { value &= n; return *this; }
	MetagenOperatorBitwiseAssign & operator &= (const MetagenOperatorBitwiseAssign & other) { value &= other.value; return *this; }
	MetagenOperatorBitwiseAssign & operator &= (const std::string & s) { value &= s.length(); return *this; }
	
	MetagenOperatorBitwiseAssign & operator |= (int n) { value |= n; return *this; }
	MetagenOperatorBitwiseAssign & operator |= (const MetagenOperatorBitwiseAssign & other) { value |= other.value; return *this; }
	MetagenOperatorBitwiseAssign & operator |= (const std::string & s) { value |= s.length(); return *this; }
	
	MetagenOperatorBitwiseAssign & operator ^= (int n) { value ^= n; return *this; }
	MetagenOperatorBitwiseAssign & operator ^= (const MetagenOperatorBitwiseAssign & other) { value ^= other.value; return *this; }
	MetagenOperatorBitwiseAssign & operator ^= (const std::string & s) { value ^= s.length(); return *this; }
	
	MetagenOperatorBitwiseAssign & operator <<= (int n) { value <<= n; return *this; }
	MetagenOperatorBitwiseAssign & operator <<= (const MetagenOperatorBitwiseAssign & other) { value <<= other.value; return *this; }
	MetagenOperatorBitwiseAssign & operator <<= (const std::string & s) { value <<= s.length(); return *this; }
	
	MetagenOperatorBitwiseAssign & operator >>= (int n) { value >>= n; return *this; }
	MetagenOperatorBitwiseAssign & operator >>= (const MetagenOperatorBitwiseAssign & other) { value >>= other.value; return *this; }
	MetagenOperatorBitwiseAssign & operator >>= (const std::string & s) { value >>= s.length(); return *this; }
	
public:
	int value;
};



#endif
