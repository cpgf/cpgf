#include "cpgf/gmetaextendtype.h"


namespace cpgf {


namespace {
	void retainInterface(IObject * i)
	{
		if(i != NULL) {
			i->addReference();
		}
	}

	void releaseInterface(IObject * i)
	{
		if(i != NULL) {
			i->releaseReference();
		}
	}
	
	void initExtendTypeData(GMetaExtendTypeData * data)
	{
		data->arraySize = 0;
		data->converter = NULL;
	}
} // unnamed namespace


GMetaExtendType::GMetaExtendType()
{
	initExtendTypeData(&this->data);
}

GMetaExtendType::GMetaExtendType(const GMetaExtendTypeData & data)
	: data(data)
{
}

GMetaExtendType::GMetaExtendType(const GMetaExtendType & other)
	: data(other.data)
{
	this->doRetainInterfaces();
}

GMetaExtendType::~GMetaExtendType()
{
	this->doReleaseInterfaces();
}

GMetaExtendType & GMetaExtendType::operator = (GMetaExtendType other)
{
	this->swap(other);

	return *this;
}

void GMetaExtendType::doRetainInterfaces()
{
	retainInterface(this->data.converter);
}

void GMetaExtendType::doReleaseInterfaces()
{
	releaseInterface(this->data.converter);
}

void GMetaExtendType::swap(GMetaExtendType & other)
{
	using std::swap;
	
	swap(this->data, other.data);
}

GMetaExtendTypeData GMetaExtendType::takeData()
{
	GMetaExtendTypeData retsultData = this->data;
	
	initExtendTypeData(&this->data);

	return retsultData;
}

uint32_t GMetaExtendType::getArraySize() const
{
	return this->data.arraySize;
}

IMetaConverter * GMetaExtendType::getConverter() const
{
	if(this->data.converter != NULL) {
		this->data.converter->addReference();
		return this->data.converter;
	}
	else {
		return NULL;
	}
}

IMetaSerializer * GMetaExtendType::getSerializer() const
{
	if(this->data.serializer != NULL) {
		this->data.serializer->addReference();
		return this->data.serializer;
	}
	else {
		return NULL;
	}
}


} // namespace cpgf

