#include "cpgf/gmetatype.h"
#include "cpgf/gmetacommon.h"
#include "cpgf/gassert.h"

#include "cpgf/gmetaclass.h"
#include "cpgf/gmetamodule.h"

#include <vector>
#include <typeinfo>

#include <string.h>


namespace cpgf {


GMetaType::GMetaType()
{
	vtInit(data.typeData);
	data.baseName = nullptr;
	data.flags = 0;
}

GMetaType::GMetaType(const GMetaTypeData & typeData)
	: data(typeData), baseType()
{
}

GMetaType::GMetaType(const GMetaTypeData & typeData, const GTypeInfo & baseType)
	: data(typeData), baseType(baseType)
{
}

GMetaType::GMetaType(const GMetaType & other)
	: data(other.data), baseType(other.baseType)
{
}

GMetaType::~GMetaType()
{
}

GMetaType & GMetaType::operator = (const GMetaType & other)
{
	if(this != &other) {
		this->data = other.data;
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

	if(this->data.baseName != nullptr && other.data.baseName != nullptr) {
		return strcmp(this->data.baseName, other.data.baseName) == 0;
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
	return vtIsEmpty(vtGetType(this->data.typeData));
}

bool GMetaType::hasFlag(unsigned int flag) const
{
	return (this->data.flags & flag) == flag;
}

const char * GMetaType::getBaseName() const
{
	return this->data.baseName;
}

const GTypeInfo & GMetaType::getBaseType() const
{
	return this->baseType;
}

bool GMetaType::baseIsClass() const
{
	return this->hasFlag(meta_internal::mtFlagBaseIsClass);
}

bool GMetaType::baseIsArray() const
{
	return this->hasFlag(meta_internal::mtFlagBaseIsArray);
}

bool GMetaType::baseIsStdString() const
{
	return this->hasFlag(meta_internal::mtFlagBaseIsStdString);
}

bool GMetaType::baseIsStdWideString() const
{
	return this->hasFlag(meta_internal::mtFlagBaseIsStdWideString);
}

bool GMetaType::isFundamental() const
{
	return vtIsFundamental(this->getVariantType());
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

bool GMetaType::isReferenceToConst() const
{
	return this->hasFlag(meta_internal::mtFlagIsReference | meta_internal::mtFlagIsReferenceToConst)
		&& !this->isReferenceToConstVolatile();
}

bool GMetaType::isReferenceToVolatile() const
{
	return this->hasFlag(meta_internal::mtFlagIsReference | meta_internal::mtFlagIsReferenceToVolatile)
		&& !this->isReferenceToConstVolatile();
}

bool GMetaType::isReferenceToConstVolatile() const
{
	return this->hasFlag(meta_internal::mtFlagIsReference | meta_internal::mtFlagIsReferenceToConstVolatile);
}

bool GMetaType::isPointer() const
{
	return this->hasFlag(meta_internal::mtFlagIsPointer);
}

bool GMetaType::isReference() const
{
	return this->hasFlag(meta_internal::mtFlagIsReference);
}

bool GMetaType::isString() const
{
	return (this->getVariantType() == GVariantType::vtString)
		|| (vtGetBaseType(this->getVariantType()) == GVariantType::vtChar && this->getPointerDimension() == 1)
	;
}

bool GMetaType::isWideString() const
{
	return (this->getVariantType() == GVariantType::vtWideString)
		|| (vtGetBaseType(this->getVariantType()) == GVariantType::vtWchar && this->getPointerDimension() == 1)
	;
}

unsigned int GMetaType::getPointerDimension() const
{
	return vtGetPointers(this->data.typeData);
}

GVariantType GMetaType::getVariantType() const
{
	return vtGetType(this->data.typeData);
}

size_t GMetaType::getVariantSize() const
{
	return vtGetSize(this->data.typeData);
}

const GMetaTypeData & GMetaType::refData() const
{
	return this->data;
}

GMetaTypeData & GMetaType::refData()
{
	return this->data;
}

void GMetaType::addPointer()
{
	vtSetPointers(this->data.typeData, vtGetPointers(this->data.typeData) + 1);
	vtSetType(this->data.typeData, vtGetType(this->data.typeData) | GVariantType::byPointer);
	this->data.flags |= meta_internal::mtFlagIsPointer;
}

void GMetaType::addPointerToConst()
{
	this->addPointer();
	this->data.flags |= meta_internal::mtFlagIsPointerToConst;
}

void GMetaType::addPointerToVolatile()
{
	this->addPointer();
	this->data.flags |= meta_internal::mtFlagIsPointerToVolatile;
}

void GMetaType::addPointerToConstVolatile()
{
	this->addPointer();
	this->data.flags |= meta_internal::mtFlagIsPointerToConstVolatile;
}

void GMetaType::addConst()
{
	this->data.flags |= meta_internal::mtFlagIsConst;
}

void GMetaType::removeReference()
{
	this->data.flags &= ~meta_internal::mtFlagIsReference;
	vtSetType(this->data.typeData, vtGetType(this->data.typeData) & ~GVariantType::maskByReference);
}

GMetaType createMetaTypeWithName(const GMetaType & type, const char * name)
{
	GMetaTypeData newData = type.refData();
	newData.baseName = name;
	return GMetaType(newData, type.getBaseType());
}

void initializeMetaType(GMetaTypeData * data)
{
	vtInit(data->typeData);
	data->flags = 0;
	data->baseName = nullptr;
}

void fixupMetaType(GMetaType * type)
{
	if(type->refData().baseName == nullptr && ! type->getBaseType().isEmpty()) {
		const GMetaTypedItem * item = getGlobalMetaClass()->getModule()->findItemByType(type->getBaseType());
		if(item != nullptr) {
			type->refData().baseName = item->getQualifiedName().c_str();
		}
	}
}

void fixupMetaType(GMetaType * type, const GMetaItem * metaItem)
{
	if(type->refData().baseName == nullptr) {
		return fixupMetaType(type, getItemModule(metaItem));
	}
}

void fixupMetaType(GMetaType * type, const GMetaModule * module)
{
	if(type->refData().baseName == nullptr && ! type->getBaseType().isEmpty()) {
		if(module == nullptr) {
			module = getGlobalMetaClass()->getModule();
		}
		const GMetaTypedItem * item = module->findItemByType(type->getBaseType());
		if(item != nullptr) {
			type->refData().baseName = item->getQualifiedName().c_str();
		}
	}
}

} // namespace cpgf
