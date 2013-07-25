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

#include "util.h"

#include "cpgf/gassert.h"

#include "Poco/RegularExpression.h"

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
	explicit ClangParserImplement(CppContext * context);
	~ClangParserImplement();

	void parse(const char * fileName);

private:
	void setupClang();
	void compileAST(const char * fileName);
	void translate();

	CppContainer * getCurrentCppContainer();

	template <typename T>
	T * addItem(clang::Decl * decl)
	{
		T * item = this->context->createItem<T>(decl);
		this->getCurrentCppContainer()->addItem(item);

		SourceManager & sm = this->compilerInstance.getSourceManager();
		item->setInMainFile(this->currentFileID == sm.getFileID(decl->getSourceRange().getBegin()));

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

//	void parseTemplateParams(TemplateDecl * templateDecl, CppTemplateItem * templateItem);

	string locationToSource(const SourceLocation & begin, const SourceLocation & end);
	string getTemplateArgumentName(const TemplateArgument & argument);
	string getQualTypeName(const QualType & qualType);
	string getTemplateSpecializationName(const TemplateSpecializationType * type);

private:
	CppContext * context;
	CppContainerStackType cppContainerStack;

	raw_fd_ostream outputStream;
	DiagnosticOptions diagnosticOptions;
	OwningPtr<CompilerInvocation> compilerInvocation;
	CompilerInstance compilerInstance;
	OwningPtr<TargetInfo> targetInfo;
	FileID currentFileID;
};


string getDeclName(NamedDecl * namedDecl)
{
	return namedDecl->getNameAsString();
}

string getDeclQualifiedName(NamedDecl * namedDecl)
{
	return namedDecl->getQualifiedNameAsString();
}

string doRemoveRecordWords(const string & text)
{
	static Poco::RegularExpression re("(struct|class|union)\\b\\s*");
	string result(text);
	re.subst(result, "");
	return result;
}

ClangParserImplement::ClangParserImplement(CppContext * context)
	: context(context), outputStream(1, false)
{
	this->setupClang();
}

void ClangParserImplement::setupClang()
{
	this->compilerInvocation.reset(new CompilerInvocation);

	// we add a customized macro here to distinguish a clreflect parsing process from a compling using clang
	//PreprocessorOptions & preprocessorOptions = this->compilerInvocation->getPreprocessorOpts();
//	preprocessorOptions.addMacroDef("__clcpp_parse__");

	// Add define/undefine macros to the pre-processor
	//for (int i = 0; ; i++)
	//{
	//	std::string macro = args.GetProperty("-D", i);
	//	if (macro == "")
	//		break;
	//	preprocessorOptions.addMacroDef(macro.c_str());
	//}
	//for (int i = 0; ; i++)
	//{
	//	std::string macro = args.GetProperty("-U", i);
	//	if (macro == "")
	//		break;
	//	preprocessorOptions.addMacroUndef(macro.c_str());
	//}

	// Setup the language parsing options for C++
	LangOptions & langOptions = *this->compilerInvocation->getLangOpts();
	this->compilerInvocation->setLangDefaults(langOptions, IK_CXX, LangStandard::lang_cxx03);
	langOptions.CPlusPlus = 1;
	langOptions.Bool = 1;
	langOptions.RTTI = 0;

	// VC compatible
	langOptions.MicrosoftExt = 1;
	langOptions.MicrosoftMode = 1;
	langOptions.MSBitfields = 1;
	langOptions.DelayedTemplateParsing = 1;

	// Gather C++ header searches from the command-line
	//HeaderSearchOptions & headerSearchOptions = this->compilerInvocation->getHeaderSearchOpts();
	//for (int i = 0; ; i++)
	//{
	//	std::string include = args.GetProperty("-i", i);
	//	if (include == "")
	//		break;
	//	headerSearchOptions.AddPath(include.c_str(), frontend::Angled, false, false);
	//}
	//for (int i = 0; ; i++)
	//{
	//	std::string include = args.GetProperty("-isystem", i);
	//	if (include == "")
	//		break;
	//	headerSearchOptions.AddPath(include.c_str(), frontend::System, false, false);
	//}

	TextDiagnosticPrinter * client = new TextDiagnosticPrinter(this->outputStream, &this->diagnosticOptions);
	char * argv = "";
	this->compilerInstance.createDiagnostics(0, false);
	this->compilerInstance.getDiagnostics().setSuppressSystemWarnings(true);

	// Setup target options - ensure record layout calculations use the MSVC C++ ABI
	TargetOptions & target_options = this->compilerInvocation->getTargetOpts();
	target_options.Triple = getDefaultTargetTriple();
	target_options.CXXABI = "microsoft";
	this->targetInfo.reset(TargetInfo::CreateTargetInfo(this->compilerInstance.getDiagnostics(), &target_options));
	this->compilerInstance.setTarget(this->targetInfo.take());

	// Set the invokation on the instance
	this->compilerInstance.createFileManager();
	this->compilerInstance.createSourceManager(this->compilerInstance.getFileManager());
	this->compilerInstance.setInvocation(this->compilerInvocation.take());
}

