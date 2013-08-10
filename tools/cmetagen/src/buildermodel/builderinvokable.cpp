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

bool BuilderInvokable::doCanBind() const
{
	const CppInvokable * cppInvokable = static_cast<const CppInvokable *>(this->getCppItem());
	
	if(cppInvokable->isTemplate() || cppInvokable->isVariadic()) {
		return false;
	}

	return super::doCanBind();
}

std::string getInvokablePolicyText(const CppInvokable * cppInvokable, bool prefixWithComma)
{
	CppPolicy cppPolicy;

	cppInvokable->getPolicy(&cppPolicy);

	string s = cppPolicy.getTextOfMakePolicy(prefixWithComma);
	if(! s.empty()) {
		s.append("()");
	}
	return s;
}


} // namespace metagen
