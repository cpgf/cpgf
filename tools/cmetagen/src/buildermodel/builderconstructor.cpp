#include "builderconstructor.h"
#include "builderfilewriter.h"
#include "builderclass.h"
#include "codewriter/cppwriter.h"
#include "model/cppconstructor.h"
#include "model/cppcontainer.h"

#include "Poco/Format.h"

using namespace std;


namespace metagen {

BuilderConstructor::BuilderConstructor(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icConstructor);
}

BuilderConstructor::~BuilderConstructor()
{
}

const CppConstructor * BuilderConstructor::getCppConstructor() const
{
	return static_cast<const CppConstructor *>(this->getCppItem());
}

void BuilderConstructor::doWriteMetaData(BuilderFileWriter * writer)
{
	this->doWriterReflection(writer);

	if(this->getCppItem()->getParent()->isClass() && static_cast<BuilderClass *>(this->getParent())->shouldWrapClass()) {
		this->doWriterClassWrapper(writer);
	}
}

void BuilderConstructor::doWriterReflection(BuilderFileWriter * writer)
{
	const CppConstructor * cppConstructor = this->getCppConstructor();
	CodeBlock * codeBlock = writer->getParentReflectionCodeBlock(cppConstructor);

	std::string s = Poco::format("%s<void * (%s)>());",
		writer->getReflectionAction("_constructor"),
		cppConstructor->getTextOfParamList(itoWithArgType)
	);

	codeBlock->appendLine(s);
}

void BuilderConstructor::doWriterClassWrapper(BuilderFileWriter * writer)
{
	const CppConstructor * cppConstructor = this->getCppConstructor();
	CodeBlock * codeBlock = writer->getWrapperClassCodeBlock(cppConstructor);
	string s;
	s = Poco::format("%s(%s)",
		cppConstructor->getParent()->getName(),
		cppConstructor->getTextOfParamList(itoWithArgType | itoWithArgName | itoWithDefaultValue)
	);
	codeBlock->appendLine(s);

	codeBlock->incIndent();
	s = Poco::format(": super(%s) {}",
		cppConstructor->getTextOfParamList(itoWithArgName)
	);
	codeBlock->appendLine(s);
	codeBlock->decIndent();

	codeBlock->appendBlankLine();
}


} // namespace metagen
