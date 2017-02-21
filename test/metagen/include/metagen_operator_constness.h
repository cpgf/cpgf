#ifndef CPGF_METAGEN_OPERATOR_CONSTNESS_H
#define CPGF_METAGEN_OPERATOR_CONSTNESS_H

#include <string>

class MetagenOperatorConstness
{
public:
	MetagenOperatorConstness() : value(0) {}
	explicit MetagenOperatorConstness(int value) : value(value) {}
	
	const MetagenOperatorConstness * constSelf() { return this; }
	const MetagenOperatorConstness & constSelfRef() { return *this; }
	
	MetagenOperatorConstness operator + (int n) { return MetagenOperatorConstness(value + n); }
	MetagenOperatorConstness operator - (int n) const { return MetagenOperatorConstness(value - n); }
	
public:
	int value;
};



#endif
