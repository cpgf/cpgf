#ifndef CPGF_METAGEN_OPERATOR_UNARY_H
#define CPGF_METAGEN_OPERATOR_UNARY_H

#include <string>

class MetagenOperatorUnary
{
public:
	MetagenOperatorUnary() : value(0) {}
	explicit MetagenOperatorUnary(int value) : value(value) {}
	
	MetagenOperatorUnary operator + () const { return MetagenOperatorUnary(value > 0 ? value : -value); }
	MetagenOperatorUnary operator - () const { return MetagenOperatorUnary(-value); }
	
	MetagenOperatorUnary operator ++ () { return MetagenOperatorUnary(++value); }
	MetagenOperatorUnary operator ++ (int) { MetagenOperatorUnary v(value); value++; return v; }

	MetagenOperatorUnary operator -- () { return MetagenOperatorUnary(--value); }
	MetagenOperatorUnary operator -- (int) { MetagenOperatorUnary v(value); value--; return v; }

public:
	int value;
};



#endif
