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
		data->serializer = NULL;
		data->scriptWrapper = NULL;
		data->sharedPointerTraits = NULL;
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
	retainInterface(this->data.serializer);
	retainInterface(this->data.scriptWrapper);
	retainInterface(this->data.sharedPointerTraits);
}

void GMetaExtendType::doReleaseInterfaces()
{
	releaseInterface(this->data.converter);
	releaseInterface(this->data.serializer);
	releaseInterface(this->data.scriptWrapper);
	releaseInterface(this->data.sharedPointerTraits);
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

IMetaScriptWrapper * GMetaExtendType::getScriptWrapper() const
{
	if(this->data.scriptWrapper != NULL) {
		this->data.scriptWrapper->addReference();
		return this->data.scriptWrapper;
	}
	else {
		return NULL;
	}
}

IMetaSharedPointerTraits * GMetaExtendType::getSharedPointerTraits() const
{
	if(this->data.sharedPointerTraits != NULL) {
		this->data.sharedPointerTraits->addReference();
		return this->data.sharedPointerTraits;
	}
	else {
		return NULL;
	}
}



} // namespace cpgf

