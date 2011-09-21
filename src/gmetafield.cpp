#include "cpgf/gmetafield.h"


namespace cpgf {


bool GMetaField::canGet() const
{
	return this->baseData->canGet();
}

bool GMetaField::canSet() const
{
	return this->baseData->canSet();
}

GVariant GMetaField::get(void * instance) const
{
	if(this->isStatic()) {
		instance = NULL;
	}

	return this->baseData->get(instance);
}

void GMetaField::set(void * instance, const GVariant & v) const
{
	if(this->isStatic()) {
		instance = NULL;
	}

	this->baseData->set(instance, v);
}

size_t GMetaField::getSize() const
{
	return this->baseData->getFieldSize();
}

void * GMetaField::getAddress(void * instance) const
{
	return this->baseData->getFieldAddress(instance);
}

GMetaConverter * GMetaField::createConverter() const
{
	return this->baseData->createConverter();
}


} // namespace cpgf
