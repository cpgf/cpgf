#include "cppfield.h"
#include "cpptype.h"

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "clang/AST/Decl.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

using namespace clang;


CppField::CppField(clang::Decl * decl)
	: super(decl), type(NULL), bitFields(0)
{
}

bool CppField::isStatic() const
{
	switch(this->getDecl()->getKind()) {
		case Decl::Var:
			return true;

		case Decl::Field:
			return false;

		default:
			return true;
	}
}
