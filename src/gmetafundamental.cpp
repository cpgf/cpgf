#include "cpgf/gmetafundamental.h"



namespace cpgf {


GVariantType GMetaFundamental::getVariantType() const
{
	return this->getData()->getVariantType();
}

GVariant GMetaFundamental::getValue(void * instance) const
{
	return this->getData()->getValue(instance);
}

size_t GMetaFundamental::getTypeSize() const
{
	return this->getData()->getTypeSize();
}

void * GMetaFundamental::createInstance() const
{
	return this->getData()->createInstance();
}

void * GMetaFundamental::createInplace(void * placement) const
{
	return this->getData()->createInplace(placement);
}

void * GMetaFundamental::cloneInstance(void * instance) const
{
	return this->getData()->cloneInstance(instance);
}

void * GMetaFundamental::cloneInplace(void * instance, void * placement) const
{
	return this->getData()->cloneInplace(instance, placement);
}


void GMetaFundamental::destroyInstance(void * instance) const
{
	this->getData()->destroyInstance(instance);
}


const meta_internal::GMetaFundamentalData * GMetaFundamental::getData() const
{
	return static_cast<const meta_internal::GMetaFundamentalData *>(static_cast<const void *>(this->dataBuffer));
}



} // namespace cpgf
