#include "builderinvokable.h"
#include "model/cppinvokable.h"
#include "model/cpppolicy.h"

using namespace std;

namespace metagen {

BuilderInvokable::BuilderInvokable(const CppItem * cppItem)
	: super(cppItem)
{
}

BuilderInvokable::~BuilderInvokable()
{
}

bool BuilderInvokable::canBind() const
{
	const CppInvokable * cppInvokable = static_cast<const CppInvokable *>(this->getCppItem());
	
	if(cppInvokable->isTemplate() || cppInvokable->isVariadic()) {
		return false;
	}

	return super::canBind();
}

std::string getInvokablePolicyText(const CppInvokable * cppInvokable, bool prefixWithComma)
{
	CppPolicy cppPolicy;

	cppInvokable->getPolicy(&cppPolicy);

	return cppPolicy.getTextOfMakePolicy(prefixWithComma) + "()";
}


} // namespace metagen
