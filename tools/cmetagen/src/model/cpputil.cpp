#include "cpputil.h"

#include "Poco/RegularExpression.h"

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "llvm/Support/raw_ostream.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/PrettyPrinter.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include <vector>

using namespace llvm;
using namespace clang;
using namespace std;


string removeRecordWords(const string & text)
{
	static Poco::RegularExpression re("(struct|class|union)\\b\\s*");
	string result(text);
	re.subst(result, "");
	return result;
}

void getNamedDeclNames(const NamedDecl * namedDecl,
	std::string & name,
	std::string & qualifiedName,
	std::string & qualifiedNameWithoutNamespace)
{
	name = namedDecl->getNameAsString();
	qualifiedName = name;
	qualifiedNameWithoutNamespace = name;

	const DeclContext * context = namedDecl->getDeclContext();
	if(context->isFunctionOrMethod()) {
		return;
	}

	vector<const DeclContext *> contextList;
	
	while(context != NULL && isa<NamedDecl>(context)) {
		contextList.push_back(context);
		context = context->getParent();
	}

	for(vector<const DeclContext *>::iterator it = contextList.begin(); it != contextList.end(); ++it) {
		const NamespaceDecl * namespaceDecl = dyn_cast<NamespaceDecl>(*it);
		if(namespaceDecl != NULL && ! namespaceDecl->isAnonymousNamespace()) {
			qualifiedName = namespaceDecl->getNameAsString() + "::" + qualifiedName;
		}
		else {
			const RecordDecl * recordDecl = dyn_cast<RecordDecl>(*it);
			if(recordDecl != NULL && ! recordDecl->isAnonymousStructOrUnion()) {
				string s(recordDecl->getNameAsString());
				qualifiedName = s + "::" + qualifiedName;
				qualifiedNameWithoutNamespace = s + "::" + qualifiedNameWithoutNamespace;
			}
		}
	}
}

std::string	 getNamedDeclOutputName(const NamedDecl * namedDecl)
{
	std::string name, qualifiedName, qualifiedNameWithoutNamespace;
	getNamedDeclNames(namedDecl, name, qualifiedName, qualifiedNameWithoutNamespace);
	return qualifiedNameWithoutNamespace;
}

std::string getTemplateSpecializationName(const TemplateSpecializationType * type)
{
	string qualifiedName;

	qualifiedName = type->getTemplateName().getAsTemplateDecl()->getQualifiedNameAsString();
	qualifiedName += "<";
	for(unsigned int i = 0; i < type->getNumArgs(); ++i) {
		if(i > 0) {
			qualifiedName += ", ";
		}
		qualifiedName += getTemplateArgumentName(type->getArg(i));
	}
	qualifiedName += " >";

	return qualifiedName;
}

string getTemplateArgumentName(const TemplateArgument & argument)
{
	string qualifiedName;

	switch(argument.getKind()) {
		case TemplateArgument::Null:
			qualifiedName = "NULL";
			break;

		case TemplateArgument::Type:
			qualifiedName = qualTypeToText(argument.getAsType(), "");
			break;

		case TemplateArgument::Declaration:
			qualifiedName = dyn_cast<NamedDecl>(argument.getAsDecl())->getQualifiedNameAsString();
			break;

		case TemplateArgument::Integral:
		case TemplateArgument::Expression:
			qualifiedName = exprToText(argument.getAsExpr());
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

// implemented in cpptypeprinter.cpp
std::string cppPrintQualType(const QualType & qualType, const std::string &name);

std::string qualTypeToText(const clang::QualType & qualType, const std::string &name)
{
	std::string text;

	llvm::raw_string_ostream stream(text);
	LangOptions langOptions;
	langOptions.CPlusPlus = 1;
	PrintingPolicy policy(langOptions);
	policy.SuppressSpecifiers = 0;
	QualType::print(qualType.split(), stream, policy, name);

	return stream.str();

//	return cppPrintQualType(qualType, name);

//	string qualifiedName;
//
////qualifiedName = type->getTypeClassName();
//
//	if(qualType->getAsCXXRecordDecl() != NULL) {
//		qualifiedName = getNamedDeclOutputName(qualType->getAsCXXRecordDecl());
//	}
//	else if(qualType->getAs<TemplateSpecializationType>() != NULL){
//		const TemplateSpecializationType * t = qualType->getAs<TemplateSpecializationType>();
//		qualifiedName = getTemplateSpecializationName(t);
//	}
//	else if(qualType->getAs<TemplateTypeParmType>() != NULL){
////		const TemplateTypeParmType * t = qualType->getAs<TemplateTypeParmType>();
//		qualifiedName = qualType.getAsString();
//	}
//	else {
//		qualifiedName = qualType.getAsString();
//	}
//
//	qualifiedName = removeRecordWords(qualifiedName);
//
//	return qualifiedName;
}

std::string exprToText(const clang::Expr * expr)
{
	std::string text;
	raw_string_ostream stream(text);
	LangOptions langOptions;
	langOptions.CPlusPlus = 1;
	PrintingPolicy policy(langOptions);
	policy.SuppressSpecifiers = 0;
	expr->printPretty(stream, NULL, policy);
	return stream.str();
}

std::string namedDeclToText(const NamedDecl * namedDecl)
{
	std::string name, qualifiedName, qualifiedNameWithoutNamespace;
	getNamedDeclNames(namedDecl, name, qualifiedName, qualifiedNameWithoutNamespace);
	return qualifiedNameWithoutNamespace;
}

std::string declToText(const clang::Decl * decl)
{
	std::string text;
	raw_string_ostream stream(text);
	LangOptions langOptions;
	langOptions.CPlusPlus = 1;
	PrintingPolicy policy(langOptions);
	policy.SuppressSpecifiers =0;
	decl->print(stream, policy, NULL, false);
	return stream.str();

	//const NamedDecl * namedDecl = dyn_cast<NamedDecl>(decl);
	//if(namedDecl != NULL) {
	//	return namedDeclToText(namedDecl);
	//}
	//else {
	//	return "";
	//}
}

