#include "builderutil.h"
#include "model/cppitem.h"
#include "model/cppcontainer.h"
#include "model/cppinvokable.h"
#include "model/cppenum.h"
#include "util.h"
#include "project.h"

#include "Poco/Format.h"

using namespace std;


namespace metagen {

std::string getReflectionClassName(const Project * project)
{
	return project->getMetaDefineParamName() + "::ClassType";
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

bool isVisibilityAllowed(ItemVisibility visibility, const Project * project)
{
	if((visibility == ivPublic) != project->doesAllowPublic()
		|| (visibility == ivProtected) != project->doesAllowProtected()
		|| (visibility == ivPrivate) != project->doesAllowPrivate()
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

std::string getPartialCreationFunctionName(const Project * project, const CppContainer * cppContainer, int index)
{
	return normalizeSymbolName("partial_" + project->getCreationFunctionPrefix() + "_" + getContainertName(cppContainer) + getSectionIndexName(index));
}

std::string getPartialCreationFunctionPrototype(const Project * project, const CppContainer * cppContainer, int index)
{
	string creationName = getPartialCreationFunctionName(project, cppContainer, index);
	return Poco::format("void %s(cpgf::GDefineMetaInfo metaInfo)", creationName);
}

std::string getReflectionFunctionName(const Project * project, const CppContainer * cppContainer,
									  int index, const std::string & postfix)
{
	return normalizeSymbolName(project->getReflectionFunctionPrefix()
		+ "_"
		+ getContainertName(cppContainer)
		+ postfix
		+ getSectionIndexName(index));
}



} // namespace metagen

