#include "buildermethod.h"
#include "builderwriter.h"
#include "builderclass.h"
#include "codewriter/cppwriter.h"
#include "model/cppmethod.h"
#include "model/cppcontainer.h"
#include "builderutil.h"
#include "project.h"

#include "Poco/Format.h"

using namespace std;

namespace metagen {


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

void BuilderMethod::doWriteMetaData(BuilderWriter * writer)
{
	this->doWriterReflection(writer);

	const CppMethod * cppMethod = this->getCppMethod();
	if(cppMethod->isVirtual()
		&& this->getCppItem()->getParent()->isClass()
		&& static_cast<BuilderClass *>(this->getParent())->shouldWrapClass()) {
		this->doWriterClassWrapper(writer);
		this->doWriterClassWrapperReflection(writer);
	}
}

void BuilderMethod::doWriterReflection(BuilderWriter * writer)
{
	const CppMethod * cppMethod = this->getCppMethod();
	CodeBlock * codeBlock = writer->getParentReflectionCodeBlock(cppMethod);
	this->doWriterReflectionCode(writer, codeBlock, cppMethod->getName());
}

void BuilderMethod::doWriterReflectionCode(BuilderWriter * writer, CodeBlock * codeBlock,
										   const std::string & methodName)
{
	const CppMethod * cppMethod = this->getCppMethod();

	size_t arity = cppMethod->getArity();
	bool hasDefaultValue = (arity > 0 && cppMethod->paramHasDefaultValue(arity - 1));

	std::string s;

	if(cppMethod->isOverloaded()) {
		s = Poco::format("%s(\"%s\", (%s)(&%s%s))",
			writer->getReflectionAction("_method"),
			methodName,
			cppMethod->getTextOfPointeredType(),
			getReflectionScope(cppMethod),
			methodName,
			string(hasDefaultValue ? "" : ";")
		);
	}
	else {
		s = Poco::format("%s(\"%s\", &%s%s)",
			writer->getReflectionAction("_method"),
			methodName,
			getReflectionScope(cppMethod),
			methodName,
			string(hasDefaultValue ? "" : ";")
		);
	}
	codeBlock->appendLine(s);

	if(hasDefaultValue) {
		this->doWriterDefaultParameterReflection(codeBlock->appendBlock(cbsIndent));
		codeBlock->appendLine(";");
	}
}

void BuilderMethod::doWriterDefaultParameterReflection(CodeBlock * codeBlock)
{
	const CppMethod * cppMethod = this->getCppMethod();
	size_t arity = cppMethod->getArity();
	while(arity != 0) {
		--arity;
		if(! cppMethod->paramHasDefaultValue(arity)) {
			break;
		}

		string defaultValue = cppMethod->getTextOfParamDeafultValue(arity);
		bool shouldSafeCopy = false;
		CppType type = cppMethod->getParamType(arity).getNonReferenceType();
		if(! type.isPointer()) {
			if(! type.isFundamental()) {
				shouldSafeCopy = true;
			}
		}
		string s;
		if(shouldSafeCopy) {
			s = Poco::format("._default(copyVariantFromCopyable(%s))", defaultValue);
		}
		else {
			s = Poco::format("._default(%s)", defaultValue);
		}
		codeBlock->appendLine(s);
	}
}

void BuilderMethod::doWriterClassWrapper(BuilderWriter * writer)
{
	const CppMethod * cppMethod = this->getCppMethod();
	CodeBlock * codeBlock = writer->getClassWrapperCodeBlock(cppMethod);

	const ItemTextOption options = ItemTextOption(itoWithName | itoWithResult | itoWithQualifiers
		| itoWithArgType | itoWithArgName | itoWithDefaultValue);
	string s;

	s = cppMethod->getText(options);
	codeBlock->appendLine(s);
	this->doWriterClassWrapperMethodBody(codeBlock->appendBlock(cbsBracketAndIndent));

	s = cppMethod->getTextWithReplacedName(options, writer->getProject()->getClassWrapperSuperPrefix() + cppMethod->getName());
	codeBlock->appendLine(s);
	this->doWriterClassWrapperCallSuperMethod(codeBlock->appendBlock(cbsBracketAndIndent));
}

void BuilderMethod::doWriterClassWrapperMethodBody(CodeBlock * codeBlock)
{
	const CppMethod * cppMethod = this->getCppMethod();
	string s;

	s = Poco::format("cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction(\"%s\"));",
		cppMethod->getName()
		);
	codeBlock->appendLine(s);

	codeBlock->appendLine("if(func)");
	CodeBlock * bodyBlock = codeBlock->appendBlock(cbsBracketAndIndent);
	s = "cpgf::invokeScriptFunction(func.get(), this";
	if(cppMethod->getArity() > 0) {
		s = Poco::format("%s, %s", s, cppMethod->getTextOfParamList(itoWithArgName));
	}
	s.append(");");
	if(cppMethod->hasResult()) {
		bodyBlock->appendLine("return " + s);
	}
	else {
		bodyBlock->appendLine(s);
		bodyBlock->appendLine("return;");
	}

	this->doWriterClassWrapperCallSuperMethod(codeBlock);
}

void BuilderMethod::doWriterClassWrapperCallSuperMethod(CodeBlock * codeBlock)
{
	const CppMethod * cppMethod = this->getCppMethod();
	if(cppMethod->isPureVirtual()) {
		codeBlock->appendLine("throw \"Abstract method\";");
	}
	else {
		string s = Poco::format("super::%s(%s);", cppMethod->getName(), cppMethod->getTextOfParamList(itoWithArgName));
		if(cppMethod->hasResult()) {
			s = "return " + s;
		}
		codeBlock->appendLine(s);
	}
}

void BuilderMethod::doWriterClassWrapperReflection(BuilderWriter * writer)
{
	const CppMethod * cppMethod = this->getCppMethod();
	CodeBlock * codeBlock = writer->getClassWrapperParentReflectionCodeBlock(cppMethod);
	this->doWriterReflectionCode(writer, codeBlock, writer->getProject()->getClassWrapperSuperPrefix() + cppMethod->getName());
}


} // namespace metagen
