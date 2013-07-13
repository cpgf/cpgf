#ifndef __CPPUTIL_H
#define __CPPUTIL_H

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


clang::QualType stripType(const clang::QualType & qualType);

void getNamedDeclNames(const clang::NamedDecl * namedDecl,
	std::string & name,
	std::string & qualifiedName,
	std::string & qualifiedNameWithoutNamespace);

std::string	 getNamedDeclOutputName(const clang::NamedDecl * namedDecl);
std::string	 getNamedDeclQualifiedName(const clang::NamedDecl * namedDecl);

std::string getTemplateSpecializationName(const clang::TemplateSpecializationType * type);
std::string getTemplateArgumentName(const clang::TemplateArgument & argument);

std::string exprToText(const clang::Expr * expr);
std::string declToText(const clang::Decl * decl);


} // namespace metagen


#endif
