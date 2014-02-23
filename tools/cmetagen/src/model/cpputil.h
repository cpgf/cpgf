#ifndef CPGF_CPPUTIL_H
#define CPGF_CPPUTIL_H

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "clang/AST/Type.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Decl.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include <string>

namespace metagen {

class CppClass;

clang::QualType stripType(const clang::QualType & qualType);

void getNamedDeclNames(const clang::NamedDecl * namedDecl,
	std::string & name,
	std::string & qualifiedName,
	std::string & qualifiedNameWithoutNamespace);

std::string	 getNamedDeclOutputName(const clang::NamedDecl * namedDecl);
std::string	 getNamedDeclQualifiedName(const clang::NamedDecl * namedDecl);

std::string getTemplateSpecializationName(const clang::ASTContext * astContext, const clang::TemplateSpecializationType * type);
std::string getTemplateArgumentName(const clang::ASTContext * astContext, const clang::TemplateArgument & argument);

std::string exprToText(const clang::ASTContext * astContext, const clang::Expr * expr);
std::string declToText(const clang::ASTContext * astContext, const clang::Decl * decl);

std::string getSourceText(const clang::ASTContext * astContext,
	const clang::SourceLocation & start, const clang::SourceLocation & end);


std::string fixIllFormedTemplates(const CppClass * ownerClass, const std::string & text);


} // namespace metagen


#endif
