#include "builderclass.h"
#include "builderfilewriter.h"
#include "builderutil.h"
#include "model/cppclass.h"
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

void BuilderClass::doWriteMetaData(BuilderFileWriter * writer)
{
	this->doWriteBaseClasses(writer);
	
	if(this->getCppClass()->isNestedClass()) {
		this->doWriteAsNestedClass(writer);
	}
}

void BuilderClass::doWriteBaseClasses(BuilderFileWriter * writer)
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

void BuilderClass::doWriteAsNestedClass(BuilderFileWriter * writer)
{
	const CppClass * cppClass = this->getCppClass();
	if(cppClass->isAnonymous() || cppClass->isTemplate()) {
		return;
	}

	CodeBlock * codeBlock = writer->getParentReflectionCodeBlock(cppClass);

	string s = Poco::format("%s(%s())",
		writer->getReflectionAction("_class"),
		writer->getCreationFunctionName(cppClass)
	);
	codeBlock->appendLine(s);
}


} // namespace metagen
