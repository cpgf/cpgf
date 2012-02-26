#include "cpgf/gmetafundamental.h"



namespace cpgf {


namespace meta_internal {


size_t GMetaFundamentalData::getTypeSize() const
{
	return this->virtualFunctions->getTypeSize(this);
}

GVariantType GMetaFundamentalData::getVariantType() const
{
	return this->virtualFunctions->getVariantType(this);
}

GVariant GMetaFundamentalData::getValue(void * instance) const
{
	return this->virtualFunctions->getValue(this, instance);
}

void * GMetaFundamentalData::createInstance() const
{
	return this->virtualFunctions->createInstance(this);
}

void * GMetaFundamentalData::createInplace(void * placement) const
{
	return this->virtualFunctions->createInplace(this, placement);
}

void * GMetaFundamentalData::cloneInstance(void * instance) const
{
	return this->virtualFunctions->cloneInstance(this, instance);
}

void * GMetaFundamentalData::cloneInplace(void * instance, void * placement) const
{
	return this->virtualFunctions->cloneInplace(this, instance, placement);
}

void GMetaFundamentalData::destroyInstance(void * o) const
{
	this->virtualFunctions->destroyInstance(this, o);
}


} // namespace meta_internal


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