ClangParserImplement::~ClangParserImplement()
{
}

void ClangParserImplement::parse(const char * fileName)
{
	this->compileAST(fileName);

	this->context->beginFile(fileName, this->compilerInstance.getASTContext().getTranslationUnitDecl());
	this->cppContainerStack.push(this->context->getCppFile());
	
	this->translate();

	this->cppContainerStack.pop();
	this->context->endFile(fileName);
}

void ClangParserImplement::compileAST(const char * fileName)
{
	// Recreate preprocessor and AST context
	this->compilerInstance.createPreprocessor();
	this->compilerInstance.createASTContext();

	// Initialize builtins
	if(this->compilerInstance.hasPreprocessor()) {
		Preprocessor & preprocessor = this->compilerInstance.getPreprocessor();
		preprocessor.getBuiltinInfo().InitializeBuiltins(preprocessor.getIdentifierTable(),
			preprocessor.getLangOpts());
	}

	// Get the file  from the file system
	const FileEntry * file = this->compilerInstance.getFileManager().getFile(fileName);
	this->currentFileID = this->compilerInstance.getSourceManager().createMainFileID(file);

	// Parse the AST
	EmptyASTConsumer ast_consumer;
	DiagnosticConsumer * client = this->compilerInstance.getDiagnostics().getClient();
	client->BeginSourceFile(this->compilerInstance.getLangOpts(), &this->compilerInstance.getPreprocessor());
	ParseAST(this->compilerInstance.getPreprocessor(), &ast_consumer, this->compilerInstance.getASTContext());
	client->EndSourceFile();
}

