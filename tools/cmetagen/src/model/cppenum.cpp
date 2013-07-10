#include "cppenum.h"

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "clang/AST/Decl.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

using namespace clang;

namespace metagen {


const CppEnum::ValueListType * CppEnum::getValueList() const
{
	return &this->valueList;
}

bool CppEnum::isAnonymous() const
{
	const EnumDecl * enumDecl = dyn_cast<EnumDecl>(this->getDecl());
	return ! enumDecl->getDeclName();
}

void CppEnum::addValue(const std::string & name, const std::string & qualifiedName)
{
	this->valueList.push_back(CppEnumValue(name, qualifiedName));
}


} // namespace metagen
