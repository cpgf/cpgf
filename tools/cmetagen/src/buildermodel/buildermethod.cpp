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


void writeMethodReflection(const CppMethod * cppMethod, BuilderWriter * writer);
void writeMethodDefaultParameterReflection(const CppMethod * cppMethod, CodeBlock * codeBlock);
void writeMethodReflectionCode(const CppMethod * cppMethod, BuilderWriter * writer, CodeBlock * codeBlock,
										   const std::string & methodName);
void writeMethodClassWrapperCallSuperMethod(const CppMethod * cppMethod, CodeBlock * codeBlock);
void writeMethodClassWrapperMethodBody(const CppMethod * cppMethod, CodeBlock * codeBlock);
void writeMethodClassWrapperReflection(const CppMethod * cppMethod, BuilderWriter * writer, const CppContainer * container);


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

	writeMethodReflection(cppMethod, writer);

	if(cppMethod->isVirtual()
		&& this->getCppItem()->getParent()->isClass()
		&& static_cast<BuilderClass *>(this->getParent())->shouldWrapClass()) {
		writeMethodClassWrapper(cppMethod, writer, cppMethod->getParent());
	}
}

void writeMethodReflection(const CppMethod * cppMethod, BuilderWriter * writer)
{
	CodeBlock * codeBlock = writer->getParentReflectionCodeBlock(cppMethod);
	writeMethodReflectionCode(cppMethod, writer, codeBlock, cppMethod->getName());
}

void writeMethodClassWrapper(const CppMethod * cppMethod, BuilderWriter * writer, const CppContainer * container)
{
	CodeBlock * codeBlock = writer->getClassWrapperCodeBlock(cppMethod, container);

	const ItemTextOption options = ItemTextOption(itoWithName | itoWithResult | itoWithQualifiers
		| itoWithArgType | itoWithArgName | itoWithDefaultValue);
	string s;

	s = cppMethod->getText(options);
	codeBlock->appendLine(s);
	writeMethodClassWrapperMethodBody(cppMethod, codeBlock->appendBlock(cbsBracketAndIndent));

	s = cppMethod->getTextWithReplacedName(options, writer->getProject()->getClassWrapperSuperPrefix() + cppMethod->getName());
	codeBlock->appendLine(s);
	writeMethodClassWrapperCallSuperMethod(cppMethod, codeBlock->appendBlock(cbsBracketAndIndent));

	writeMethodClassWrapperReflection(cppMethod, writer, container);
}

void writeMethodDefaultParameterReflection(const CppMethod * cppMethod, CodeBlock * codeBlock)
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

void writeMethodReflectionCode(const CppMethod * cppMethod, BuilderWriter * writer, CodeBlock * codeBlock,
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
		writeMethodDefaultParameterReflection(cppMethod, codeBlock->appendBlock(cbsIndent));
		codeBlock->appendLine(";");
	}
}

void writeMethodClassWrapperCallSuperMethod(const CppMethod * cppMethod, CodeBlock * codeBlock)
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

void writeMethodClassWrapperMethodBody(const CppMethod * cppMethod, CodeBlock * codeBlock)
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

	writeMethodClassWrapperCallSuperMethod(cppMethod, codeBlock);
}

void writeMethodClassWrapperReflection(const CppMethod * cppMethod, BuilderWriter * writer, const CppContainer * container)
{
	CodeBlock * codeBlock = writer->getClassWrapperParentReflectionCodeBlock(cppMethod, container);
	writeMethodReflectionCode(cppMethod, writer, codeBlock,
		writer->getProject()->getClassWrapperSuperPrefix() + cppMethod->getName());
}


} // namespace metagen
