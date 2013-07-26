#include "builderclass.h"
#include "buildermethod.h"
#include "builderwriter.h"
#include "builderutil.h"
#include "model/cppclass.h"
#include "model/cppmethod.h"
#include "codewriter/cppwriter.h"
#include "util.h"

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

void BuilderClass::doWriteMetaData(BuilderWriter * writer)
{
	this->doWriteBaseClasses(writer);
	
	if(this->getCppClass()->isNestedClass()) {
		this->doWriteAsNestedClass(writer);
	}
	
	if(this->shouldWrapClass()) {
		this->doWriteInheritedOverridableMethods(writer);
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
			string s = Poco::format("%s<%s >());",
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
	if(cppClass->isAnonymous() || cppClass->isTemplate()) {
		return;
	}

	BuilderSection * section;
	CodeBlock * codeBlock = writer->getParentReflectionCodeBlock(cppClass, &section);

	string s = Poco::format("%s(%s())",
		writer->getReflectionAction("_class"),
		getCreationFunctionName(writer->getBuilderContext(), section)
	);
	codeBlock->appendLine(s);
}

void BuilderClass::doWriteInheritedOverridableMethods(BuilderWriter * writer)
{
	const CppClass * cppClass = this->getCppClass();

	for(CppClass::BaseClassListType::const_iterator it = cppClass->getBaseClassList()->begin();
		it != cppClass->getBaseClassList()->end();
		++it) {
		const CppClass * baseClass = (*it)->getCppClass();

		for(CppClass::MethodListType::const_iterator methodIt = baseClass->getMethodList()->begin();
			methodIt != baseClass->getMethodList()->end();
			++methodIt) {
			const CppMethod * method = *methodIt;
			if(method->isVirtual()) {
				writeMethodClassWrapper(method, writer, cppClass);
			}
		}
	}
}


} // namespace metagen
