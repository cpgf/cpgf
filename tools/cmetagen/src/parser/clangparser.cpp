#include "clangparser.h"

#include "model/cppnamespace.h"
#include "model/cppclass.h"
#include "model/cppfield.h"
#include "model/cppmethod.h"
#include "model/cppconstructor.h"
#include "model/cppdestructor.h"
#include "model/cppenum.h"
#include "model/cppoperator.h"
#include "model/cppcontext.h"
#include "model/cppfile.h"
#include "model/cpputil.h"
#include "model/cppsourcefile.h"

#include "project.h"
#include "util.h"
#include "constants.h"
#include "exception.h"
#include "splitcommandline.h"

#include "cpgf/gassert.h"
#include "cpgf/gscopedptr.h"
#include "cpgf/gcallback.h"

#include "Poco/RegularExpression.h"
#include "Poco/StringTokenizer.h"
#include "Poco/String.h"

#include <stack>
#include <string>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "llvm/Support/Host.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Sema/Sema.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Basic/FileManager.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"	
#include "clang/Frontend/LangStandard.h"
#include "clang/AST/DeclTemplate.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include <iostream>

using namespace std;
using namespace cpgf;

using namespace llvm;
using namespace llvm::sys;
using namespace clang;

namespace metagen {

class EmptyASTConsumer : public ASTConsumer
{
public:
	virtual ~EmptyASTConsumer() {}
	virtual bool HandleTopLevelDecl(DeclGroupRef) { return true; }
};

typedef GCallback<void (CompilerInstance * compilerInstance)> ParserCallbackType;

class ParserLibClang
{
public:
	explicit ParserLibClang(const Project * project);

