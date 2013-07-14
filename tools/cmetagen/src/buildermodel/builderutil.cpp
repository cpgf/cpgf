#include "builderutil.h"
#include "model/cppitem.h"
#include "model/cppcontainer.h"
#include "util.h"
#include "config.h"

#include "Poco/Format.h"

using namespace std;


namespace metagen {

std::string getReflectionClassName(const Config * config)
{
	return config->getMetaDefineParamName() + "::ClassType";
}

std::string getReflectionScope(const CppItem * item)
{
	if(item->isGlobal()) {
		return "";
	}
	else {
		return getReflectionClassName(item->getConfig()) + "::";
	}
}

bool isVisibilityAllowed(ItemVisibility visibility, const Config * config)
{
	if((visibility == ivPublic) != config->doesAllowPublic()
		|| (visibility == ivProtected) != config->doesAllowProtected()
		|| (visibility == ivPrivate) != config->doesAllowPrivate()
		) {
		return false;
	}

	return true;
}

string getContainertName(const CppContainer * cppContainer)
{
	string result;

	if(cppContainer->isClass()) {
		result = normalizeSymbolName(cppContainer->getQualifiedName());
	}
	else {
		result = "Global";
	}

	return result;
}

string getIndexName(int fileIndex)
{
	string result;

	if(fileIndex > 0) {
		Poco::format(result, "_%d", fileIndex);
	}

	return result;
}

std::string getPartialCreationFunctionName(const Config * config, const CppContainer * cppContainer, int index)
{
	return normalizeSymbolName("partial_" + config->getCreationFunctionPrefix() + "_" + getContainertName(cppContainer) + getIndexName(index));
}

std::string getPartialCreationFunctionPrototype(const Config * config, const CppContainer * cppContainer, int index)
{
	string creationName = getPartialCreationFunctionName(config, cppContainer, index);
	return Poco::format("void %s(cpgf::GDefineMetaInfo metaInfo)", creationName);
}

std::string getReflectionFunctionName(const Config * config, const CppContainer * cppContainer, int index)
{
	return normalizeSymbolName(config->getReflectionFunctionPrefix() + "_" + getContainertName(cppContainer) + getIndexName(index));
}



} // namespace metagen

