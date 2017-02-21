#include "builderutil.h"
#include "buildercontext.h"
#include "buildersection.h"
#include "buildertemplateinstantiation.h"
#include "model/cppitem.h"
#include "model/cppcontainer.h"
#include "model/cppclass.h"
#include "model/cppinvokable.h"
#include "model/cppenum.h"
#include "codewriter/cppwriter.h"
#include "codewriter/codewriter.h"
#include "codewriter/codeblock.h"
#include "constants.h"
#include "util.h"
#include "project.h"

#include "cpgf/gassert.h"

#include "Poco/Format.h"

using namespace std;


namespace metagen {

string getTextOfVisibility(ItemVisibility visibility)
{
	switch(visibility) {
		case ivPrivate:
			return "private";

		case ivProtected:
			return "protected";

		default:
			return "public";
	}
}

std::string getReflectionClassName(const Project * project, bool asType)
{
	std::string s = project->getMetaDefineParamName() + "::ClassType";
	if(asType) {
		return "typename " + s;
	}
	else {
		return s;
	}
}

std::string getReflectionScope(const CppItem * item, bool asType)
{
	if(item->isGlobal()) {
		string scope = item->getParent()->getQualifiedName();
		if(! scope.empty()) {
			scope.append("::");
		}
		return scope;
	}
	else {
		return getReflectionClassName(item->getProject(), asType) + "::";
	}
}

size_t getCppItemPayload(const CppItem * item)
{
	if(item->isFile() || item->isNamespace()) {
		return 0;
	}
	size_t payload = 1;
	if(item->isInvokable()) {
		const CppInvokable * invokable = static_cast<const CppInvokable *>(item);
		for(size_t i = 0; i < invokable->getArity(); ++i) {
			if(invokable->paramHasDefaultValue(i)) {
				++payload;
			}
		}
	}
	else if(item->isEnum()) {
		payload += static_cast<const CppEnum *>(item)->getValueList()->size();
	}
	return payload;
}

std::string getClassWrapperClassName(const BuilderContext * builderContext, const CppContainer * cppContainer)
{
	return cppContainer->getName() + builderContext->getProject()->getClassWrapperPostfix();
}

string getCppClassNormalizedSymboName(const CppClass * cppClass)
{
	return cppClass->getQualifiedName();
}

string getContainerNormalizedSymboName(const BuilderContext * builderContext, BuilderSection * section)
{
	GASSERT(section->getCppItem()->isContainer());

	const CppContainer * cppContainer = static_cast<const CppContainer *>(section->getCppItem());
	string result;

	switch(getNameTypeFromBuilderSection(section)) {
		case cntNormal:
			if(cppContainer->isClass() || cppContainer->isNamespace()) {
				result = cppContainer->getQualifiedName();
				if(cppContainer->isNamespace()) {
					// Add _namespace to avoid conflicting with class. A conflicition may occur when
					// in file A.h there is namespace Foo and a class Foo::A
					result = result + "_namespace_" + builderContext->getSourceBaseFileName();
				}
			}
			else {
				result = builderContext->getSourceBaseFileName() + "_Global";
				result = result + "_" + builderContext->getSourceBaseFileName();
			}
			break;

		case cntClassWrapper:
			result = cppContainer->getQualifiedName() + builderContext->getProject()->getClassWrapperPostfix();
			break;
	}
	return normalizeSymbolName(result);
}

string getSectionIndexName(size_t sectionIndex)
{
	string result;

	if(sectionIndex > 0) {
		Poco::format(result, "_%d", sectionIndex);
	}

	return result;
}

std::string getPartialCreationFunctionName(const BuilderContext * builderContext, BuilderSection * section)
{
	GASSERT(section->getCppItem()->isContainer());

	size_t sectionIndex = section->getIndex();

	return normalizeSymbolName(Poco::format("partial_%s_%s%s",
		builderContext->getProject()->getCreationFunctionPrefix(),
		getContainerNormalizedSymboName(builderContext, section),
		getSectionIndexName(sectionIndex)
		)
	);
}

std::string getPartialCreationFunctionPrototype(const BuilderContext * builderContext, BuilderSection * section)
{
	string creationName = getPartialCreationFunctionName(builderContext, section);
	return Poco::format("void %s(cpgf::GDefineMetaInfo metaInfo)", creationName);
}

std::string getCreationFunctionName(const BuilderContext * builderContext, BuilderSection * section)
{
	return normalizeSymbolName(builderContext->getProject()->getCreationFunctionPrefix()
		+ getContainerNormalizedSymboName(builderContext, section));
}

std::string getCreationFunctionPrototype(const BuilderContext * builderContext, BuilderSection * section)
{
	string functionName = getCreationFunctionName(builderContext, section);
	return getCreationFunctionPrototype(functionName);
}

std::string getCreationFunctionPrototype(const std::string & functionName)
{
	return Poco::format("cpgf::GDefineMetaInfo %s()", functionName);
}

std::string getReflectionFunctionName(const BuilderContext * builderContext, BuilderSection * section)
{
	GASSERT(section->getCppItem()->isContainer());

	size_t sectionIndex = section->getIndex();

	return normalizeSymbolName(Poco::format("%s_%s%s",
		builderContext->getProject()->getReflectionFunctionPrefix(),
		getContainerNormalizedSymboName(builderContext, section),
		getSectionIndexName(sectionIndex)
		)
	);
}

CodeNameType getNameTypeFromBuilderSection(BuilderSection * section)
{
	if(section->isClassWrapper()) {
		return cntClassWrapper;
	}
	else {
		return cntNormal;
	}
}

void initializeReflectionFunctionOutline(const BuilderContext * builderContext, BuilderSection * section)
{
	GASSERT(section->getCppItem()->isContainer());

	const CppContainer * cppContainer = static_cast<const CppContainer *>(section->getCppItem());
	CodeBlock * codeBlock = section->getCodeBlock();
	string functionName = getReflectionFunctionName(builderContext, section);

	const std::string & D = builderContext->getProject()->getMetaDefineParamName();
	CodeBlock * headerBlock = codeBlock->getNamedBlock(CodeBlockName_FunctionHeader);
	const CppClass * cppClass = cppContainer->isClass() ? static_cast<const CppClass *>(cppContainer) : NULL;

	string s = "template <typename " + D;
	if(cppClass != NULL && cppClass->isChainedTemplate()) {
		s.append(", ");
		s.append(cppClass->getTextOfChainedTemplateParamList(itoWithArgType | itoWithArgName | itoWithDefaultValue));
	}
	s.append(" >");
	headerBlock->appendLine(s);

	s = Poco::format("void %s(%s & _d)", functionName, D);
	headerBlock->appendLine(s);
	
	CodeBlock * bodyBlock = codeBlock->getNamedBlock(CodeBlockName_FunctionBody);
	bodyBlock->appendLine("using namespace cpgf;");
	bodyBlock->appendBlankLine();
	bodyBlock->appendLine("(void)_d;");
	bodyBlock->appendBlankLine();
	
	// force the block order for each kind of items
	for(ItemCategory ic = icFirst; ic < icCount; ic = ItemCategory(int(ic) + 1)) {
		bodyBlock->getNamedBlock(ItemNames[ic]);
	}
}

string getMetaTypeTypedef(const BuilderContext * builderContext,
	BuilderSection * section,
	string * outReflectionTemplateParams)
{
	const CppContainer * cppContainer = static_cast<const CppContainer *>(section->getCppItem());
	const CppClass * cppClass = NULL;
	if(cppContainer->isClass()) {
		cppClass = static_cast<const CppClass *>(cppContainer);
	}

	const BuilderTemplateInstantiation * templateInstantiation = section->getTemplateInstantiation();

	if(cppClass != NULL && cppClass->isTemplate()) {
		if(templateInstantiation == NULL) {
			return "";
		}
	}

	string metaType;
	if(cppClass != NULL) {
		string className = getContainerNormalizedSymboName(builderContext, section);
		if(section->isClassWrapper()) {
			// Don't get qualified name since the wrapper is not in the same namespace as the class it wrapped for.
			className = getClassWrapperClassName(builderContext, cppClass);
		}
		else {
			className = cppClass->getQualifiedName();
		}
		string selfType;
		if(cppClass->isTemplate()) {
			selfType = Poco::format("%s<%s >", className, templateInstantiation->getTemplateParams());
			if(outReflectionTemplateParams != NULL) {
				*outReflectionTemplateParams = Poco::format("<%s, %s >",
					metaTypeTypeDefName, templateInstantiation->getTemplateParams());
			}
		}
		else {
			selfType = className;
		}
		metaType = Poco::format("cpgf::GDefineMetaClass<%s >", selfType);
	}
	else {
		metaType = "GDefineMetaNamespace";
	}

	return Poco::format("typedef %s %s;", metaType, metaTypeTypeDefName);
}

void initializePartialCreationFunction(const BuilderContext * builderContext, BuilderSection * section)
{
	GASSERT(section->getCppItem()->isContainer());

	const CppContainer * cppContainer = static_cast<const CppContainer *>(section->getCppItem());
	CodeBlock * codeBlock = section->getCodeBlock();

	const CppClass * cppClass = NULL;
	if(cppContainer->isClass()) {
		cppClass = static_cast<const CppClass *>(cppContainer);
	}

	string prototype = getPartialCreationFunctionPrototype(builderContext, section);

	string s;

	const BuilderTemplateInstantiation * templateInstantiation = section->getTemplateInstantiation();

	if(cppClass != NULL && cppClass->isTemplate()) {
		if(templateInstantiation == NULL) {
			return;
		}

		codeBlock->appendLine(s);
	}
	codeBlock->appendLine(prototype);

	CodeBlock * bodyBlock = codeBlock->appendBlock(cbsBracketAndIndent);

	string reflectionTemplateParams;

	s = getMetaTypeTypedef(builderContext, section, &reflectionTemplateParams);
	bodyBlock->appendLine(s);

	s = Poco::format("%s meta = %s::fromMetaClass(metaInfo.getMetaClass());", metaTypeTypeDefName, metaTypeTypeDefName);
	bodyBlock->appendLine(s);
	s = Poco::format("%s%s(meta);",
		getReflectionFunctionName(builderContext, section), reflectionTemplateParams);
	bodyBlock->appendLine(s);
}

void initializeClassWrapperOutline(const BuilderContext * builderContext, BuilderSection * section)
{
	GASSERT(section->getCppItem()->isClass());

	const CppContainer * cppContainer = static_cast<const CppContainer *>(section->getCppItem());
	CodeBlock * codeBlock = section->getCodeBlock();

	const CppClass * cppClass = static_cast<const CppClass *>(cppContainer);
	string className = cppClass->getQualifiedName();
	string s;
	
	if(cppClass->isTemplate()) {
		s = Poco::format("template <%s >", cppClass->getTextOfChainedTemplateParamList(itoWithArgType | itoWithArgName));
		codeBlock->appendLine(s);
		className.append(Poco::format("<%s >", cppClass->getTextOfChainedTemplateParamList(itoWithArgName)));
	}

	s = Poco::format("class %s : public %s, public cpgf::GScriptWrapper",
		getClassWrapperClassName(builderContext, cppClass),
		className
	);
	codeBlock->appendLine(s);

	CodeBlock * bodyBlock = codeBlock->getNamedBlock(CodeBlockName_ClassBody, cbsBracketWithSemicolon);
	bodyBlock->appendLine("private:");
	CodeBlock * superBlock = bodyBlock->appendBlock(cbsIndent | cbsTailEmptyLine);
	s = Poco::format("typedef %s super;", getCppContainerInstantiationName(cppClass));
	superBlock->appendLine(s);
	for(int i = ivFirst; i < ivCount; ++i) {
		s = getTextOfVisibility(ItemVisibility(i));
		CodeBlock * block = bodyBlock->getNamedBlock(s, cbsTailEmptyLine);
		block->appendLine(s + ":");
		block->getNamedBlock(CodeBlockName_Customize, cbsIndent | cbsTailEmptyLine);
	}
}


void generateMainRegisterHeaderFile(const std::set<std::string> & creationFunctionNameList, const Project * project)
{
	const string headerFileName(
		normalizeFile(
			normalizePath(project->getHeaderOutputPath())
			+ project->getMainRegisterFileName()
			+ project->getHeaderFileExtension()
		)
	);

	CppWriter cppWriter;

	cppWriter.setHeaderGuard(headerFileName);
	cppWriter.setNamespace(project->getCppNamespace());
	cppWriter.include(includeMetaDefine);

	CodeBlock * fileBlock = cppWriter.getCodeBlock();

	for(std::set<std::string>::const_iterator it = creationFunctionNameList.begin();
		it != creationFunctionNameList.end();
		++it) {
		fileBlock->appendLine(getCreationFunctionPrototype(*it) + ";");
	}

	fileBlock->appendLine("template <typename Meta>");
	fileBlock->appendLine(
		Poco::format("void %s(Meta _d)", project->getMainRegisterFunctionName())
	);

	fileBlock->appendBlankLine();

	CodeBlock * bodyBlock = fileBlock->appendBlock(cbsBracketAndIndent);

	for(std::set<std::string>::const_iterator it = creationFunctionNameList.begin();
		it != creationFunctionNameList.end();
		++it) {
		bodyBlock->appendLine(
			Poco::format("_d._class(%s());", *it)
		);
	}

	CodeWriter codeWriter;
	cppWriter.write(&codeWriter);

	string fileContent;
	if(readStringFromFile(headerFileName, &fileContent)) {
		if(codeWriter.getText() == fileContent) {
			return;
		}
	}

	writeStringToFile(headerFileName, codeWriter.getText());
}

void generateMainRegisterSourceFile(const Project * project)
{
	if(! project->shouldAutoRegisterToGlobal()) {
		return;
	}

	const string headerIncludeFileName(
		normalizeFile(
			project->getMainRegisterFileName()
			+ project->getHeaderFileExtension()
		)
	);
	const string sourceFileName(
		normalizeFile(
			normalizePath(project->getSourceOutputPath())
			+ project->getMainRegisterFileName()
			+ project->getSourceFileExtension()
		)
	);

	CppWriter cppWriter;

	cppWriter.setNamespace(project->getCppNamespace());
	cppWriter.include(headerIncludeFileName);
	cppWriter.include(includeOutmain);

	CodeBlock * fileBlock = cppWriter.getCodeBlock();
	fileBlock->appendLine("namespace");

	CodeBlock * innerBlock = fileBlock->appendBlock(cbsBracket);
	innerBlock->appendBlankLine();
	innerBlock->appendLine("G_AUTO_RUN_BEFORE_MAIN()");
	
	CodeBlock * bodyBlock = innerBlock->appendBlock(cbsBracketAndIndent);
	bodyBlock->appendLine(
		Poco::format("cpgf::GDefineMetaNamespace _d = cpgf::GDefineMetaNamespace::define(\"%s\");",
			project->getMetaNamespace())
	);
	bodyBlock->appendLine(
		Poco::format("%s(_d);", project->getMainRegisterFunctionName())
	);

	CodeWriter codeWriter;
	cppWriter.write(&codeWriter);

	string fileContent;
	if(readStringFromFile(sourceFileName, &fileContent)) {
		if(codeWriter.getText() == fileContent) {
			return;
		}
	}

	writeStringToFile(sourceFileName, codeWriter.getText());
}

void generateMainRegisterFiles(const std::set<std::string> & creationFunctionNameList, const Project * project)
{
	generateMainRegisterHeaderFile(creationFunctionNameList, project);
	generateMainRegisterSourceFile(project);
}

bool shouldGenerateCreationFunction(const CppItem * cppItem)
{
	// nested class, don't register to global
	if(cppItem->getParent() != NULL
		&& cppItem->getParent()->isClass()) {
		return false;
	}
	
	return true;
}


} // namespace metagen