	void parse(const CppSourceFile & sourceFile, const ParserCallbackType & callback);

private:
	void setupClang(const CppSourceFile & sourceFile);
	void compileAST(const CppSourceFile & sourceFile);

private:
	raw_fd_ostream outputStream;
	DiagnosticOptions diagnosticOptions;
	GScopedPointer<CompilerInvocation> compilerInvocation;
	GScopedPointer<CompilerInstance> compilerInstance;
	const Project * project;
};

ParserLibClang::ParserLibClang(const Project * project)
	: outputStream(1, false), project(project)
{
}

void ParserLibClang::parse(const CppSourceFile & sourceFile, const ParserCallbackType & callback)
{
	this->setupClang(sourceFile);
	this->compileAST(sourceFile);
	callback(this->compilerInstance.get());
}

void ParserLibClang::setupClang(const CppSourceFile & sourceFile)
{
	this->compilerInstance.reset(new CompilerInstance);
	
	TextDiagnosticPrinter * client = new TextDiagnosticPrinter(this->outputStream, &this->diagnosticOptions);
//IgnoringDiagConsumer * client = new IgnoringDiagConsumer();
	this->compilerInstance->createDiagnostics(client, false);
	
	this->compilerInvocation.reset(new CompilerInvocation);

	vector<string> commands;
	if(! this->project->getClangOptions().empty()) {
		splitCommandLine(&commands, this->project->getClangOptions().c_str());
	}
	for(vector<string>::const_iterator it = sourceFile.getIncludeList().begin();
		it != sourceFile.getIncludeList().end();
		++it) {
		string includeLine = *it;
		if(includeLine.at(0) == '@') {
			const string includeDirector("#include");
			includeLine = includeLine.c_str() + 1;
			Poco::StringTokenizer tokenizer(includeLine, "\n", Poco::StringTokenizer::TOK_IGNORE_EMPTY | Poco::StringTokenizer::TOK_TRIM);
			for(Poco::StringTokenizer::Iterator it = tokenizer.begin(); it != tokenizer.end(); ++it) {
				includeLine = *it;
				size_t pos = includeLine.find(includeDirector);
				if(pos != string::npos) {
					commands.push_back("-include");
					includeLine = includeLine.substr(pos + includeDirector.length());
					includeLine.erase(std::remove(includeLine.begin(), includeLine.end(), '"'), includeLine.end());
					Poco::trimInPlace(includeLine);
					commands.push_back(includeLine);
				}
			}
		}
		else {
			commands.push_back("-include");
			commands.push_back(includeLine);
		}
	}
	if(! commands.empty()) {
		vector<const char *> commandPointers;
		for(vector<string>::iterator it = commands.begin(); it != commands.end(); ++it) {
			commandPointers.push_back(it->c_str());
		}
		CompilerInvocation::CreateFromArgs(*(this->compilerInvocation.get()),
			&commandPointers[0], &commandPointers[0] + commandPointers.size(),
			this->compilerInstance->getDiagnostics());
	}

	PreprocessorOptions & preprocessorOptions = this->compilerInvocation->getPreprocessorOpts();
	preprocessorOptions.addMacroDef(ParserPredefinedMacro);

	LangOptions & langOptions = *this->compilerInvocation->getLangOpts();
	langOptions.Bool = 1;
	// Use -std=XXX in option clangOptions
	//this->compilerInvocation->setLangDefaults(langOptions, IK_CXX, LangStandard::lang_cxx11);
	// Use -x c++ in option clangOptions
	//langOptions.CPlusPlus = 1;
	// Use -fcxx-exceptions in clangOptions
	//langOptions.CXXExceptions = 1;

	// use -fms-compatibility and -fms-extensions in clangOptions
	//langOptions.MicrosoftExt = 1;
	//langOptions.MicrosoftMode = 1;
	// use -fdelayed-template-parsing in clangOptions
	// langOptions.DelayedTemplateParsing = 1;

//	HeaderSearchOptions & headerSearchOptions = this->compilerInvocation->getHeaderSearchOpts();

	DiagnosticsEngine & diagnostics = this->compilerInstance->getDiagnostics();
	diagnostics.setSuppressSystemWarnings(true);

	TargetOptions & target_options = this->compilerInvocation->getTargetOpts();
	GScopedPointer<TargetInfo> targetInfo(TargetInfo::CreateTargetInfo(diagnostics, &target_options));
	this->compilerInstance->setTarget(targetInfo.take());

	this->compilerInstance->createFileManager();
	this->compilerInstance->createSourceManager(this->compilerInstance->getFileManager());
	this->compilerInstance->setInvocation(this->compilerInvocation.take());
}

void ParserLibClang::compileAST(const CppSourceFile & sourceFile)
{
	this->compilerInstance->createPreprocessor();
	this->compilerInstance->createASTContext();

	if(this->compilerInstance->hasPreprocessor()) {
		Preprocessor & preprocessor = this->compilerInstance->getPreprocessor();
		preprocessor.getBuiltinInfo().InitializeBuiltins(preprocessor.getIdentifierTable(),
			preprocessor.getLangOpts());
	}

	const FileEntry * file = this->compilerInstance->getFileManager().getFile(sourceFile.getFileName());
	this->compilerInstance->getSourceManager().createMainFileID(file);

	EmptyASTConsumer astConsumer;
	DiagnosticConsumer * client = this->compilerInstance->getDiagnostics().getClient();
	client->BeginSourceFile(this->compilerInstance->getLangOpts(), &this->compilerInstance->getPreprocessor());
	ParseAST(this->compilerInstance->getPreprocessor(), &astConsumer, this->compilerInstance->getASTContext());
	client->EndSourceFile();

	if(client->getNumErrors() > 0 && this->project->shouldStopOnCompileError()) {
		fatalError("Compile errors occured");
	}
}

typedef stack<CppContainer *> CppContainerStackType;

class CppContainerGuard
{
public:
	explicit CppContainerGuard(CppContainerStackType * cppContainerStack, CppContainer * cppContext)
		: cppContainerStack(cppContainerStack)
	{
		this->cppContainerStack->push(cppContext);
	}