void ClangParserImplement::translate()
{
	TranslationUnitDecl * translateUnitDecl = this->compilerInstance.getASTContext().getTranslationUnitDecl();

	this->parseDeclContext(translateUnitDecl);

	if (this->compilerInstance.hasPreprocessor()) {
		Preprocessor & preprocessor = this->compilerInstance.getPreprocessor();
		for(Preprocessor::macro_iterator it = preprocessor.macro_begin(); it != preprocessor.macro_end(); ++it) {
//			cout << "Macro    " << string(it->first->getName()) << endl;
		}
	}
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
cout << ">>> " << decl->getDeclKindName() << "     " << (dyn_cast<NamedDecl>(decl) ? dyn_cast<NamedDecl>(decl)->getNameAsString() : "") << endl;

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

//	this->parseTemplateParams(classTemplateDecl, cls);
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
	CppNamedItem * cppNamedItem = const_cast<CppNamedItem *>(this->context->findNamedItem(icNamespace, getNamedDeclQualifiedName(namespaceDecl)));
	if(cppNamedItem != NULL) {
		GASSERT(cppNamedItem->isNamespace());
		ns = static_cast<CppNamespace *>(cppNamedItem);
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
	CppNamedItem * cppNamedItem = const_cast<CppNamedItem *>(this->context->findNamedItem(icEnum, getNamedDeclQualifiedName(enumDecl)));
	if(cppNamedItem != NULL) {
		GASSERT(cppNamedItem->isEnum());
		e = static_cast<CppEnum *>(cppNamedItem);
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
	QualType qualType = baseSpecifier->getType();
	const Type * type = qualType.getTypePtr();
	string qualifiedName;
	if(type->getAsCXXRecordDecl() != NULL) {
		qualifiedName = type->getAsCXXRecordDecl()->getQualifiedNameAsString();
	}
	else if(dyn_cast<TemplateSpecializationType>(type) != NULL){
		const TemplateSpecializationType * t = dyn_cast<TemplateSpecializationType>(type);
		qualifiedName = this->getTemplateSpecializationName(t);
	}

	BaseClass * baseClass = new BaseClass(baseSpecifier, this->context);
	cls->getBaseClassList()->push_back(baseClass);
}

string ClangParserImplement::getTemplateArgumentName(const TemplateArgument & argument)
{
	string qualifiedName;

	switch(argument.getKind()) {
		case TemplateArgument::Null:
			qualifiedName = "NULL";
			break;

		case TemplateArgument::Type:
			qualifiedName = this->getQualTypeName(argument.getAsType());
			break;

		case TemplateArgument::Declaration:
			qualifiedName = dyn_cast<NamedDecl>(argument.getAsDecl())->getQualifiedNameAsString();
			break;

		case TemplateArgument::Integral:
		case TemplateArgument::Expression:
			qualifiedName = locationToSource(argument.getAsExpr()->getLocStart(), argument.getAsExpr()->getLocEnd());
			break;

		case TemplateArgument::Template:
			qualifiedName = argument.getAsTemplate().getAsTemplateDecl()->getQualifiedNameAsString();
			break;

		case TemplateArgument::TemplateExpansion:
			break;

		case TemplateArgument::Pack:
			break;

	}

	return qualifiedName;
}

string ClangParserImplement::getQualTypeName(const QualType & qualType)
{
	string qualifiedName;

//qualifiedName = type->getTypeClassName();

	if(qualType->getAsCXXRecordDecl() != NULL) {
		qualifiedName = qualType->getAsCXXRecordDecl()->getQualifiedNameAsString();
	}
	else if(qualType->getAs<TemplateSpecializationType>() != NULL){
		const TemplateSpecializationType * t = qualType->getAs<TemplateSpecializationType>();
		qualifiedName = this->getTemplateSpecializationName(t);
	}
	else if(qualType->getAs<TemplateTypeParmType>() != NULL){
//		const TemplateTypeParmType * t = qualType->getAs<TemplateTypeParmType>();
		qualifiedName = qualType.getAsString();
	}
	else {
		qualifiedName = qualType.getAsString();
	}

	qualifiedName = doRemoveRecordWords(qualifiedName);

	return qualifiedName;
}

string ClangParserImplement::getTemplateSpecializationName(const TemplateSpecializationType * type)
{
	string qualifiedName;

	qualifiedName = type->getTemplateName().getAsTemplateDecl()->getQualifiedNameAsString();
	qualifiedName += "<";
	for(unsigned int i = 0; i < type->getNumArgs(); ++i) {
		if(i > 0) {
			qualifiedName += ", ";
		}
		qualifiedName += this->getTemplateArgumentName(type->getArg(i));
	}
	qualifiedName += " >";

	return qualifiedName;
}

string ClangParserImplement::locationToSource(const SourceLocation & begin, const SourceLocation & end)
{
	SourceManager * sourceManager = &this->compilerInstance.getSourceManager();
	const LangOptions & langOptions = this->compilerInstance.getLangOpts();
    SourceLocation e(Lexer::getLocForEndOfToken(end, 0, *sourceManager, langOptions));
    return std::string(sourceManager->getCharacterData(begin), sourceManager->getCharacterData(e) - sourceManager->getCharacterData(begin));
}

//void ClangParserImplement::parseTemplateParams(TemplateDecl * templateDecl, CppTemplateItem * templateItem)
//{
//	TemplateParameterList * templateParamList = templateDecl->getTemplateParameters();
//	for(TemplateParameterList::iterator it = templateParamList->begin(); it != templateParamList->end(); ++it) {
//		NamedDecl * namedDecl = *it;
//		Decl::Kind kind = namedDecl->getKind();
//
//		CppType * type = NULL;
//		string defaultValue;
//		if(kind == Decl::TemplateTypeParm) {
//			TemplateTypeParmDecl * paramDecl = dyn_cast<TemplateTypeParmDecl>(namedDecl);
//			type = this->addType(paramDecl->wasDeclaredWithTypename() ? "typename" : "class");
//			if(paramDecl->hasDefaultArgument()) {
//				defaultValue = doRemoveRecordWords(paramDecl->getDefaultArgument().getAsString());
//			}
//		}
//		else if(kind == Decl::NonTypeTemplateParm) {
//			NonTypeTemplateParmDecl * paramDecl = dyn_cast<NonTypeTemplateParmDecl>(namedDecl);
//			type = this->addType(paramDecl->getType());
//			if(paramDecl->hasDefaultArgument()) {
//				defaultValue = this->locationToSource(paramDecl->getDefaultArgument()->getLocStart(), paramDecl->getDefaultArgument()->getLocEnd());
//			}
//		}
//		else if(kind == Decl::TemplateTemplateParm) {
//			TemplateTemplateParmDecl * paramDecl = dyn_cast<TemplateTemplateParmDecl>(namedDecl);
//			string t = locationToSource(paramDecl->getLocStart(), paramDecl->getLocEnd());
//			t = removeAllAfterEqualSign(t);
//			t = removeLastToken(t);
//			type = this->addType(t);
//			if(paramDecl->hasDefaultArgument()) {
//				defaultValue = this->getTemplateArgumentName(paramDecl->getDefaultArgument().getArgument());
//				if(defaultValue.empty()) {
//					defaultValue = this->locationToSource(paramDecl->getDefaultArgument().getSourceRange().getBegin(), paramDecl->getDefaultArgument().getSourceRange().getEnd());
//				}
//			}
//		}
//		if(type != NULL) {
//			CppParam * param = templateItem->addTemplateParam();
//			string name = namedDecl->getNameAsString();
//			param->setName(name);
//			param->setType(type);
//			if(! defaultValue.empty()) {
//				param->setDefaultValue(defaultValue);
//			}
//		}
//	}
//}


ClangParser::ClangParser(CppContext * context)
	:implement(new ClangParserImplement(context))
{
}

ClangParser::~ClangParser()
{
}

void ClangParser::parse(const char * fileName)
{
	this->implement->parse(fileName);
}


} // namespace metagen
