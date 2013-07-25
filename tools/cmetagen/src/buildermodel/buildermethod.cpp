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


void doWriterReflection(const CppMethod * cppMethod, BuilderWriter * writer);
void doWriterDefaultParameterReflection(const CppMethod * cppMethod, CodeBlock * codeBlock);
void doWriterReflectionCode(const CppMethod * cppMethod, BuilderWriter * writer, CodeBlock * codeBlock,
										   const std::string & methodName);
void doWriterClassWrapper(const CppMethod * cppMethod, BuilderWriter * writer);
void doWriterClassWrapperCallSuperMethod(const CppMethod * cppMethod, CodeBlock * codeBlock);
void doWriterClassWrapperMethodBody(const CppMethod * cppMethod, CodeBlock * codeBlock);
void doWriterClassWrapperReflection(const CppMethod * cppMethod, BuilderWriter * writer);


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
	const CppMethod * cppMethod = this->getCppMethod();

	doWriterReflection(cppMethod, writer);

	if(cppMethod->isVirtual()
		&& this->getCppItem()->getParent()->isClass()
		&& static_cast<BuilderClass *>(this->getParent())->shouldWrapClass()) {
		doWriterClassWrapper(cppMethod, writer);
		doWriterClassWrapperReflection(cppMethod, writer);
	}
}

void doWriterReflection(const CppMethod * cppMethod, BuilderWriter * writer)
{
	CodeBlock * codeBlock = writer->getParentReflectionCodeBlock(cppMethod);
	doWriterReflectionCode(cppMethod, writer, codeBlock, cppMethod->getName());
}

void doWriterClassWrapper(const CppMethod * cppMethod, BuilderWriter * writer)
{
	CodeBlock * codeBlock = writer->getClassWrapperCodeBlock(cppMethod, cppMethod->getParent());

	const ItemTextOption options = ItemTextOption(itoWithName | itoWithResult | itoWithQualifiers
		| itoWithArgType | itoWithArgName | itoWithDefaultValue);
	string s;

	s = cppMethod->getText(options);
	codeBlock->appendLine(s);
	doWriterClassWrapperMethodBody(cppMethod, codeBlock->appendBlock(cbsBracketAndIndent));

	s = cppMethod->getTextWithReplacedName(options, writer->getProject()->getClassWrapperSuperPrefix() + cppMethod->getName());
	codeBlock->appendLine(s);
	doWriterClassWrapperCallSuperMethod(cppMethod, codeBlock->appendBlock(cbsBracketAndIndent));
}

void doWriterDefaultParameterReflection(const CppMethod * cppMethod, CodeBlock * codeBlock)
{
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

void doWriterReflectionCode(const CppMethod * cppMethod, BuilderWriter * writer, CodeBlock * codeBlock,
										   const std::string & methodName)
{
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
		doWriterDefaultParameterReflection(cppMethod, codeBlock->appendBlock(cbsIndent));
		codeBlock->appendLine(";");
	}
}

void doWriterClassWrapperCallSuperMethod(const CppMethod * cppMethod, CodeBlock * codeBlock)
{
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

void doWriterClassWrapperMethodBody(const CppMethod * cppMethod, CodeBlock * codeBlock)
{
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

	doWriterClassWrapperCallSuperMethod(cppMethod, codeBlock);
}

void doWriterClassWrapperReflection(const CppMethod * cppMethod, BuilderWriter * writer)
{
	CodeBlock * codeBlock = writer->getClassWrapperParentReflectionCodeBlock(cppMethod);
	doWriterReflectionCode(cppMethod, writer, codeBlock,
		writer->getProject()->getClassWrapperSuperPrefix() + cppMethod->getName());
}



} // namespace metagen