	~CppContainerGuard() {
		this->cppContainerStack->pop();
	}

private:
	CppContainerStackType * cppContainerStack;
};

class ClangParserImplement
{
public:
	ClangParserImplement(CppContext * context, const Project * project);
	~ClangParserImplement();

	void parse(const CppSourceFile & sourceFile);

private:
	void translate(CompilerInstance * compilerInstance);

	CppContainer * getCurrentCppContainer();

	template <typename T>
	T * addItem(clang::Decl * decl)
	{
		T * item = this->context->createItem<T>(decl);
		this->getCurrentCppContainer()->addItem(item);

		SourceManager & sm = this->compilerInstance->getSourceManager();
		item->setInMainFile(this->compilerInstance->getSourceManager().getMainFileID() == sm.getFileID(decl->getSourceRange().getBegin()));

		return item;
	}

private:
	void parseDeclContext(DeclContext * declContext);

	// top level parser
	void parseDecl(Decl * decl);
	void parseVar(VarDecl * varDecl);
	void parseClass(CXXRecordDecl * classDecl);
	void parseTemplateClass(ClassTemplateDecl * classTemplateDecl);
	void parseTemplateFunction(FunctionTemplateDecl * functionTemplateDecl);
	void parseFunction(FunctionDecl * functionDecl); // either member and non-member function
	void parseNamespace(NamespaceDecl * namespaceDecl);
	void parseEnum(EnumDecl * enumDecl);
	void parseConstructor(CXXConstructorDecl * constructorDecl);
	void parseDestructor(CXXDestructorDecl * destructorDecl);
	void parseOperator(FunctionDecl * functionDecl);
	void parseTemplateOperator(FunctionTemplateDecl * functionTemplateDecl);

	// non-top level parser
	void parseField(FieldDecl * fieldDecl);
	void parseBaseClass(CppClass * cls, CXXBaseSpecifier * baseSpecifier);

private:
	CppContext * context;
	CppContainerStackType cppContainerStack;

	GScopedPointer<ParserLibClang> parser;

	CompilerInstance * compilerInstance;

