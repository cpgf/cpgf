#include "builderfield.h"
#include "builderfilewriter.h"
#include "model/cppfield.h"
#include "model/cpptype.h"
#include "model/cppcontainer.h"
#include "codewriter/cppwriter.h"
#include "config.h"
#include "util.h"

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

const CppField * BuilderField::getCppField() const
{
	return static_cast<const CppField *>(this->getCppItem());
}
		
bool BuilderField::canBind() const
{
	if(this->getCppField()->isBitField() && ! this->getConfig()->shouldWrapBitFields()) {
		return false;
	}
	
	return super::canBind();
}

void BuilderField::doWriteMetaData(BuilderFileWriter * writer)
{
	if(this->getCppField()->isBitField()) {
		this->doWriteBitFieldWrapper(writer);
	}
	else {
		this->doWriteReflection(writer);
	}
}

void BuilderField::doWriteReflection(BuilderFileWriter * writer)
{
	const CppField * cppField = this->getCppField();
	CodeBlock * codeBlock = writer->getReflectionCodeBlock(cppField);

	std::string s;
	Poco::format(s, "%s(\"%s\", &%s);", writer->getReflectionAction("_field"), cppField->getName(), cppField->getQualifiedName());

	codeBlock->addLine(s);
}

void BuilderField::doWriteBitFieldWrapper(BuilderFileWriter * writer)
{
	const CppField * cppField = this->getCppField();
	string bitFieldWrapperPrefix = "b1tF1EldWrapPer_" + normalizeSymbolName(cppField->getQualifiedName());
	string bitFieldWrapperGetter = bitFieldWrapperPrefix + "_getter";
	string bitFieldWrapperSetter = bitFieldWrapperPrefix + "_setter";
	string self = cppField->getParent()->getOutputName() + " * self";

	CppType fieldType(cppField->getType());
	string s;

	// getter
	CodeBlock * getterBlock = writer->getWrapperCodeBlock(cppField, ftHeader)->addBlock();
	s = bitFieldWrapperGetter + "(" + self + ")";
	s = "inline " + fieldType.getQualifiedName(s);
	getterBlock->addLine(s);
	CodeBlock * getterBody = getterBlock->addBlock(cbsBracketAndIndent);
	s = "return self->" + cppField->getName()+ ";";
	getterBody->addLine(s);

	// setter
	CodeBlock * setterBlock = writer->getWrapperCodeBlock(cppField, ftHeader)->addBlock();
	s = "inline void " + bitFieldWrapperGetter + "(" + self + ", " + fieldType.getQualifiedName("value") + ")";
	setterBlock->addLine(s);
	CodeBlock * setterBody = setterBlock->addBlock(cbsBracketAndIndent);
	s = "self->" + cppField->getName()+ " = value;";
	setterBody->addLine(s);

	// reflection
	s = "";
	CodeBlock * codeBlock = writer->getReflectionCodeBlock(cppField);
	Poco::format(s, "%s(\"%s\", &%s, &%s, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());",
		writer->getReflectionAction("_property"),
		cppField->getName(),
		bitFieldWrapperGetter,
		bitFieldWrapperSetter
	);
	codeBlock->addLine(s);
}

