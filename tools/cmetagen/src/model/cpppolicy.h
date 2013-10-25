#ifndef CPGF_CPPPOLICY_H
#define CPGF_CPPPOLICY_H

#include <string>
#include <set>

namespace metagen {

class CppPolicy
{
private:
	typedef std::set<std::string> RuleSetType;
	
public:
	void addRule(const std::string & rule);
	
	bool hasRule() const;
	
	std::string getTextOfMakePolicy(bool prefixWithComma);

private:
	RuleSetType ruleSet;
};

const std::string ruleCopyConstReference("GMetaRuleCopyConstReference");
const std::string ruleParamNoncopyable("GMetaRuleParamNoncopyable");
const std::string ruleDestructorAbsent("GMetaRuleDestructorAbsent");
const std::string ruleDefaultConstructorAbsent("GMetaRuleDefaultConstructorAbsent");
const std::string ruleCopyConstructorAbsent("GMetaRuleCopyConstructorAbsent");

std::string makeIndexedRule(const std::string & rule, int index);


} // namespace metagen


#endif

