#include "cppclass.h"
#include "cppconstructor.h"
#include "cppdestructor.h"
#include "cppcontext.h"
#include "cpppolicy.h"
#include "cpputil.h"
#include "util.h"

#include "cpgf/gassert.h"

#include "Poco/Format.h"

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "clang/AST/DeclTemplate.h"
#include "clang/AST/DeclCXX.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

using namespace clang;
using namespace std;

namespace metagen {

const CXXRecordDecl * getRecordDecl(const Decl * decl)
{
	const CXXRecordDecl * recordDecl;
	const ClassTemplateDecl * templateDecl = dyn_cast<ClassTemplateDecl>(decl);
	if(templateDecl != NULL) {
		recordDecl = templateDecl->getTemplatedDecl();
	}
	else {
		recordDecl = dyn_cast<CXXRecordDecl>(decl);
	}
	return recordDecl;
}

BaseClass::BaseClass(const clang::CXXBaseSpecifier * baseSpecifier, const CppContext * cppContext,
	const CppClass * masterCppClass)
	: baseSpecifier(baseSpecifier), cppContext(cppContext), masterCppClass(masterCppClass)
{
}

ItemVisibility accessToVisibility(AccessSpecifier access);
ItemVisibility BaseClass::getVisibility() const
{
	return accessToVisibility(this->baseSpecifier->getAccessSpecifier());
}

std::string BaseClass::getQualifiedName() const
{
	CppType cppType(this->baseSpecifier->getType());
	return fixIllFormedTemplates(this->masterCppClass, cppType.getQualifiedName());
/*
	if(cppType.isTemplateDependent()) {
		return getSourceText(this->masterCppClass->getASTContext(),
			this->baseSpecifier->getTypeSourceInfo()->getTypeLoc().getBeginLoc(),
			this->baseSpecifier->getTypeSourceInfo()->getTypeLoc().getEndLoc()
		);
	}
	else {
		return cppType.getQualifiedName();
	}
*/
}

const CppClass * BaseClass::getCppClass() const
{
	const CppItem * cppItem = this->cppContext->findClassByType(CppType(this->baseSpecifier->getType()));
	GASSERT(cppItem != NULL && cppItem->isClass());
	return static_cast<const CppClass *>(cppItem);
}

CppClass::CppClass(const clang::Decl * decl)
	: super(decl), destructor(NULL)
{
}

CppClass::~CppClass()
{
	clearPointerContainer(this->baseClassList);
}

bool CppClass::isTemplate() const
{
	return isa<ClassTemplateDecl>(this->getDecl());
}

bool CppClass::isChainedTemplate() const
{
	if(this->isTemplate()) {
		return true;
	}
	else if(this->getParent()->isClass()) {
		return static_cast<const CppClass *>(this->getParent())->isChainedTemplate();
	}
	else {
		return false;
	}
}

bool CppClass::isAnonymous() const
{
	const CXXRecordDecl * cxxRecordDecl = getRecordDecl(this->getDecl());
	return cxxRecordDecl->isAnonymousStructOrUnion();
}

bool CppClass::isAbstract() const
{
	const CXXRecordDecl * cxxRecordDecl = getRecordDecl(this->getDecl());
	return cxxRecordDecl->isAbstract();
}

CppClassTraits CppClass::getClassTraits() const
{
	CppClassTraits classTraits;

	for(ConstructorListType::const_iterator it = this->constructorList.begin();
		it != this->constructorList.end();
		++it) {
		const CppConstructor * ctor = *it;
		if(ctor->isImplicitTypeConverter()) {
			classTraits.setHasTypeConvertConstructor(true);
		}
		if(! isVisibilityAllowed(ctor->getVisibility(), this->getCppContext()->getProject())) {
			if(ctor->isCopyConstructor()) {
				classTraits.setCopyConstructorHidden(true);
			}
		}
	}

	if(this->isDefaultConstructorHidden()) {
		classTraits.setDefaultConstructorHidden(true);
	}

	if(this->getDestructor() != NULL
		&& ! isVisibilityAllowed(this->getDestructor()->getVisibility(), this->getCppContext()->getProject())) {
		classTraits.setDestructorHidden(true);
	}

	return classTraits;
}

bool CppClass::isDefaultConstructorHidden() const
{
	for(ConstructorListType::const_iterator it = this->constructorList.begin();
		it != this->constructorList.end();
		++it) {
		const CppConstructor * ctor = *it;
		if(ctor->isDefaultConstructor()) {
			if(isVisibilityAllowed(ctor->getVisibility(), this->getCppContext()->getProject())) {
				return false;
			}
			else {
				return true;
			}
		}
	}

	// We didn't find default constructor, so if the constructorList is not empty,
	// there is non-default constructor which hides the implicit default constructor.
	return ! this->constructorList.empty();
}

void CppClass::doAddItem(CppItem * item)
{
	switch(item->getCategory()) {
		case icConstructor:
			this->pushItem(this->constructorList, item);
			break;

		case icDestructor:
			this->destructor = static_cast<CppDestructor *>(item);
			break;
			
		default:
			super::doAddItem(item);
			break;
	}
}


int CppClass::getTemplateDepth() const
{
	if(! this->isTemplate()) {
		return -1;
	}

	const TemplateDecl * templateDecl = dyn_cast<TemplateDecl>(this->getDecl());
	const TemplateParameterList * templateParamList = templateDecl->getTemplateParameters();
	return (int)(templateParamList->getDepth());
}

int CppClass::getTemplateParamCount() const
{
	const TemplateDecl * templateDecl = dyn_cast<TemplateDecl>(this->getDecl());
	const TemplateParameterList * templateParamList = templateDecl->getTemplateParameters();
	return (int)(templateParamList->size());
}

std::string CppClass::getTemplateParamName(int paramIndex) const
{
	const TemplateDecl * templateDecl = dyn_cast<TemplateDecl>(this->getDecl());
	const TemplateParameterList * templateParamList = templateDecl->getTemplateParameters();
	
	const NamedDecl * namedDecl = templateParamList->getParam(paramIndex);
	Decl::Kind kind = namedDecl->getKind();

	if(kind == Decl::TemplateTypeParm) {
		const TemplateTypeParmDecl * paramDecl = dyn_cast<TemplateTypeParmDecl>(namedDecl);
		return paramDecl->getNameAsString();
	}
	else if(kind == Decl::NonTypeTemplateParm) {
		const NonTypeTemplateParmDecl * paramDecl = dyn_cast<NonTypeTemplateParmDecl>(namedDecl);
		return paramDecl->getNameAsString();
	}
	else if(kind == Decl::TemplateTemplateParm) {
		const TemplateTemplateParmDecl * paramDecl = dyn_cast<TemplateTemplateParmDecl>(namedDecl);
			return paramDecl->getNameAsString();
	}
	return "";
}

std::string CppClass::getTextOfTemplateParamList(const ItemTextOptionFlags & options) const
{
	if(! this->isTemplate()) {
		return "";
	}

	string text;
	const TemplateDecl * templateDecl = dyn_cast<TemplateDecl>(this->getDecl());
	const TemplateParameterList * templateParamList = templateDecl->getTemplateParameters();
	for(TemplateParameterList::const_iterator it = templateParamList->begin(); it != templateParamList->end(); ++it) {
		if(! text.empty()) {
			text.append(", ");
		}

		const NamedDecl * namedDecl = *it;
		Decl::Kind kind = namedDecl->getKind();

		string defaultValue;
		if(kind == Decl::TemplateTypeParm) {
			const TemplateTypeParmDecl * paramDecl = dyn_cast<TemplateTypeParmDecl>(namedDecl);
			if(options.has(itoWithArgType)) {
				text.append(paramDecl->wasDeclaredWithTypename() ? "typename " : "class ");
			}
			text.append(paramDecl->getNameAsString());
			if(paramDecl->hasDefaultArgument()) {
				defaultValue = CppType(paramDecl->getDefaultArgument()).getQualifiedName();
			}
		}
		else if(kind == Decl::NonTypeTemplateParm) {
			const NonTypeTemplateParmDecl * paramDecl = dyn_cast<NonTypeTemplateParmDecl>(namedDecl);
			if(options.has(itoWithArgType)) {
				text.append(CppType(paramDecl->getType()).getQualifiedName(paramDecl->getNameAsString()));
			}
			else {
				text.append(paramDecl->getNameAsString());
			}
			if(paramDecl->hasDefaultArgument()) {
				defaultValue = exprToText(this->getASTContext(), paramDecl->getDefaultArgument());
			}
		}
		else if(kind == Decl::TemplateTemplateParm) {
			const TemplateTemplateParmDecl * paramDecl = dyn_cast<TemplateTemplateParmDecl>(namedDecl);
			if(options.has(itoWithArgType)) {
				text.append(declToText(this->getASTContext(), paramDecl));
			}
			else {
				text.append(paramDecl->getNameAsString());
			}
			if(paramDecl->hasDefaultArgument()) {
				defaultValue = getTemplateArgumentName(this->getASTContext(), paramDecl->getDefaultArgument().getArgument());
			}
		}
		if(! defaultValue.empty() && options.has(itoWithDefaultValue)) {
			text.append(" = " + defaultValue);
		}
	}

	return text;
}

std::string CppClass::getTextOfChainedTemplateParamList(const ItemTextOptionFlags & options) const
{
	string text;
	if(this->getParent()->isClass()) {
		text = static_cast<const CppClass *>(this->getParent())->getTextOfChainedTemplateParamList(options);
	}
	if(this->isTemplate()) {
		if(! text.empty()) {
			text.append(", ");
		}
		text.append(this->getTextOfTemplateParamList(options));
	}
	return text;
}

std::string CppClass::getTextOfQualifedInstantiationName() const
{
	string text;
	if(this->getParent()->isClass()) {
		text = static_cast<const CppClass *>(this->getParent())->getTextOfQualifedInstantiationName();
	}
	else {
		text = this->getParent()->getQualifiedName();
	}
	if(! text.empty()) {
		text.append("::");
	}
	if(this->isTemplate()) {
		text.append(Poco::format("%s<%s >", this->getName(), this->getTextOfTemplateParamList(itoWithArgName)));
	}
	else {
		text.append(this->getName());
	}
	return text;
}

void CppClass::getPolicy(CppPolicy * outPolicy) const
{
	CppClassTraits classTraits = this->getCppContext()->getClassTraits(this);
	
	if(classTraits.isCopyConstructorHidden()) {
		outPolicy->addRule(ruleCopyConstructorAbsent);
	}
	if(classTraits.isDefaultConstructorHidden()) {
		outPolicy->addRule(ruleDefaultConstructorAbsent);
	}
	if(classTraits.isDestructorHidden()) {
		outPolicy->addRule(ruleDestructorAbsent);
	}
}

} // namespace metagen
