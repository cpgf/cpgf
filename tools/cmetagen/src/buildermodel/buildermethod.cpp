#include "buildermethod.h"
#include "builderfilewriter.h"
#include "codewriter/cppwriter.h"
#include "model/cppmethod.h"

#include "Poco/Format.h"


BuilderMethod::BuilderMethod(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icMethod);
}

BuilderMethod::~BuilderMethod()
{
}

const CppMethod * BuilderMethod::getCppMethod() const
{
	return static_cast<const CppMethod *>(this->getCppItem());
}

void BuilderMethod::doWriteMetaData(BuilderFileWriter * writer)
{
	const CppMethod * cppMethod = this->getCppMethod();
	CodeBlock * codeBlock = writer->getReflectionCodeBlock(cppMethod);

	std::string s;
	Poco::format(s, "%s(\"%s\", (%s)(&%s));", writer->getReflectionAction("_method"), cppMethod->getName(), cppMethod->getTextOfPointeredType(), cppMethod->getOutputName());

	codeBlock->addLine(s);
}

