#include "cppclass.h"
#include "cppconstructor.h"
#include "cppdestructor.h"
#include "cppcontext.h"
#include "cpputil.h"

#include "util.h"

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


BaseClass::BaseClass(const clang::CXXBaseSpecifier * baseSpecifier, const CppContext * cppContext)
	: baseSpecifier(baseSpecifier), cppContext(cppContext)
{
}

ItemVisibility accessToVisibility(AccessSpecifier access);
ItemVisibility BaseClass::getVisibility() const
{
	return accessToVisibility(this->baseSpecifier->getAccessSpecifier());
}

std::string BaseClass::getQualifiedName() const
{
	return CppType(this->baseSpecifier->getType()).getQualifiedName();
}

const CppClass * BaseClass::getCppClass() const
{
	return static_cast<const CppClass *>(this->cppContext->findNamedItem(icClass, this->getQualifiedName()));
}

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
				defaultValue = exprToText(paramDecl->getDefaultArgument());
			}
		}
		else if(kind == Decl::TemplateTemplateParm) {
			const TemplateTemplateParmDecl * paramDecl = dyn_cast<TemplateTemplateParmDecl>(namedDecl);
			if(options.has(itoWithArgType)) {
				text.append(declToText(paramDecl));
			}
			else {
				text.append(paramDecl->getNameAsString());
			}
			if(paramDecl->hasDefaultArgument()) {
				defaultValue = getTemplateArgumentName(paramDecl->getDefaultArgument().getArgument());
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


} // namespace metagen
