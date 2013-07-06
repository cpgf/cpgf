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

#include "Poco/Format.h"

using namespace llvm;
using namespace llvm::sys;
using namespace clang;
using namespace std;


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

bool CppInvokable::isOverloaded() const
{
	return this->getParent()->getSameNamedItemCount(this) > 1;
}

bool CppInvokable::hasResult() const
{
	return ! this->getResultType().isVoid();
}

size_t CppInvokable::getArity() const
{
	const FunctionDecl * functionDecl = getFunctionDecl(this->getDecl());
	return functionDecl->param_size();
}

CppType CppInvokable::getParamType(size_t index) const
{
	const FunctionDecl * functionDecl = getFunctionDecl(this->getDecl());
	const ParmVarDecl * paramDecl = functionDecl->getParamDecl(index);

	return CppType(paramDecl->getType());
}

bool CppInvokable::paramHasDefaultValue(size_t index) const
{
	const FunctionDecl * functionDecl = getFunctionDecl(this->getDecl());
	const ParmVarDecl * paramDecl = functionDecl->getParamDecl(index);

	return paramDecl->hasDefaultArg();
}

std::string CppInvokable::getTextOfParamDeafultValue(size_t index) const
{
	const FunctionDecl * functionDecl = getFunctionDecl(this->getDecl());
	const ParmVarDecl * paramDecl = functionDecl->getParamDecl(index);

	if(paramDecl->hasDefaultArg()) {
		return exprToText(paramDecl->getDefaultArg());
	}
	else {
		return "";
	}
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

	return CppType(qualType).getQualifiedName(s);
}

std::string CppInvokable::getTextOfParamList(const ItemTextOptionFlags & options) const
{
	std::string text;
	
	const FunctionDecl * functionDecl = getFunctionDecl(this->getDecl());
	int index = -1;
	for(FunctionDecl::param_const_iterator it = functionDecl->param_begin(); it != functionDecl->param_end(); ++it) {
		++index;
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
			string name = (*it)->getNameAsString();
			if(name.empty()) {
				name = Poco::format("pAr9_Am%d", index);
			}
			text.append(name);
		}
	}

	return text;
}

CppType CppInvokable::getFunctionType() const
{
	return CppType(getFunctionDecl(this->getDecl())->getType());
}

CppType CppInvokable::getResultType() const
{
	QualType qualType = getFunctionDecl(this->getDecl())->getResultType();
	return CppType(qualType);
}

