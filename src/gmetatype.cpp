#include "cpgf/gmetatype.h"
#include "cpgf/gmetacommon.h"
#include "cpgf/gassert.h"

#include "pinclude/gmetatypereg.h"

#include <vector>
#include <typeinfo>

#include <string.h>


namespace cpgf {


GMetaType::GMetaType()
	: baseName(NULL), flags(0), baseType()
{
	vtInit(typeData);
}

GMetaType::GMetaType(const GMetaTypeData & data)
	: baseName(data.baseName), flags(data.flags), typeData(data.typeData), baseType()
{
}

GMetaType::GMetaType(const GMetaTypeData & data, const GTypeInfo & baseType)
	: baseName(data.baseName), flags(data.flags), typeData(data.typeData), baseType(baseType)
{
}

GMetaType::GMetaType(const GMetaType & other)
	: baseName(other.baseName), flags(other.flags), typeData(other.typeData), baseType(other.baseType)
{
}

GMetaType::~GMetaType()
{
}

GMetaType & GMetaType::operator = (const GMetaType & other)
{
	if(this != &other) {
		this->baseName = other.baseName;
		this->flags = other.flags;
		this->typeData = other.typeData;
		this->baseType = other.baseType;
	}

	return *this;
}

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

bool GMetaType::operator != (const GMetaType & other) const
{
	return ! this->operator == (other);
}

bool GMetaType::isEmpty() const
{
	return vtIsEmpty(vtGetType(this->typeData));
}

const char * GMetaType::getBaseName() const
{
	return this->baseName;
}

const GTypeInfo & GMetaType::getBaseType() const
{
	return this->baseType;
}

bool GMetaType::baseIsClass() const
{
	return this->hasFlag(meta_internal::mtFlagBaseIsClass);
}

bool GMetaType::isFundamental() const
{
	return vtIsFundamental(this->getVariantType());
}

bool GMetaType::baseIsArray() const
{
	return this->hasFlag(meta_internal::mtFlagBaseIsArray);
}

bool GMetaType::isFunction() const
{
	return this->hasFlag(meta_internal::mtFlagIsFunction);
}

bool GMetaType::isConstFunction() const
{
	return this->hasFlag(meta_internal::mtFlagIsConstFunction);
}

bool GMetaType::isVolatileFunction() const
{
	return this->hasFlag(meta_internal::mtFlagIsVolatileFunction);
}

bool GMetaType::isConstVolatileFunction() const
{
	return this->hasFlag(meta_internal::mtFlagIsConstVolatileFunction);
}

bool GMetaType::isConst() const
{
	return this->hasFlag(meta_internal::mtFlagIsConst)
		&& !this->isConstVolatile();
}

bool GMetaType::isVolatile() const
{
	return this->hasFlag(meta_internal::mtFlagIsVolatile)
		&& !this->isConstVolatile();
}

bool GMetaType::isConstVolatile() const
{
	return this->hasFlag(meta_internal::mtFlagIsConstVolatile);
}

bool GMetaType::isPointerToConst() const
{
	return this->hasFlag(meta_internal::mtFlagIsPointer | meta_internal::mtFlagIsPointerToConst)
		&& !this->isPointerToConstVolatile();
}

bool GMetaType::isPointerToVolatile() const
{
	return this->hasFlag(meta_internal::mtFlagIsPointer | meta_internal::mtFlagIsPointerToVolatile)
		&& !this->isPointerToConstVolatile();
}

bool GMetaType::isPointerToConstVolatile() const
{
	return this->hasFlag(meta_internal::mtFlagIsPointer | meta_internal::mtFlagIsPointerToConstVolatile);
}

bool GMetaType::isPointer() const
{
	return this->hasFlag(meta_internal::mtFlagIsPointer);
}

bool GMetaType::isReference() const
{
	return this->hasFlag(meta_internal::mtFlagIsReference);
}

unsigned int GMetaType::getPointerDimension() const
{
	return vtGetPointers(this->typeData);
}

GVariantType GMetaType::getVariantType() const
{
	return vtGetType(this->typeData);
}

size_t GMetaType::getVariantSize() const
{
	return vtGetSize(this->typeData);
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
