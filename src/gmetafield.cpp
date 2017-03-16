#include "cpgf/gmetafield.h"


namespace cpgf {

namespace meta_internal {


void GMetaFieldDataBase::deleteSelf()
{
	this->virtualFunctions->deleteSelf(this);
}

bool GMetaFieldDataBase::canGet() const
{
	return this->virtualFunctions->canGet();
}

bool GMetaFieldDataBase::canSet() const
{
	return this->virtualFunctions->canSet();
}

GVariant GMetaFieldDataBase::get(const void * instance) const
{
	return this->virtualFunctions->get(this, instance);
}

void GMetaFieldDataBase::set(void * instance, const GVariant & v) const
{
	this->virtualFunctions->set(this, instance, v);
}

size_t GMetaFieldDataBase::getFieldSize() const
{
	return this->virtualFunctions->getFieldSize();
}

void * GMetaFieldDataBase::getFieldAddress(const void * instance) const
{
	return this->virtualFunctions->getFieldAddress(this, instance);
}


	
} // namespace meta_internal


bool GMetaField::canGet() const
{
	return this->baseData->canGet();
}

bool GMetaField::canSet() const
{
	return this->baseData->canSet();
}

GVariant GMetaField::get(const void * instance) const
{
	if(this->isStatic()) {
		instance = nullptr;
	}

	return this->baseData->get(instance);
}

void GMetaField::set(void * instance, const GVariant & v) const
{
	if(this->isStatic()) {
		instance = nullptr;
	}

	this->baseData->set(instance, v);
}

void * GMetaField::getAddress(const void * instance) const
{
	return this->baseData->getFieldAddress(instance);
}

size_t GMetaField::getSize() const
{
	return this->baseData->getFieldSize();
}

GMetaExtendType GMetaField::getItemExtendType(uint32_t flags) const
{
	return this->baseData->getItemExtendType(flags, this);
}


} // namespace cpgf
