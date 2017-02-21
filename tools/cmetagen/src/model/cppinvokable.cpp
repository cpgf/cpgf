#include "cppinvokable.h"
#include "cppcontainer.h"
#include "cppclass.h"
#include "cppclasstraits.h"
#include "cppcontext.h"
#include "cpppolicy.h"
#include "cpputil.h"

#include "cpgf/gassert.h"

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

namespace metagen {


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

bool CppInvokable::isVirtual() const
{
	const CXXMethodDecl * methodDecl = getMethodDecl(this->getDecl());

	if(methodDecl != NULL) {
		return methodDecl->isVirtual();
	}
	else {
		return false;
	}
}

bool CppInvokable::isPureVirtual() const
{
	const CXXMethodDecl * methodDecl = getMethodDecl(this->getDecl());

	if(methodDecl != NULL) {
		return methodDecl->isPure();
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
	if(this->isConstructor()) {
		return false;
	}
	else {
		return ! this->getResultType().isVoid();
	}
}

size_t CppInvokable::getArity() const
{
	const FunctionDecl * functionDecl = getFunctionDecl(this->getDecl());
	return functionDecl->param_size();
}

size_t CppInvokable::getNonDefaultParamCount() const
{
	size_t paramCount = this->getArity();
	while(paramCount > 0 && this->paramHasDefaultValue(paramCount - 1)) {
		--paramCount;
	}
	return paramCount;
}

CppType CppInvokable::getParamType(size_t index) const
{
	const FunctionDecl * functionDecl = getFunctionDecl(this->getDecl());
	const ParmVarDecl * paramDecl = functionDecl->getParamDecl((unsigned int)index);

	return CppType(paramDecl->getType());
}

bool CppInvokable::paramHasDefaultValue(size_t index) const
{
	const FunctionDecl * functionDecl = getFunctionDecl(this->getDecl());
	const ParmVarDecl * paramDecl = functionDecl->getParamDecl((unsigned int)index);

	return paramDecl->hasDefaultArg();
}

std::string CppInvokable::getParamName(size_t index) const
{
	const FunctionDecl * functionDecl = getFunctionDecl(this->getDecl());
	const ParmVarDecl * paramDecl = functionDecl->getParamDecl((unsigned int)index);
	return paramDecl->getNameAsString();
}

std::string CppInvokable::getTextOfUnusedParamsPlaceholder() const
{
	string result;

	const FunctionDecl * functionDecl = getFunctionDecl(this->getDecl());
	for(FunctionDecl::param_const_iterator it = functionDecl->param_begin(); it != functionDecl->param_end(); ++it) {
		if(! result.empty()) {
			result.append(", ");
		}
		result.append("(void)" + (*it)->getNameAsString());
	}

	if(! result.empty()) {
		result.append(";");
	}

	return result;
}

std::string CppInvokable::getTextOfParamDeafultValue(size_t index) const
{
	const FunctionDecl * functionDecl = getFunctionDecl(this->getDecl());
	const ParmVarDecl * paramDecl = functionDecl->getParamDecl((unsigned int)index);

	if(paramDecl->hasDefaultArg()) {
		return exprToText(this->getASTContext(), paramDecl->getDefaultArg());
	}
	else {
		return "";
	}
}

std::string CppInvokable::getTextOfPointeredType(bool includeClassName) const
{
	QualType qualType = getFunctionDecl(this->getDecl())->getType();
	std::string s;

	if(this->isStatic() || !includeClassName) {
		s = "*";
	}
	else {
		s = getCppContainerInstantiationName(this->getParent()) + "::*";
	}

	string text = CppType(qualType).getQualifiedName(s);
	text = fixIllFormedTemplates(dynamic_cast<const CppClass *>(this->getParent()), text);
	return text;
}

bool CppInvokable::hasTemplateDependentParam() const
{
	size_t paramCount = this->getArity();
	for(size_t i = 0; i < paramCount; ++i) {
		if(this->getParamType(i).isTemplateDependent()) {
			return true;
		}
	}

	return false;
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

		string name = (*it)->getNameAsString();
		if(name.empty()) {
			name = Poco::format("pAr9_Am%d", index);
		}
		if(options.has(itoWithArgType)) {
			if(! text.empty()) {
				text.append(" ");
			}
			text.append(CppType((*it)->getType()).getQualifiedName(
				options.has(itoWithArgName) ? name : ""
				));
		}
		else if(options.has(itoWithArgName)) {
			if(! text.empty()) {
				text.append(" ");
			}
			text.append(name);
		}
		if(options.has(itoWithDefaultValue) && paramHasDefaultValue(index)) {
			if(options.hasAny(itoWithArgType | itoWithArgName)) {
				text.append(" = ");
			}
			text.append(this->getTextOfParamDeafultValue(index));
		}
	}

	return text;
}

std::string CppInvokable::getText(const ItemTextOptionFlags & options) const
{
	string name = this->isConstructor() ? this->getParent()->getName() : this->getName();
	return this->getTextWithReplacedName(options, name);
}

std::string CppInvokable::getTextWithReplacedName(const ItemTextOptionFlags & options, const std::string & replacedName) const
{
	string text;

	if(options.hasAny(itoIncludeArg)) {
		text = Poco::format("(%s)", this->getTextOfParamList(options));
	}

	if(options.has(itoWithName)) {
		string name = replacedName;
		if(options.has(itoWithParentName)) {
			if(! this->isStatic()) {
				name = this->getParent()->getQualifiedName() + "::" + name;
			}
			if(options.has(itoAsPointer)) {
				name = Poco::format("(%s*)", name);
			}
		}
		text = name + text;
	}
	if(options.has(itoWithResult) && ! this->isConstructor()) {
		text = this->getResultType().getQualifiedName(text);
	}
	if(options.has(itoWithQualifiers)) {
		if(this->isConst()) {
			text = text + " const";
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

void CppInvokable::getPolicy(CppPolicy * outPolicy) const
{
	if(this->hasResult()) {
		this->doGetPolicy(outPolicy, this->getResultType(), -1);
	}

	for(size_t i = 0; i < this->getArity(); ++i) {
		this->doGetPolicy(outPolicy, this->getParamType(i), (int)i);
	}
}

void CppInvokable::doGetPolicy(CppPolicy * outPolicy, const CppType & type, int index) const
{
	const CppClass * cppClass = this->getCppContext()->findClassByType(type.getBaseType());
	if(cppClass != NULL) {
		CppClassTraits classTraits = this->getCppContext()->getClassTraits(cppClass);
		if(type.isReferenceToConst()) {
			if(classTraits.isHasTypeConvertConstructor()) {
				outPolicy->addRule(makeIndexedRule(ruleCopyConstReference, index));
			}
		}
		else if(type.isPointer()) {
		}
		else {
			if(classTraits.isCopyConstructorHidden()) {
				outPolicy->addRule(makeIndexedRule(ruleParamNoncopyable, index));
			}
		}
	}
}

} // namespace metagen
