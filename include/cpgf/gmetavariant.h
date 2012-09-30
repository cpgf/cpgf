#ifndef __GMETAVARIANT_H
#define __GMETAVARIANT_H

#include "cpgf/gvariant.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gclassutil.h"

namespace cpgf {

#pragma pack(push, 1)
#pragma pack(1)
struct GMetaVarData
{
	GVariantData varData;
	GMetaTypeData typeData;
};
#pragma pack(pop)


GMAKE_FINAL(GMetaVariant)

class GMetaVariant : GFINAL_BASE(GMetaVariant)
{
public:
	GMetaVariant() {
	}

	template <typename T>
	GMetaVariant(const T & v) : typedValue(createTypedVariant(v, createMetaType<T>())) {
	}

	GMetaVariant(GMetaVarData data) : typedValue(createTypedVariant(GVariant(data.varData), GMetaType(data.typeData))) {
	}

	GMetaVariant(const GVariant & v, const GMetaType & type) : typedValue(createTypedVariant(v, type)) {
	}

	GMetaVariant(const GMetaVariant & other) : typedValue(other.typedValue) {
	}

	GMetaVariant & operator = (const GMetaVariant & other) {
		this->typedValue = other.typedValue;

		return *this;
	}

	GVariant getValue() const {
		return getVariantRealValue(this->typedValue);
	}

	GMetaType getType() const {
		return getVariantRealType(this->typedValue);
	}

	GMetaVarData getData() const {
		GMetaVarData data;

		data.varData = this->getValue().getData();
		data.typeData = this->getType().getData();

		return data;
	}

	GMetaVarData takeData() {
		GMetaVarData data;

		data.varData = this->getValue().takeData();
		data.typeData = this->getType().getData();

		return data;
	}

private:
	GMetaVariant(const GVariant &);

private:
	GVariant typedValue;
};



} // namespace cpgf


#endif
