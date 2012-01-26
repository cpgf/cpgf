#include "cpgf/gmetaenum.h"

#include "pinclude/gmetatypereg.h"

#include <vector>


namespace cpgf {

namespace meta_internal {


class GMetaEnumDataImplement
{
public:
	std::vector<GVariant> enumerators;
	std::vector<std::string> keyNameList;
};


GMetaEnumData::GMetaEnumData(size_t typeSize)
	: implement(new GMetaEnumDataImplement), typeSize(typeSize)
{
}

GMetaEnumData::~GMetaEnumData()
{
}

size_t GMetaEnumData::getTypeSize() const
{
	return this->typeSize;
}

size_t GMetaEnumData::getCount() const
{
	return this->implement->enumerators.size();
}

const char * GMetaEnumData::getKey(size_t index) const
{
	if(index >= this->getCount()) {
		return NULL;
	}
	else {
		return this->implement->keyNameList[index].c_str();
	}
}

GVariant GMetaEnumData::getValue(size_t index) const
{
	if(index >= this->getCount()) {
		return 0;
	}
	else {
		return this->implement->enumerators[index];
	}
}

int GMetaEnumData::findKey(const char * key) const
{
	size_t count = this->getCount();

	for(size_t i = 0; i < count; ++i) {
		if(strcmp(this->getKey(i), key) == 0) {
			return static_cast<int>(i);
		}
	}

	return -1;
}

void GMetaEnumData::addEnum(const char * key, const GVariant & value)
{
	this->implement->keyNameList.push_back(key);
	this->implement->enumerators.push_back(value);
}

void GMetaEnumData::addValue(const GVariant & value)
{
	this->implement->enumerators.push_back(value);
}

void * GMetaEnumData::createInstance() const
{
	return newZeroBuffer(NULL, this->getTypeSize(), NULL);
}

void * GMetaEnumData::createInplace(void * placement) const
{
	return newZeroBuffer(placement, this->getTypeSize(), NULL);
}

void * GMetaEnumData::cloneInstance(void * instance) const
{
	return newZeroBuffer(NULL, this->getTypeSize(), instance);
}

void * GMetaEnumData::cloneInplace(void * instance, void * placement) const
{
	return newZeroBuffer(placement, this->getTypeSize(), instance);
}

void GMetaEnumData::destroyInstance(void * instance) const
{
	delete static_cast<char *>(instance);
}



} // namespace meta_internal



GMetaEnum::GMetaEnum(const char * name, const GMetaType & itemType, meta_internal::GMetaEnumData * baseData)
	: super(name, itemType, mcatEnum), baseData(baseData)
{
	this->addModifier(metaModifierStatic);
	
	meta_internal::registerMetaTypedItem(this);
}

GMetaEnum::~GMetaEnum()
{
	meta_internal::removeMetaTypedItem(this);
}

size_t GMetaEnum::getTypeSize() const
{
	return this->baseData->getTypeSize();
}

size_t GMetaEnum::getCount() const
{
	return this->baseData->getCount();
}

const char * GMetaEnum::getKey(size_t index) const
{
	return this->baseData->getKey(index);
}

GVariant GMetaEnum::getValue(size_t index) const
{
	return this->baseData->getValue(index);
}

int GMetaEnum::findKey(const char * key) const
{
	return this->baseData->findKey(key);
}

GMetaEnum & GMetaEnum::operator () (const char * key, const GVariant & value)
{
	this->baseData->addEnum(key, value);

	return *this;
}

void * GMetaEnum::createInstance() const
{
	return this->baseData->createInstance();
}

void * GMetaEnum::createInplace(void * placement) const
{
	return this->baseData->createInplace(placement);
}

void * GMetaEnum::cloneInstance(void * instance) const
{
	return this->baseData->cloneInstance(instance);
}

void * GMetaEnum::cloneInplace(void * instance, void * placement) const
{
	return this->baseData->cloneInplace(instance, placement);
}


void GMetaEnum::destroyInstance(void * instance) const
{
	this->baseData->destroyInstance(instance);
}



} // namespace cpgf
