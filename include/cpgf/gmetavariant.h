#ifndef __GMETAVARIANT_H
#define __GMETAVARIANT_H

#include "cpgf/gvariant.h"
#include "cpgf/gmetatype.h"

namespace cpgf {

class GMetaVariant
{
public:
	template <typename T>
	GMetaVariant(const T & v) : value(v), type(createMetaType<T>()) {
		fixupMetaType(&this->type);
	}

	GMetaVariant(const GVariant & v, const GMetaType & type) : value(v), type(type) {
	}

	GMetaVariant(const GMetaVariant & other) : value(other.value), type(other.type) {
	}

	GMetaVariant & operator = (const GMetaVariant & other) {
		this->value = other.value;
		this->type = other.type;
	}

private:
	GVariant value;
	GMetaType type;
};

} // namespace cpgf


#endif
