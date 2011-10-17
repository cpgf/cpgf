#ifndef __GMETAVARIANT_H
#define __GMETAVARIANT_H

#include "cpgf/gvariant.h"
#include "cpgf/gmetatype.h"

namespace cpgf {

#pragma pack(push, 1)
#pragma pack(1)
struct GMetaVarData
{
	GVariantData varData;
	GMetaTypeData typeData;
};
#pragma pack(pop)


class GMetaVariant
{
public:
	GMetaVariant() {
	}

	template <typename T>
	GMetaVariant(const T & v) : value(v), type(createMetaType<T>()) {
		fixupMetaType(&this->type);
	}

	GMetaVariant(const GMetaVarData & data) : value(data.varData), type(data.typeData) {
	}

	GMetaVariant(const GVariant & v, const GMetaType & type) : value(v), type(type) {
	}

	GMetaVariant(const GMetaVariant & other) : value(other.value), type(other.type) {
	}

	GMetaVariant & operator = (const GMetaVariant & other) {
		this->value = other.value;
		this->type = other.type;

		return *this;
	}

	const GVariant & getValue() const {
		return this->value;
	}

	const GMetaType & getType() const {
		return this->type;
	}

	GMetaVarData getData() const {
		GMetaVarData data;

		data.varData = this->value.getData();
		data.typeData = this->type.getData();

		return data;
	}

	GMetaVarData takeData() {
		GMetaVarData data;

		data.varData = this->value.takeData();
		data.typeData = this->type.getData();

		return data;
	}

private:
	GMetaVariant(const GVariant &);

private:
	GVariant value;
	GMetaType type;
};



} // namespace cpgf


#endif
