#ifndef CPGF_METAGEN_OPERATOR_OTHER_H
#define CPGF_METAGEN_OPERATOR_OTHER_H

#include "cpgf/gmetacommon.h"

#include <string>

class MetagenOperatorOther
{
public:
	MetagenOperatorOther() : value(0) {}
	explicit MetagenOperatorOther(int value) : value(value), f((float)value) {}

	MetagenOperatorOther & operator = (const MetagenOperatorOther & other) { value = other.value; return *this; }
	
	MetagenOperatorOther operator , (int n) const { return MetagenOperatorOther(value + n); }
	
	int operator [] (int n) const { return value + n; }
	int operator [] (const std::string & s) const { return value + (int)s.length(); }
	
	float & operator [] (const MetagenOperatorOther & n) { (void)n; return f; }
	
	int operator & () { return value + 1; }
	int operator * () { return value - 1; }
	
	int operator () (const std::string & s, int n) const {
		return value + (int)s.length() + n;
	}

	int operator () (const cpgf::GMetaVariadicParam * params) const {
		int total = value;
		for(size_t i = 0; i < params->paramCount; ++i) {
			total += cpgf::fromVariant<int>(*(params->params[i]));
		}

		return total;
	}

public:
	int value;
	float f;
};



#endif
