#include "util.h"
#include "config.h"

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

std::string removeQualifications(const std::string & name)
{
	static Poco::RegularExpression re("^.*\\b(\\w+)\\s*$");
	string result(name);
	re.subst(result, "$1");
	return result;
}

