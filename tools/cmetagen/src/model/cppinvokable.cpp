#include "cppinvokable.h"
#include "cpgf/gassert.h"

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "llvm/Support/Casting.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/DeclTemplate.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

using namespace llvm;
using namespace llvm::sys;
using namespace clang;

FunctionDecl * getFunctionDecl(Decl * decl)
{
	FunctionTemplateDecl * functionTemplateDecl = dyn_cast_or_null<FunctionTemplateDecl>(decl);
	if(functionTemplateDecl != NULL) {
		decl = functionTemplateDecl->getTemplatedDecl();
	}

	GASSERT(isa<FunctionDecl>(decl));

	return dyn_cast_or_null<FunctionDecl>(decl);
}

CXXMethodDecl * getMethodDecl(Decl * decl)
{
	return dyn_cast_or_null<CXXMethodDecl>(getFunctionDecl(decl));
}

CppInvokable::CppInvokable(clang::Decl * decl)
	: super(decl)
{
}

bool CppInvokable::isStatic() const
{
	CXXMethodDecl * methodDecl = getMethodDecl(this->getDecl());

	if(methodDecl != NULL) {
		return methodDecl->isStatic();
	}
	else {
		return true;
	}
}

bool CppInvokable::isConst() const
{
	CXXMethodDecl * methodDecl = getMethodDecl(this->getDecl());

	if(methodDecl != NULL) {
		return methodDecl->isConst();
	}
	else {
		return false;
	}
}

bool CppInvokable::isVariadic() const
{
	FunctionDecl * functionDecl = getFunctionDecl(this->getDecl());
	return functionDecl->isVariadic();
}

bool CppInvokable::isTemplate() const
{
	return isa<FunctionTemplateDecl>(this->getDecl());
}

size_t CppInvokable::getArity() const
{
	FunctionDecl * functionDecl = getFunctionDecl(this->getDecl());
	return functionDecl->param_size();
}
