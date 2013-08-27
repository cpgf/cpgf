#include "buildermethod.h"
#include "builderwriter.h"
#include "builderclass.h"
#include "codewriter/cppwriter.h"
#include "codewriter/codeblock.h"
#include "model/cppmethod.h"
#include "model/cppcontainer.h"
#include "builderutil.h"
#include "project.h"
#include "logger.h"

#include "Poco/Format.h"

using namespace std;

namespace metagen {


void writeMethodReflection(const CppMethod * cppMethod, BuilderWriter * writer);
void writeMethodDefaultParameterReflection(const CppMethod * cppMethod, CodeBlock * codeBlock);
void writeMethodReflectionCode(const CppMethod * cppMethod, BuilderWriter * writer, CodeBlock * codeBlock,
										   const string & methodName);
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

bool BuilderMethod::canBind() const
{
	if(this->doCanBind()) {
		if(isVisibilityAllowed(this->getCppItem()->getVisibility(), this->getProject())) {
			return true;
		}
		if(this->shouldWrapClass() && this->getCppItem()->getVisibility() == ivProtected) {
			return true;
		}
	}
	
	return false;
}
void BuilderMethod::doWriteMetaData(BuilderWriter * writer)
{
	const CppMethod * cppMethod = this->getCppMethod();

	writeMethodReflection(cppMethod, writer);
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
										   const string & methodName)
{
	size_t arity = cppMethod->getArity();
	bool hasDefaultValue = (arity > 0 && cppMethod->paramHasDefaultValue(arity - 1));

	string s, t;

	s = Poco::format("%s(\"%s\"",
		writer->getReflectionAction("_method"),
		methodName);
	t = Poco::format("&%s%s)%s",
		getReflectionScope(cppMethod, false),
		methodName,
		getInvokablePolicyText(cppMethod, true)
	);
	if(cppMethod->isOverloaded()) {
		if(cppMethod->isArityUnique()) {
			s = Poco::format("%s, cpgf::selectFunctionByArity%d(%s)", s, (int)(cppMethod->getArity()), t);
		}
		else {
			if(cppMethod->hasTemplateDependentParam()) {
				getLogger().warn(
					Poco::format("Function %s is overloaded and has template dependent argument, meta data can't be generated.\n",
					cppMethod->getTextOfPointeredType(true))
				);
//				return;
			}
			s = Poco::format("%s, (%s)(%s)", s, cppMethod->getTextOfPointeredType(true), t);
		}
	}
	else {
		s = Poco::format("%s, %s", s, t);
	}

	if(! hasDefaultValue) {
		s.append(";");
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
		codeBlock->appendLine(cppMethod->getTextOfUnusedParamsPlaceholder());
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

	s = Poco::format("cpgf::GScopedInterface<cpgf::IScriptFunction> _Fu0Nc(this->getScriptFunction(\"%s\"));",
		cppMethod->getName()
		);
	codeBlock->appendLine(s);

	codeBlock->appendLine("if(_Fu0Nc)");
	CodeBlock * bodyBlock = codeBlock->appendBlock(cbsBracketAndIndent);
	s = "cpgf::invokeScriptFunction(_Fu0Nc.get(), this";
	if(cppMethod->getArity() > 0) {
		s = Poco::format("%s, %s", s, cppMethod->getTextOfParamList(itoWithArgName));
	}
	s.append(")");
	if(cppMethod->hasResult()) {
		bodyBlock->appendLine(Poco::format("return cpgf::fromVariant<%s>(%s);", cppMethod->getResultType().getQualifiedName(), s));
	}
	else {
		bodyBlock->appendLine(s + ";");
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
