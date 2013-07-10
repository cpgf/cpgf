#include "cppoperator.h"
#include "cpptype.h"

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "clang/AST/DeclCXX.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include "Poco/RegularExpression.h"

using namespace clang;

namespace metagen {


CppOperator::CppOperator(const clang::Decl * decl)
	: super(decl)
{
}

const std::string & CppOperator::getOperatorName() const
{
	if(this->operatorName.empty()) {
		if(this->isTypeConverter()) {
			const CXXConversionDecl * conversionDecl = dyn_cast<CXXConversionDecl>(this->getDecl());
			this->operatorName = CppType(conversionDecl->getConversionType()).getQualifiedName();
		}
		else {
			static Poco::RegularExpression re("^\\s*\\boperator\\b\\s*");
			this->operatorName = this->getName();
			re.subst(this->operatorName, "");
		}
	}

	return this->operatorName;
}

bool CppOperator::isFunctor() const
{
	return this->getOperatorName() == "()";
}

bool CppOperator::isArray() const
{
	return this->getOperatorName() == "[]";
}

bool CppOperator::isTypeConverter() const
{
	return this->getDecl()->getKind() == Decl::CXXConversion;
}


} // namespace metagen
