#include "application.h"

#include "model/cppcontext.h"
#include "model/cppnamespace.h"
#include "buildermodel/buildercontext.h"
#include "buildermodel/builderutil.h"
#include "codewriter/cppwriter.h"
#include "codewriter/codewriter.h"
#include "codewriter/codeblock.h"
#include "exception.h"
#include "logger.h"
#include "util.h"
#include "constants.h"

#include "Poco/Glob.h"
#include "Poco/File.h"
#include "Poco/Format.h"

#include <set>

#include <iostream>

using namespace std;
using namespace cpgf;

namespace metagen {


Application::Application(int argc, char * argv[])
{
}

Application::~Application()
{
}

void Application::run()
{
	try {
		this->doRun();
	}
	catch(const Exception & e) {
		logger.error(string("\n\n") + e.what());
	}
}

void Application::doRun()
{
	this->project.loadProject("zzz.js");

	this->processFiles();
	this->generateMainRegisterHeaderFile();
	this->generateMainRegisterSourceFile();
}

void Application::processFiles()
{
	for(StringArrayType::const_iterator it = this->project.getFiles().begin();
		it != this->project.getFiles().end();
		++it) {
		this->processOnePath(*it);
	}
}

void Application::processOnePath(const std::string & path)
{
	set<string> fileSet;
	string absolutePath = this->project.getAbsoluteFileName(path);

	Poco::Glob::glob(absolutePath, fileSet, Poco::Glob::GLOB_DOT_SPECIAL);
	for(set<string>::iterator it = fileSet.begin(); it != fileSet.end(); ++it) {
		if(Poco::File(*it).isDirectory()) {
			continue;
		}
		else {
			this->processOneFile(*it);
		}
	}
}

void Application::processOneFile(const std::string & file)
{
	if(this->project.doesForce()
		|| shouldTargetFileBeUpdated(file, this->project.getOutputHeaderFileName(file))) {
		logger.info(Poco::format("Generate for file %s...", file));

		CppContext context(&this->project);
		string absoluteFileName = this->project.getAbsoluteFileName(file);
		context.process(absoluteFileName);

		BuilderContext builderContext(&this->project, absoluteFileName,
			makeCallback(this, &Application::onGenerateCreationFunction));
		builderContext.process(&context);

		logger.info("done.\n");
	}
	else {
		logger.info(Poco::format("File %s is up to date, skipped.\n", file));
	}
}

void Application::onGenerateCreationFunction(const BuilderContext * builderContext, BuilderSection * section)
{
	// nested class, don't register to global
	if(section->getCppItem()->getParent() != NULL
		&& section->getCppItem()->getParent()->isClass()) {
		return;
	}

	string name = getCreationFunctionName(builderContext, section);
	if(this->creationFunctionNameList.find(name) != this->creationFunctionNameList.end()) {
		logger.warn(Poco::format("Creation function %s is duplicated.", name));
	}
	this->creationFunctionNameList.insert(name);
}

void Application::generateMainRegisterHeaderFile() const
{
	const string headerFileName(
		normalizeFile(
			this->project.getHeaderOutputPath()
			+ this->project.getMainRegisterFileName()
			+ this->project.getHeaderFileExtension()
		)
	);

	CppWriter cppWriter;

	cppWriter.setHeaderGuard(headerFileName);
	cppWriter.setNamespace(this->project.getCppNamespace());
	cppWriter.include(includeMetaDefine);

	CodeBlock * fileBlock = cppWriter.getCodeBlock();

	for(CreationFunctionNameListType::const_iterator it = this->creationFunctionNameList.begin();
		it != this->creationFunctionNameList.end();
		++it) {
		fileBlock->appendLine(getCreationFunctionPrototype(*it) + ";");
	}

	fileBlock->appendLine("template <typename Meta>");
	fileBlock->appendLine(
		Poco::format("void %s(Meta _d)", this->project.getMainRegisterFunctionName())
	);

	fileBlock->appendBlankLine();

	CodeBlock * bodyBlock = fileBlock->appendBlock(cbsBracketAndIndent);

	for(CreationFunctionNameListType::const_iterator it = this->creationFunctionNameList.begin();
		it != this->creationFunctionNameList.end();
		++it) {
		bodyBlock->appendLine(
			Poco::format("_d._class(%s());", *it)
		);
	}

	CodeWriter codeWriter;
	cppWriter.write(&codeWriter);

	writeStringToFile(headerFileName, codeWriter.getText());
}

void Application::generateMainRegisterSourceFile() const
{
	if(! this->project.shouldAutoRegisterToGlobal()) {
		return;
	}

	const string headerIncludeFileName(
		normalizeFile(
			this->project.getMainRegisterFileName()
			+ this->project.getHeaderFileExtension()
		)
	);
	const string sourceFileName(
		normalizeFile(
			this->project.getHeaderOutputPath()
			+ this->project.getMainRegisterFileName()
			+ this->project.getSourceFileExtension()
		)
	);

	CppWriter cppWriter;

	cppWriter.setNamespace(this->project.getCppNamespace());
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
			this->project.getMetaNamespace())
	);
	bodyBlock->appendLine(
		Poco::format("%s(_d);", this->project.getMainRegisterFunctionName())
	);

	CodeWriter codeWriter;
	cppWriter.write(&codeWriter);

	writeStringToFile(sourceFileName, codeWriter.getText());
}


} // namespace metagen
