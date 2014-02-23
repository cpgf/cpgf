#include "cppcontext.h"
#include "cppclass.h"
#include "cppnamespace.h"
#include "cppfile.h"
#include "cpputil.h"
#include "util.h"

#include "cpgf/gassert.h"

#include <utility>

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

CppContext::CppContext(const Project * project)
	: project(project), parser(project)
{
}

CppContext::~CppContext()
{
	clearPointerContainer(this->itemList);
}

void CppContext::process(const CppSourceFile & sourceFile)
{
	this->parser.parse(this, sourceFile);
}

void CppContext::beginFile(clang::Decl * decl)
{
	GASSERT(! this->cppFile);

	this->cppFile.reset(new CppFile(decl));
}

void CppContext::endFile()
{
}

void CppContext::itemCreated(CppItem * item)
{
	this->itemList.push_back(item);
	item->setCppContext(this);
	
	if(item->isNamed()) {
		this->itemNameMap.insert(make_pair(this->getDeclKeyName(item->getDecl()), item));
	}
}

std::string CppContext::getDeclKeyName(const clang::Decl * decl) const
{
	string result;
	if(isa<NamedDecl>(decl)) {
		result = getNamedDeclQualifiedName(dyn_cast<NamedDecl>(decl));
	}
	if(result.empty()) {
		result = decl->getDeclKindName();
	}
	return result;
}

CppItem * CppContext::findItemByDecl(const clang::Decl * decl) const
{
	if(decl == NULL || dyn_cast<NamedDecl>(decl) == NULL) {
		return NULL;
	}

	ItemNameMapType::const_iterator it = this->itemNameMap.find(this->getDeclKeyName(decl));
	if(it == this->itemNameMap.end()) {
		return NULL;
	}
	else {
		return it->second;
	}
}

CppClass * CppContext::findClassByType(const CppType & cppType) const
{
	const clang::Decl * decl = cppType.getCXXRecordDecl();

	if(decl != NULL && dyn_cast<ClassTemplateSpecializationDecl>(decl) != NULL) {
		decl = dyn_cast<ClassTemplateSpecializationDecl>(decl)->getSpecializedTemplate();
	}

	CppItem * cppItem = this->findItemByDecl(decl);
	if(cppItem == NULL || ! cppItem->isClass()) {
		return NULL;
	}
	else {
		return static_cast<CppClass *>(cppItem);
	}
}

CppClassTraits CppContext::getClassTraits(const CppClass * cppClass) const
{
	return cppClass->getClassTraits();
}


} // namespace metagen
