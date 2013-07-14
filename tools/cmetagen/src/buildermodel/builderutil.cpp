#include "builderutil.h"
#include "model/cppitem.h"
#include "model/cppcontainer.h"
#include "model/cppinvokable.h"
#include "model/cppenum.h"
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

size_t getCppItemPayload(const CppItem * item)
{
	if(item->isFile() || item->isNamespace()) {
		return 0;
	}
	int payload = 1;
	if(item->isInvokable()) {
		const CppInvokable * invokable = static_cast<const CppInvokable *>(item);
		for(size_t i = 0; i < invokable->getArity(); ++i) {
			if(invokable->paramHasDefaultValue(i)) {
				++payload;
			}
		}
	}
	else if(item->isEnum()) {
		payload += static_cast<const CppEnum *>(item)->getValueList()->size();
	}
	return payload;
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

string getSectionIndexName(int sectionIndex)
{
	string result;

	if(sectionIndex > 0) {
		Poco::format(result, "_%d", sectionIndex);
	}

	return result;
}

std::string getPartialCreationFunctionName(const Config * config, const CppContainer * cppContainer, int index)
{
	return normalizeSymbolName("partial_" + config->getCreationFunctionPrefix() + "_" + getContainertName(cppContainer) + getSectionIndexName(index));
}

std::string getPartialCreationFunctionPrototype(const Config * config, const CppContainer * cppContainer, int index)
{
	string creationName = getPartialCreationFunctionName(config, cppContainer, index);
	return Poco::format("void %s(cpgf::GDefineMetaInfo metaInfo)", creationName);
}

std::string getReflectionFunctionName(const Config * config, const CppContainer * cppContainer,
									  int index, const std::string & postfix)
{
	return normalizeSymbolName(config->getReflectionFunctionPrefix()
		+ "_"
		+ getContainertName(cppContainer)
		+ postfix
		+ getSectionIndexName(index));
}



} // namespace metagen