	const Project * project;
};


ClangParserImplement::ClangParserImplement(CppContext * context, const Project * project)
	: context(context), compilerInstance(NULL), project(project)
{
}

ClangParserImplement::~ClangParserImplement()
{
}

void ClangParserImplement::parse(const CppSourceFile & sourceFile)
{
	this->parser.reset(new ParserLibClang(this->project));
	this->parser->parse(sourceFile, makeCallback(this, &ClangParserImplement::translate));
}

void ClangParserImplement::translate(CompilerInstance * compilerInstance)
{
	this->compilerInstance = compilerInstance;

	this->context->beginFile(compilerInstance->getASTContext().getTranslationUnitDecl());
	this->cppContainerStack.push(this->context->getCppFile());

	TranslationUnitDecl * translateUnitDecl = compilerInstance->getASTContext().getTranslationUnitDecl();

	this->parseDeclContext(translateUnitDecl);

	if (this->compilerInstance->hasPreprocessor()) {
		Preprocessor & preprocessor = compilerInstance->getPreprocessor();
		for(Preprocessor::macro_iterator it = preprocessor.macro_begin(); it != preprocessor.macro_end(); ++it) {
//			cout << "Macro    " << string(it->first->getName()) << endl;
		}
	}

	this->cppContainerStack.pop();
	this->context->endFile();
}

CppContainer * ClangParserImplement::getCurrentCppContainer()
{
	return this->cppContainerStack.top();
}

void ClangParserImplement::parseDeclContext(DeclContext * declContext)
{
	for(DeclContext::decl_iterator it = declContext->decls_begin(); it != declContext->decls_end(); ++it) {
		this->parseDecl(*it);
	}
}

void ClangParserImplement::parseDecl(Decl * decl)
{
	if(decl->isInvalidDecl()) {
		return;
	}

	Decl::Kind kind = decl->getKind();
//cout << ">>> " << decl->getDeclKindName() << "     " << (dyn_cast<NamedDecl>(decl) ? dyn_cast<NamedDecl>(decl)->getNameAsString() : "") << endl;

	switch (kind) {
		case Decl::LinkageSpec:
			this->parseDeclContext(dyn_cast<DeclContext>(decl));
			break;

		case Decl::Namespace:
			this->parseNamespace(dyn_cast<NamespaceDecl>(decl));
			break;

		case Decl::CXXRecord:
			this->parseClass(dyn_cast<CXXRecordDecl>(decl));
			break;

		case Decl::ClassTemplate:
			this->parseTemplateClass(dyn_cast<ClassTemplateDecl>(decl));
			break;

		case Decl::Function:
			this->parseFunction(dyn_cast<FunctionDecl>(decl));
			break;

		case Decl::FunctionTemplate:
			this->parseTemplateFunction(dyn_cast<FunctionTemplateDecl>(decl));
			break;

		case Decl::Enum:
			this->parseEnum(dyn_cast<EnumDecl>(decl));
			break;

		case Decl::Var:
			this->parseVar(dyn_cast<VarDecl>(decl));
			break;

		case Decl::CXXMethod:
			this->parseFunction(dyn_cast<CXXMethodDecl>(decl));
			break;

		case Decl::Field:
			this->parseField(dyn_cast<FieldDecl>(decl));
			break;

		case Decl::CXXConstructor:
			this->parseConstructor(dyn_cast<CXXConstructorDecl>(decl));
			break;

		case Decl::CXXDestructor:
			this->parseDestructor(dyn_cast<CXXDestructorDecl>(decl));
			break;

		case Decl::CXXConversion:
			this->parseOperator(dyn_cast<CXXConversionDecl>(decl));
			break;
	}
}

void ClangParserImplement::parseVar(VarDecl * varDecl)
{
	if(varDecl == NULL) {
		return;
	}

	this->addItem<CppField>(varDecl);
}

void ClangParserImplement::parseClass(CXXRecordDecl * classDecl)
{
	if(classDecl == NULL) {
		return;
	}

	if(! classDecl->isCompleteDefinition() || ! classDecl->isThisDeclarationADefinition()) {
		return;
	}

	CppClass * cls = this->addItem<CppClass>(classDecl);
	
	CppContainerGuard nsGuard(&this->cppContainerStack, cls);

	this->parseDeclContext(classDecl);

	for(CXXRecordDecl::base_class_iterator it = classDecl->bases_begin(); it != classDecl->bases_end(); ++it) {
		this->parseBaseClass(cls, &*it);
	}
}

void ClangParserImplement::parseTemplateClass(ClassTemplateDecl * classTemplateDecl)
{
	if(classTemplateDecl == NULL || ! classTemplateDecl->isThisDeclarationADefinition()) {
		return;
	}

	CppClass * cls = this->addItem<CppClass>(classTemplateDecl);

	CppContainerGuard nsGuard(&this->cppContainerStack, cls);

	CXXRecordDecl * classDecl = classTemplateDecl->getTemplatedDecl();
	this->parseDeclContext(classDecl);

	for(CXXRecordDecl::base_class_iterator it = classDecl->bases_begin(); it != classDecl->bases_end(); ++it) {
		this->parseBaseClass(cls, &*it);
	}
}

bool isOperator(const string & name)
{
	static const Poco::RegularExpression operatorRegex("\\boperator\\b");
	Poco::RegularExpression::Match match;
	return operatorRegex.match(name, match) > 0;
}

void ClangParserImplement::parseTemplateFunction(FunctionTemplateDecl * functionTemplateDecl)
{
	if(functionTemplateDecl == NULL || ! functionTemplateDecl->isFirstDeclaration()) {
		return;
	}

	string name = functionTemplateDecl->getNameAsString();
	if(isOperator(name)) {
		this->parseTemplateOperator(functionTemplateDecl);
	}
	else {
		this->addItem<CppMethod>(functionTemplateDecl);
	}
}

void ClangParserImplement::parseFunction(FunctionDecl * functionDecl)
{
	if(functionDecl == NULL || ! functionDecl->isFirstDeclaration() || functionDecl->isMain()) {
		return;
	}

	string name = functionDecl->getNameAsString();
	if(isOperator(name)) {
		this->parseOperator(functionDecl);
	}
	else {
		this->addItem<CppMethod>(functionDecl);
	}
}

void ClangParserImplement::parseNamespace(NamespaceDecl * namespaceDecl)
{
	if(namespaceDecl == NULL) {
		return;
	}

	CppNamespace * ns;
	CppItem * cppItem = this->context->findItemByDecl(namespaceDecl);
	if(cppItem != NULL) {
		GASSERT(cppItem->isNamespace());
		ns = static_cast<CppNamespace *>(cppItem);
	}
	else {
		ns = this->addItem<CppNamespace>(namespaceDecl);
	}
	
	CppContainerGuard nsGuard(&this->cppContainerStack, ns);

	this->parseDeclContext(namespaceDecl);
}

void ClangParserImplement::parseEnum(EnumDecl * enumDecl)
{
	if(enumDecl == NULL || ! enumDecl->isFirstDeclaration()) {
		return;
	}

	CppEnum * e;
	CppItem * cppItem = this->context->findItemByDecl(enumDecl);
	if(cppItem != NULL) {
		GASSERT(cppItem->isEnum());
		e = static_cast<CppEnum *>(cppItem);
	}
	else {
		e = this->addItem<CppEnum>(enumDecl);
	}
	for(EnumDecl::enumerator_iterator it = enumDecl->enumerator_begin(); it != enumDecl->enumerator_end(); ++it) {
		e->addValue(it->getNameAsString(), it->getQualifiedNameAsString());
	}
}

void ClangParserImplement::parseConstructor(CXXConstructorDecl * constructorDecl)
{
	if(constructorDecl == NULL || ! constructorDecl->isFirstDeclaration()) {
		return;
	}

	this->addItem<CppConstructor>(constructorDecl);
}

void ClangParserImplement::parseDestructor(CXXDestructorDecl * destructorDecl)
{
	if(destructorDecl == NULL || ! destructorDecl->isFirstDeclaration()) {
		return;
	}

	this->addItem<CppDestructor>(destructorDecl);
}

void ClangParserImplement::parseOperator(FunctionDecl * functionDecl)
{
	if(functionDecl == NULL || ! functionDecl->isFirstDeclaration()) {
		return;
	}

	this->addItem<CppOperator>(functionDecl);
}

void ClangParserImplement::parseTemplateOperator(FunctionTemplateDecl * functionTemplateDecl)
{
	if(functionTemplateDecl == NULL || ! functionTemplateDecl->isFirstDeclaration()) {
		return;
	}

	FunctionDecl * functionDecl = functionTemplateDecl->getTemplatedDecl();
	this->parseOperator(functionDecl);
}

void ClangParserImplement::parseField(FieldDecl * fieldDecl)
{
	if(fieldDecl == NULL) {
		return;
	}

	this->addItem<CppField>(fieldDecl);
}

void ClangParserImplement::parseBaseClass(CppClass * cls, CXXBaseSpecifier * baseSpecifier)
{
	BaseClass * baseClass = new BaseClass(baseSpecifier, this->context, cls);
	cls->getBaseClassList()->push_back(baseClass);
}


ClangParser::ClangParser(const Project * project)
	: project(project)
{
}

ClangParser::~ClangParser()
{
}

void ClangParser::parse(CppContext * context, const CppSourceFile & sourceFile)
{
	this->implement.reset(new ClangParserImplement(context, project));
	this->implement->parse(sourceFile);
}


} // namespace metagen
