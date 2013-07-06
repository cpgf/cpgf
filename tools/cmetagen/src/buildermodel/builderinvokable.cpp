#include "builderinvokable.h"
#include "model/cppinvokable.h"


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

