#include "cpppolicy.h"

#include "Poco/Format.h"
#include "Poco/String.h"

using namespace std;

namespace metagen {

void CppPolicy::addRule(const std::string & rule)
{
	this->ruleSet.insert(rule);
}

bool CppPolicy::hasRule() const
{
	return ! this->ruleSet.empty();
}

std::string CppPolicy::getTextOfMakePolicy(bool prefixWithComma)
{
	string result;
	
	if(this->hasRule()) {
		result = Poco::format("cpgf::MakePolicy<%s >",
			Poco::cat(string(", "), this->ruleSet.begin(), this->ruleSet.end()));

		if(prefixWithComma && ! result.empty()) {
			result = ", " + result;
		}
	}
	
	return result;
}

std::string makeIndexedRule(const std::string & rule, int index)
{
	return Poco::format("%s<%d>", rule, index);
}


} // namespace metagen

