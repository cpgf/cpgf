#include "builderutil.h"
#include "buildercontext.h"
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
		return getReflectionClassName(item->getProject()) + "::";
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

string getContainerQualifiedName(const BuilderContext * builderContext, const CppContainer * cppContainer)
{
	string result;

	if(cppContainer->isClass() || cppContainer->isNamespace()) {
		result = normalizeSymbolName(cppContainer->getQualifiedName());
	}
	else {
		result = builderContext->getSourceBaseFileName() + "_Global";
	}

	return result;
}

std::string getClassWrapperClassQualifiedName(const BuilderContext * builderContext, const CppContainer * cppContainer)
{
	return cppContainer->getQualifiedName() + builderContext->getProject()->getClassWrapperPostfix();
}

std::string getClassWrapperClassName(const BuilderContext * builderContext, const CppContainer * cppContainer)
{
	return cppContainer->getName() + builderContext->getProject()->getClassWrapperPostfix();
}

std::string getContainerOrClassWrapperClassName(CodeNameType nameType, const BuilderContext * builderContext, const CppContainer * cppContainer)
{
	switch(nameType) {
		case cntNormal:
			return cppContainer->getName();

		case cntClassWrapper:
			return getClassWrapperClassName(builderContext, cppContainer);
	}
	return "";
}

string getContainerOrClassWrapperQualifiedName(CodeNameType nameType, const BuilderContext * builderContext, const CppContainer * cppContainer)
{
	switch(nameType) {
		case cntNormal:
			return getContainerQualifiedName(builderContext, cppContainer);

		case cntClassWrapper:
			return getClassWrapperClassQualifiedName(builderContext, cppContainer);
	}
	return "";
}

string getSectionIndexName(int sectionIndex)
{
	string result;

	if(sectionIndex > 0) {
		Poco::format(result, "_%d", sectionIndex);
	}

	return result;
}

std::string getPartialCreationFunctionName(CodeNameType nameType, const BuilderContext * builderContext,
		const CppContainer * cppContainer, int index)
{
	return normalizeSymbolName(Poco::format("partial_%s_%s%s",
		builderContext->getProject()->getCreationFunctionPrefix(),
		getContainerOrClassWrapperQualifiedName(nameType, builderContext, cppContainer),
		getSectionIndexName(index)
		)
	);
}

std::string getPartialCreationFunctionPrototype(CodeNameType nameType, const BuilderContext * builderContext,
		const CppContainer * cppContainer, int index)
{
	string creationName = getPartialCreationFunctionName(nameType, builderContext, cppContainer, index);
	return Poco::format("void %s(cpgf::GDefineMetaInfo metaInfo)", creationName);
}

std::string getReflectionFunctionName(CodeNameType nameType, const BuilderContext * builderContext, const CppContainer * cppContainer,
									  int index)
{
	return normalizeSymbolName(Poco::format("%s_%s%s",
		builderContext->getProject()->getReflectionFunctionPrefix(),
		getContainerOrClassWrapperQualifiedName(nameType, builderContext, cppContainer),
		getSectionIndexName(index)
		)
	);
}

} // namespace metagen

