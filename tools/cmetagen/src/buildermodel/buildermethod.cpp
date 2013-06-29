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

void BuilderMethod::doWriteMetaData(BuilderFileWriter * writer)
{
	const CppMethod * cppMethod = static_cast<const CppMethod *>(this->getCppItem());
	CodeBlock * codeBlock = writer->getMetaDataCodeBlock(cppMethod, BuilderFileWriter::ftHeader);

	std::string s;
	Poco::format(s, "%s(\"%s\", (%s)(&%s));", writer->getReflectionAction("_method"), cppMethod->getName(), cppMethod->getPointeredType(), cppMethod->getOutputName());

	codeBlock->addLine(s);
}

