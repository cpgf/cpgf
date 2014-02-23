#include "cppmethod.h"
#include "cppcontainer.h"

namespace metagen {


CppMethod::CppMethod(const clang::Decl * decl)
	: super(decl)
{
}

bool CppMethod::isArityUnique() const
{
	size_t nonDefaultParamCount = this->getNonDefaultParamCount();

	for(CppContainer::MethodListType::const_iterator it = this->getParent()->getMethodList()->begin();
		it != this->getParent()->getMethodList()->end();
		++it) {
		if((*it) == this) {
			continue;
		}
		if((*it)->getNonDefaultParamCount() == nonDefaultParamCount
			&& (*it)->getQualifiedName() == this->getQualifiedName()) {
			return false;
		}
	}

	return true;
}


} // namespace metagen
