#include "cpgf/gmetaextendtype.h"


namespace cpgf {


GMetaExtendType::GMetaExtendType()
{
	this->data.arraySize = 0;
}

GMetaExtendType::GMetaExtendType(const GMetaExtendTypeData & data)
	: data(data)
{
}

GMetaExtendType::GMetaExtendType(const GMetaExtendType & other)
	: data(other.data)
{
}

GMetaExtendType::~GMetaExtendType() {
}

GMetaExtendType & GMetaExtendType::operator = (GMetaExtendType other)
{
	this->swap(other);

	return *this;
}

void GMetaExtendType::swap(GMetaExtendType & other)
{
	using std::swap;
	
	swap(this->data, other.data);
}



} // namespace cpgf

