#include "cppclasstraits.h"
#include "cpppolicy.h"

namespace metagen {

bool CppClassTraits::isDestructorHidden() const
{
	return this->flags.has(tfDestructorHidden);
}

void CppClassTraits::setDestructorHidden(bool value)
{
	this->flags.setByBool(tfDestructorHidden, value);
}

bool CppClassTraits::isDefaultConstructorHidden() const
{
	return this->flags.has(tfDefaultConstructorHidden);
}

void CppClassTraits::setDefaultConstructorHidden(bool value)
{
	this->flags.setByBool(tfDefaultConstructorHidden, value);
}

bool CppClassTraits::isCopyConstructorHidden() const
{
	return this->flags.has(tfCopyConstructorHidden);
}

void CppClassTraits::setCopyConstructorHidden(bool value)
{
	this->flags.setByBool(tfCopyConstructorHidden, value);
}

bool CppClassTraits::isHasTypeConvertConstructor() const
{
	return this->flags.has(tfHasTypeConvertConstructor);
}

void CppClassTraits::setHasTypeConvertConstructor(bool value)
{
	this->flags.setByBool(tfHasTypeConvertConstructor, value);
}


} // namespace metagen
