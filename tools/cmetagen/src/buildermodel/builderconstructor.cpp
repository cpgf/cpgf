#include "builderconstructor.h"
#include "builderwriter.h"
#include "builderclass.h"
#include "builderutil.h"
#include "codewriter/cppwriter.h"
#include "codewriter/codeblock.h"
#include "model/cppconstructor.h"
#include "model/cppcontainer.h"
#include "model/cppclass.h"

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

void BuilderConstructor::doWriteMetaData(BuilderWriter * writer)
{
	this->doWriterReflection(writer);

	if(this->getCppItem()->getParent()->isClass() && static_cast<BuilderClass *>(this->getParent())->shouldWrapClass()) {
		this->doWriterClassWrapper(writer);
		this->doWriterClassWrapperReflection(writer);
	}
}

void BuilderConstructor::doWriterReflection(BuilderWriter * writer)
{
	const CppConstructor * cppConstructor = this->getCppConstructor();
	if(static_cast<const CppClass *>(cppConstructor->getParent())->isAbstract()) {
		return;
	}
	CodeBlock * codeBlock = writer->getParentReflectionCodeBlock(cppConstructor);
	this->doWriterReflectionCode(writer, codeBlock);
}

void BuilderConstructor::doWriterReflectionCode(BuilderWriter * writer, CodeBlock * codeBlock)
{
	const CppConstructor * cppConstructor = this->getCppConstructor();

	std::string s = Poco::format("%s<void * (%s)>(%s);",
		writer->getReflectionAction("_constructor"),
		cppConstructor->getTextOfParamList(itoWithArgType),
		getInvokablePolicyText(cppConstructor, false)
	);

	codeBlock->appendLine(s);
}

void BuilderConstructor::doWriterClassWrapper(BuilderWriter * writer)
{
	const CppConstructor * cppConstructor = this->getCppConstructor();
	CodeBlock * codeBlock = writer->getClassWrapperCodeBlock(cppConstructor, cppConstructor->getParent());
	codeBlock->appendLine(cppConstructor->getTextWithReplacedName(
		itoWithName | itoWithArgType | itoWithArgName | itoWithDefaultValue,
		getClassWrapperClassName(writer->getBuilderContext(), cppConstructor->getParent())
	));

	string s;
	codeBlock->incIndent();
	s = Poco::format(": super(%s) {}",
		cppConstructor->getTextOfParamList(itoWithArgName)
	);
	codeBlock->appendLine(s);
	codeBlock->decIndent();

	codeBlock->appendBlankLine();
}

void BuilderConstructor::doWriterClassWrapperReflection(BuilderWriter * writer)
{
	const CppConstructor * cppConstructor = this->getCppConstructor();
	CodeBlock * codeBlock = writer->getClassWrapperParentReflectionCodeBlock(cppConstructor, cppConstructor->getParent());
	this->doWriterReflectionCode(writer, codeBlock);
}


} // namespace metagen
