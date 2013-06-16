#include "builderfield.h"
#include "builderfilewriter.h"
#include "model/cppfield.h"
#include "model/cpptype.h"
#include "codewriter/cppwriter.h"

#include "Poco/Format.h"

using namespace std;


BuilderField::BuilderField(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icField);
}

BuilderField::~BuilderField()
{
}

void BuilderField::doWriteMetaData(BuilderFileWriter * writer)
{
	const CppField * cppField = static_cast<const CppField *>(this->getCppItem());
	CodeBlock * codeBlock = writer->getMetaDataCodeBlock(cppField, BuilderFileWriter::ftHeader);

	string s;
	Poco::format(s, "%s(\"%s\", &%s);", writer->getReflectionAction("_field"), cppField->getName(), cppField->getQualifiedName());

	codeBlock->addLine(s);
}

