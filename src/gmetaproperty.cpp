#include "cpgf/gmetaproperty.h"
#include "cpgf/gexception.h"


namespace cpgf {


bool GMetaProperty::canGet() const
{
	return this->baseData->canGet();
}

bool GMetaProperty::canSet() const
{
	return this->baseData->canSet();
}

GVariant GMetaProperty::get(void * instance) const
{
	if(!this->baseData->canGet()) {
		raiseFormatException(Error_Meta_ReadDenied, "Can't read property %s", this->getName().c_str());
	}

	return this->baseData->get(instance);
}

void GMetaProperty::set(void * instance, const GVariant & value) const
{
	if(!this->baseData->canSet()) {
		raiseFormatException(Error_Meta_WriteDenied, "Can't write property %s", this->getName().c_str());
	}

	this->baseData->set(instance, value);
}

size_t GMetaProperty::getSize() const
{
	return this->baseData->getPropertySize();
}

const GMetaType & GMetaProperty::getItemType() const
{
	return super::getItemType();
}

GMetaConverter * GMetaProperty::createConverter() const
{
	return this->baseData->createConverter();
}



} // namespace cpgf
