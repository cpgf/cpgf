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
		raiseCoreException(Error_Meta_ReadDenied);
	}

	return this->baseData->get(instance);
}

void GMetaProperty::set(void * instance, const GVariant & value) const
{
	if(!this->baseData->canSet()) {
		raiseCoreException(Error_Meta_WriteDenied);
	}

	this->baseData->set(instance, value);
}

void * GMetaProperty::getAddress(void * instance) const
{
	return this->baseData->getPropertyAddress(instance);
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
