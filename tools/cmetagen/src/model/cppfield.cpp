#include "cppfield.h"

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "clang/AST/Decl.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

using namespace clang;

namespace metagen {


CppField::CppField(const clang::Decl * decl)
	: super(decl)
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

bool CppField::isBitField() const
{
	const FieldDecl * fieldDecl = dyn_cast_or_null<FieldDecl>(this->getDecl());
	return (fieldDecl != NULL) && fieldDecl->isBitField();
}

CppType CppField::getType() const
{
	const FieldDecl * fieldDecl = dyn_cast_or_null<FieldDecl>(this->getDecl());
	return CppType(fieldDecl->getType());
}

bool CppField::isAnonymousStructOrUnion() const
{
	const FieldDecl * fieldDecl = dyn_cast_or_null<FieldDecl>(this->getDecl());
	return (fieldDecl != NULL) && fieldDecl->isAnonymousStructOrUnion();
}


} // namespace metagen
