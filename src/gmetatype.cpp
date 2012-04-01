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
	data.typeData = this->typeData;
	
	return data;
}

void GMetaType::addPointer()
{
	vtSetPointers(this->typeData, vtGetPointers(this->typeData) + 1);
	vtSetType(this->typeData, vtGetType(this->typeData) | byPointer);
	this->flags |= meta_internal::mtFlagIsPointer;
}

void GMetaType::addPointerToConst()
{
	this->addPointer();
	this->flags |= meta_internal::mtFlagIsPointerToConst;
}

void GMetaType::addPointerToVolatile()
{
	this->addPointer();
	this->flags |= meta_internal::mtFlagIsPointerToVolatile;
}

void GMetaType::addPointerToConstVolatile()
{
	this->addPointer();
	this->flags |= meta_internal::mtFlagIsPointerToConstVolatile;
}

void GMetaType::addConst()
{
	this->flags |= meta_internal::mtFlagIsConst;
}

void GMetaType::removeReference()
{
	this->flags &= ~meta_internal::mtFlagIsReference;
	vtSetType(this->typeData, vtGetType(this->typeData) & ~byReference);
}

GMetaType createMetaTypeWithName(const GMetaType & type, const char * name)
{
	GMetaTypeData data = type.getData();
	data.baseName = name;
	return GMetaType(data, type.getBaseType());
}

void initializeMetaType(GMetaTypeData * data)
{
	vtInit(data->typeData);
	data->flags = 0;
	data->baseName = NULL;
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
