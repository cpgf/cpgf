#include "cpgf/gmetatype.h"
#include "cpgf/gmetacommon.h"
#include "cpgf/gassert.h"

#include "pinclude/gmetatypereg.h"

#include <vector>
#include <typeinfo>

#include <string.h>


namespace cpgf {



bool GMetaType::operator == (const GMetaType & other) const
{
	if(this->getPointerDimension() != other.getPointerDimension()) {
		return false;
	}

	if(this->isPointer() && other.isPointer()) {
		if(
			this->isPointerToConstVolatile() != other.isPointerToConstVolatile()
			|| this->isPointerToConst() != other.isPointerToConst()
			|| this->isPointerToVolatile() != other.isPointerToVolatile()
		) {
			return false;
		}
	}

	if(this->baseName != NULL && other.baseName != NULL) {
		return strcmp(this->baseName, other.baseName) == 0;
	}

	if(!this->baseType.isEmpty() && !other.baseType.isEmpty()) {
		return this->baseType == other.baseType;
	}

	return false;
}

GMetaTypeData GMetaType::getData() const
{
	GMetaTypeData data;
	
	data.baseName = this->baseName;
	data.flags = static_cast<uint32_t>(this->flags);
	data.varType = static_cast<uint16_t>(this->varType);
	
	return data;
}

void GMetaType::addPointer()
{
	++this->varPointers;
	this->varType |= byPointer;
}

void fixupMetaType(GMetaType * type)
{
	if(type->baseName == NULL) {
		const GMetaTypedItem * item = meta_internal::findRegisteredMetaType(type->getBaseType());
		if(item != NULL) {
			type->baseName = item->getTypeName().c_str();
		}
	}
}



} // namespace cpgf
