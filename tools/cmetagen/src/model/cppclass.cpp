#include "cppclass.h"
#include "cppconstructor.h"
#include "cppdestructor.h"

#include "util.h"

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "clang/AST/DeclTemplate.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

using namespace clang;

CppClass::CppClass(clang::Decl * decl)
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

void CppClass::doAddItem(CppItem * item)
{
	switch(item->getCategory()) {
		case icConstructor:
			this->constructorList.push_back(static_cast<CppConstructor *>(item));
			break;

		case icDestructor:
			this->destructor = static_cast<CppDestructor *>(item);
			break;
			
		default:
			super::doAddItem(item);
			break;
	}
}

