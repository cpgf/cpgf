#include "builderfield.h"
#include "builderwriter.h"
#include "model/cppfield.h"
#include "model/cpptype.h"
#include "model/cppcontainer.h"
#include "codewriter/cppwriter.h"
#include "codewriter/codeblock.h"
#include "project.h"
#include "util.h"
#include "builderutil.h"

#include "Poco/Format.h"

using namespace std;

namespace metagen {


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
		
bool BuilderField::doCanBind() const
{
	if(this->getCppField()->isBitField() && ! this->getProject()->shouldWrapBitFields()) {
		return false;
	}
	
	if(this->getCppField()->isAnonymousStructOrUnion()) {
		return false;
	}
	
	return super::doCanBind();
}

void BuilderField::doWriteMetaData(BuilderWriter * writer)
{
	if(this->getCppField()->isBitField()) {
		this->doWriteBitFieldWrapper(writer);
	}
	else {
		this->doWriteReflection(writer);
	}
}

void BuilderField::doWriteReflection(BuilderWriter * writer)
{
	const CppField * cppField = this->getCppField();
	CodeBlock * codeBlock = writer->getParentReflectionCodeBlock(cppField);

	std::string s = Poco::format("%s(\"%s\", &%s%s);",
		writer->getReflectionAction("_field"),
		cppField->getName(),
		getReflectionScope(cppField, false),
		cppField->getName()
	);

	codeBlock->appendLine(s);
}

void BuilderField::doWriteBitFieldWrapper(BuilderWriter * writer)
{
	const CppField * cppField = this->getCppField();
	string bitFieldWrapperPrefix = "b1tF1EldWrapPer_" + normalizeSymbolName(cppField->getQualifiedName());
	string bitFieldWrapperGetter = bitFieldWrapperPrefix + "_getter";
	string bitFieldWrapperSetter = bitFieldWrapperPrefix + "_setter";
	string self = "T * self";

	CppType fieldType(cppField->getType());
	string s;

	// getter
	CodeBlock * getterBlock = writer->createBitFieldWrapperCodeBlock(cppField)->appendBlock();
	getterBlock->appendLine("template <typename T>");
	s = bitFieldWrapperGetter + "(" + self + ")";
	s = "inline " + fieldType.getQualifiedName(s);
	getterBlock->appendLine(s);
	CodeBlock * getterBody = getterBlock->appendBlock(cbsBracketAndIndent);
	s = "return self->" + cppField->getName()+ ";";
	getterBody->appendLine(s);

	// setter
	CodeBlock * setterBlock = writer->createBitFieldWrapperCodeBlock(cppField)->appendBlock();
	setterBlock->appendLine("template <typename T>");
	s = "inline void " + bitFieldWrapperSetter + "(" + self + ", " + fieldType.getQualifiedName("value") + ")";
	setterBlock->appendLine(s);
	CodeBlock * setterBody = setterBlock->appendBlock(cbsBracketAndIndent);
	s = "self->" + cppField->getName()+ " = value;";
	setterBody->appendLine(s);

	// reflection
	string templateParam = "<" + getReflectionClassName(this->getProject(), true) + ">";
	CodeBlock * codeBlock = writer->getParentReflectionCodeBlock(cppField);
	s = Poco::format("%s(\"%s\", &%s, &%s, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());",
		writer->getReflectionAction("_property"),
		cppField->getName(),
		bitFieldWrapperGetter + templateParam,
		bitFieldWrapperSetter + templateParam
	);
	codeBlock->appendLine(s);
}


} // namespace metagen
