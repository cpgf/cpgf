#include "cpgf/gmetafield.h"


namespace cpgf {

namespace meta_internal {


void GMetaFieldDataBase::deleteObject()
{
	this->virtualFunctions->deleteObject(this);
}

bool GMetaFieldDataBase::canGet() const
{
	return this->virtualFunctions->canGet(this);
}

bool GMetaFieldDataBase::canSet() const
{
	return this->virtualFunctions->canSet(this);
}

GVariant GMetaFieldDataBase::get(void * instance) const
{
	return this->virtualFunctions->get(this, instance);
}

void GMetaFieldDataBase::set(void * instance, const GVariant & v) const
{
	this->virtualFunctions->set(this, instance, v);
}

size_t GMetaFieldDataBase::getFieldSize() const
{
	return this->virtualFunctions->getFieldSize(this);
}

void * GMetaFieldDataBase::getFieldAddress(void * instance) const
{
	return this->virtualFunctions->getFieldAddress(this, instance);
}

GMetaExtendType GMetaFieldDataBase::getItemExtendType(uint32_t flags) const
{
	return this->virtualFunctions->getItemExtendType(this, flags);
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

void * GMetaField::getAddress(void * instance) const
{
	return this->baseData->getFieldAddress(instance);
}

size_t GMetaField::getSize() const
{
	return this->baseData->getFieldSize();
}

GMetaExtendType GMetaField::getItemExtendType(uint32_t flags) const
{
	return this->baseData->getItemExtendType(flags);
}


} // namespace cpgf
