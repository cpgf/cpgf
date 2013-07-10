#include "cppnamespace.h"
#include "cppclass.h"
#include "cppfield.h"
#include "cppmethod.h"
#include "cppenum.h"
#include "cppoperator.h"

#include "util.h"

#include "cpgf/gassert.h"

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "clang/AST/Decl.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

using namespace clang;

namespace metagen {


bool CppNamespace::isAnonymous() const
{
	const NamespaceDecl * namespaceDecl = dyn_cast<NamespaceDecl>(this->getDecl());
	return namespaceDecl->isAnonymousNamespace();
}


} // namespace metagen
