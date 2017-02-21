#ifndef CPGF_METAGEN_OPERATOR_LOGIC_H
#define CPGF_METAGEN_OPERATOR_LOGIC_H

#include <string>

class MetagenOperatorLogic
{
public:
	MetagenOperatorLogic() : value(0) {}
	explicit MetagenOperatorLogic(unsigned int value) : value(value) {}
	
	bool operator > (unsigned int n) const { return value > n; }
	bool operator > (const MetagenOperatorLogic & other) const { return value > other.value; }
	bool operator > (const std::string & s) const { return value > s.length(); }
	
	bool operator >= (unsigned int n) const { return value >= n; }
	bool operator >= (const MetagenOperatorLogic & other) const { return value >= other.value; }
	bool operator >= (const std::string & s) const { return value >= s.length(); }

	bool operator < (unsigned int n) const { return value < n; }
	bool operator < (const MetagenOperatorLogic & other) const { return value < other.value; }
	bool operator < (const std::string & s) const { return value < s.length(); }

	bool operator <= (unsigned int n) const { return value <= n; }
	bool operator <= (const MetagenOperatorLogic & other) const { return value <= other.value; }
	bool operator <= (const std::string & s) const { return value <= s.length(); }

	bool operator && (unsigned int n) const { return value && n; }
	bool operator && (const MetagenOperatorLogic & other) const { return value && other.value; }
	bool operator && (const std::string & s) const { return value && s.length(); }

	bool operator || (unsigned int n) const { return value || n; }
	bool operator || (const MetagenOperatorLogic & other) const { return value || other.value; }
	bool operator || (const std::string & s) const { return value || s.length(); }

	bool operator ! () const { return !value; }

public:
	unsigned int value;
};



#endif
