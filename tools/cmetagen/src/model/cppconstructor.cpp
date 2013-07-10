#include "cppconstructor.h"

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "clang/AST/DeclCXX.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

using namespace clang;

namespace metagen {


CppConstructor::CppConstructor(const clang::Decl * decl)
	: super(decl)
{
}

bool CppConstructor::isCopyConstructor() const
{
	const CXXConstructorDecl * constructorDecl = dyn_cast<CXXConstructorDecl>(this->getDecl());

	return constructorDecl->isCopyOrMoveConstructor();
}

bool CppConstructor::isImplicitTypeConverter() const
{
	const CXXConstructorDecl * constructorDecl = dyn_cast<CXXConstructorDecl>(this->getDecl());

	return constructorDecl->isConvertingConstructor(false);
}


} // namespace metagen
