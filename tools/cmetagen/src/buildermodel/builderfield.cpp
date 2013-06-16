#include "builderfield.h"
#include "builderfilewriter.h"
#include "model/cppfield.h"
#include "codewriter/cppwriter.h"

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

	string s = "FIELD---";
	s.append(cppField->getName());
	codeBlock->addLine(s);
}

