#include "cppinvokable.h"
#include "cppcontainer.h"
#include "cpgf/gassert.h"
#include "cpputil.h"

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "llvm/Support/Casting.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/ASTContext.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

using namespace llvm;
using namespace llvm::sys;
using namespace clang;


const FunctionDecl * getFunctionDecl(const Decl * decl)
{
	const FunctionTemplateDecl * functionTemplateDecl = dyn_cast_or_null<FunctionTemplateDecl>(decl);
	if(functionTemplateDecl != NULL) {
		decl = functionTemplateDecl->getTemplatedDecl();
	}

	GASSERT(isa<FunctionDecl>(decl));

	return dyn_cast_or_null<FunctionDecl>(decl);
}

const CXXMethodDecl * getMethodDecl(const Decl * decl)
{
	return dyn_cast_or_null<CXXMethodDecl>(getFunctionDecl(decl));
}

CppInvokable::CppInvokable(const clang::Decl * decl)
	: super(decl)
{
}

bool CppInvokable::isStatic() const
{
	const CXXMethodDecl * methodDecl = getMethodDecl(this->getDecl());

	if(methodDecl != NULL) {
		return methodDecl->isStatic();
	}
	else {
		return true;
	}
}

bool CppInvokable::isConst() const
{
	const CXXMethodDecl * methodDecl = getMethodDecl(this->getDecl());

	if(methodDecl != NULL) {
		return methodDecl->isConst();
	}
	else {
		return false;
	}
}

bool CppInvokable::isVariadic() const
{
	const FunctionDecl * functionDecl = getFunctionDecl(this->getDecl());
	return functionDecl->isVariadic();
}

bool CppInvokable::isTemplate() const
{
	return isa<FunctionTemplateDecl>(this->getDecl());
}

size_t CppInvokable::getArity() const
{
	const FunctionDecl * functionDecl = getFunctionDecl(this->getDecl());
	return functionDecl->param_size();
}

std::string CppInvokable::getTextOfPointeredType() const
{
	QualType qualType = getFunctionDecl(this->getDecl())->getType();
	std::string s;

	if(this->isStatic()) {
		s = "*";
	}
	else {
		s = this->getParent()->getOutputName() + "::*";
	}

	return qualTypeToText(qualType, s);
}

std::string CppInvokable::getTextOfParamList(const ItemTextOptionFlags & options) const
{
	std::string text;
	
	const FunctionDecl * functionDecl = getFunctionDecl(this->getDecl());
	for(FunctionDecl::param_const_iterator it = functionDecl->param_begin(); it != functionDecl->param_end(); ++it) {
		if(! text.empty()) {
			text.append(",");
		}
		if(options.has(itoWithType)) {
			if(! text.empty()) {
				text.append(" ");
			}
			text.append(CppType((*it)->getType()).getQualifiedName());
		}
		if(options.has(itoWithName)) {
			if(! text.empty()) {
				text.append(" ");
			}
			text.append((*it)->getNameAsString());
		}
	}

	return text;
}

CppType CppInvokable::getResultType() const
{
	QualType qualType = getFunctionDecl(this->getDecl())->getResultType();
	return CppType(qualType);
}

