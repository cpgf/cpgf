#include "builderclass.h"
#include "buildermethod.h"
#include "builderwriter.h"
#include "builderutil.h"
#include "model/cppclass.h"
#include "model/cppmethod.h"
#include "codewriter/cppwriter.h"
#include "codewriter/codeblock.h"
#include "util.h"
#include "project.h"

#include "Poco/Format.h"

#include <string>

using namespace std;


namespace metagen {


BuilderClass::BuilderClass(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icClass);
}

BuilderClass::~BuilderClass()
{
}

const CppClass * BuilderClass::getCppClass() const
{
	return static_cast<const CppClass *>(this->getCppItem());
}

void writedOverridableMethods(BuilderWriter * writer, const CppClass * cppClass, const Project * project);
void BuilderClass::doWriteMetaData(BuilderWriter * writer)
{
	this->doWriteBaseClasses(writer);
	
	if(this->getCppClass()->isNestedClass()) {
		this->doWriteAsNestedClass(writer);
	}
	
	if(this->shouldWrapClass()) {
		writedOverridableMethods(writer, this->getCppClass(), this->getProject());
	}
}

void BuilderClass::doWriteBaseClasses(BuilderWriter * writer)
{
	const CppClass * cppClass = this->getCppClass();
	CodeBlock * codeBlock = writer->getContainerReflectionCodeBlock(cppClass);
	
	for(CppClass::BaseClassListType::const_iterator it = cppClass->getBaseClassList()->begin();
		it != cppClass->getBaseClassList()->end();
		++it) {
		if(isVisibilityAllowed((*it)->getVisibility(), this->getProject())) {
			string s = Poco::format("%s<%s >();",
				writer->getReflectionAction("_base"),
				(*it)->getQualifiedName()
			);
			codeBlock->appendLine(s);
		}
	}
}

void BuilderClass::doWriteAsNestedClass(BuilderWriter * writer)
{
	const CppClass * cppClass = this->getCppClass();
	if(cppClass->isAnonymous() || cppClass->isChainedTemplate()) {
		return;
	}

	CodeBlock * codeBlock = writer->getParentReflectionCodeBlock(cppClass, NULL);

	string s = Poco::format("%s(%s());",
		writer->getReflectionAction("_class"),
		normalizeSymbolName(this->getProject()->getCreationFunctionPrefix()
			+ getCppClassNormalizedSymboName(cppClass))
	);
	codeBlock->appendLine(s);
}

void collectOverridableMethods(map<string, const CppMethod *> * methodMap, const CppClass * cppClass, const Project * project)
{
	for(CppClass::MethodListType::const_iterator methodIt = cppClass->getMethodList()->begin();
		methodIt != cppClass->getMethodList()->end();
		++methodIt) {
		const CppMethod * method = *methodIt;
		if(method->isVirtual() && isVisibilityAllowed(method->getVisibility(), project)) {
			methodMap->insert(make_pair(method->getName() + method->getTextOfPointeredType(false), method));
		}
	}

	for(CppClass::BaseClassListType::const_iterator it = cppClass->getBaseClassList()->begin();
		it != cppClass->getBaseClassList()->end();
		++it) {
		const CppClass * baseClass = (*it)->getCppClass();

		if(isVisibilityAllowed(baseClass->getVisibility(), project)) {
			collectOverridableMethods(methodMap, baseClass, project);
		}
	}
}

void writedOverridableMethods(BuilderWriter * writer, const CppClass * cppClass, const Project * project)
{
	map<string, const CppMethod *> methodMap;
	collectOverridableMethods(&methodMap, cppClass, project);
	for(map<string, const CppMethod *>::iterator it = methodMap.begin(); it != methodMap.end(); ++it) {
		writeMethodClassWrapper(it->second, writer, cppClass);
	}
}


} // namespace metagen
