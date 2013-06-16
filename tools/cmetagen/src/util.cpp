#include "util.h"

#include "model/cppparam.h"

#include "Poco/String.h"
#include "Poco/RegularExpression.h"

#include <algorithm>

using namespace std;
using namespace cpgf;


std::string normalizePath(const std::string & path)
{
	std::string result(path);
	std::replace(result.begin(), result.end(), '\\', '/');
	return result;
}

std::string removeLastToken(const std::string & s)
{
	static Poco::RegularExpression re("\\s*\\w+\\s*$");
	string result(s);
	re.subst(result, "");
	return result;
}

std::string removeAllAfterEqualSign(const std::string & s)
{
	static Poco::RegularExpression re("\\s*=.*$");
	string result(s);
	re.subst(result, "");
	return result;
}

std::string normalizeSymbolName(const std::string & name)
{
	string result = name;
	
	Poco::replaceInPlace(result, "::", "_");
	Poco::replaceInPlace(result, "__", "_");
	
	return result;
}

std::string paramListToString(const CppParamList * paramList, const GFlags<ParamListToStringOption> options)
{
	string result;

	int paramCount = paramList->getCount();
	for(int i = 0; i < paramCount; ++i) {
		if(i > 0) {
			result.append(",");
		}
		CppParam * param = paramList->get(i);
		if(options.has(poWithType)) {
			result.append(" ").append(param->getType()->getQualifiedName());
		}
		if(options.has(poWithName)) {
			result.append(" ").append(param->getName());
		}
		if(options.has(poWithDefaultValue) && param->hasDefaultValue()) {
			result.append(" = ").append(param->getDefaultValue());
		}
	}

	return result;
}
