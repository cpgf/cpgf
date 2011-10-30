#ifndef __GMETATYPE_H
#define __GMETATYPE_H

#include "cpgf/gvariant.h"
#include "cpgf/gstdint.h"
#include "cpgf/gassert.h"


#include <typeinfo>

#include <string.h>


namespace cpgf {


#pragma pack(push, 1)
#pragma pack(1)
struct GMetaTypeData
{
	const char * baseName;
	uint32_t flags;
	GVarTypeData typeData;
};
#pragma pack(pop)


class GTypeInfo
{
public:
	GTypeInfo() : typeInfo(NULL) {
	}

	explicit GTypeInfo(const std::type_info & typeInfo) : typeInfo(&typeInfo) {
	}

	GTypeInfo(const GTypeInfo & other) : typeInfo(other.typeInfo) {
	}

	GTypeInfo & operator = (const GTypeInfo & other) {
		this->typeInfo = other.typeInfo;

		return *this;
	}

	bool operator == (const GTypeInfo & other) const {
		return *this->typeInfo == *other.typeInfo;
	}

	bool operator != (const GTypeInfo & other) const {
		return *this->typeInfo != *other.typeInfo;
	}

	bool operator < (const GTypeInfo & other) const {
		return !! this->typeInfo->before(*other.typeInfo);
	}

	bool isEmpty() const {
		return this->typeInfo == NULL;
	}

private:
	const std::type_info * typeInfo;
};

template <typename T>
GTypeInfo gTypeId()
{
	return GTypeInfo(typeid(T));
}


#include "cpgf/private/gmetatype_p.h"


class GMetaType
{
//	GASSERT_STATIC(sizeof(GMetaTypeData) == 8);

public:
	GMetaType() : baseName(NULL), flags(0), baseType() {
		vtInit(typeData);
	}

	explicit GMetaType(const GMetaTypeData & data)
		: baseName(data.baseName), flags(data.flags), typeData(data.typeData), baseType() {
	}

	GMetaType(const GMetaTypeData & data, const GTypeInfo & baseType)
		: baseName(data.baseName), flags(data.flags), typeData(data.typeData), baseType(baseType) {
	}

	GMetaType(const GMetaType & other)
		: baseName(other.baseName), flags(other.flags), typeData(other.typeData), baseType(other.baseType) {
	}

	~GMetaType() {
	}

	GMetaType & operator = (const GMetaType & other) {
		if(this != &other) {
			this->baseName = other.baseName;
			this->flags = other.flags;
			this->typeData = other.typeData;
			this->baseType = other.baseType;
		}

		return *this;
	}

	bool operator == (const GMetaType & other) const;

	bool operator != (const GMetaType & other) const {
		return ! this->operator == (other);
	}

	bool isEmpty() const {
		return vtIsEmpty(vtGetType(this->typeData));
	}

	const char * getBaseName() const {
		return this->baseName;
	}

	const GTypeInfo & getBaseType() const {
		return this->baseType;
	}

	bool baseIsClass() const {
		return this->hasFlag(meta_internal::mtFlagBaseIsClass);
	}
	
	bool isFundamental() const {
		return vtIsFundamental(this->getVariantType());
	}

	bool isFunction() const {
		return this->hasFlag(meta_internal::mtFlagIsFunction);
	}

	bool isConstFunction() const {
		return this->hasFlag(meta_internal::mtFlagIsConstFunction);
	}

	bool isVolatileFunction() const {
		return this->hasFlag(meta_internal::mtFlagIsVolatileFunction);
	}

	bool isConstVolatileFunction() const {
		return this->hasFlag(meta_internal::mtFlagIsConstVolatileFunction);
	}

	bool isConst() const {
		return this->hasFlag(meta_internal::mtFlagIsConst)
			&& !this->isConstVolatile();
	}

	bool isVolatile() const {
		return this->hasFlag(meta_internal::mtFlagIsVolatile)
			&& !this->isConstVolatile();
	}

	bool isConstVolatile() const {
		return this->hasFlag(meta_internal::mtFlagIsConstVolatile);
	}

	bool isPointerToConst() const {
		return this->hasFlag(meta_internal::mtFlagIsPointer | meta_internal::mtFlagIsPointerToConst)
			&& !this->isPointerToConstVolatile();
	}

	bool isPointerToVolatile() const {
		return this->hasFlag(meta_internal::mtFlagIsPointer | meta_internal::mtFlagIsPointerToVolatile)
			&& !this->isPointerToConstVolatile();
	}

	bool isPointerToConstVolatile() const {
		return this->hasFlag(meta_internal::mtFlagIsPointer | meta_internal::mtFlagIsPointerToConstVolatile);
	}

	bool isPointer() const {
		return this->hasFlag(meta_internal::mtFlagIsPointer);
	}

	bool isReference() const {
		return this->hasFlag(meta_internal::mtFlagIsReference);
	}

	unsigned int getPointerDimension() const {
		return vtGetPointers(this->typeData);
	}

	GVariantType getVariantType() const {
		return vtGetType(this->typeData);
	}

	GMetaTypeData getData() const;

	void addPointer();

private:
	bool hasFlag(unsigned int flag) const {
		return (this->flags & flag) == flag;
	}

private:
	const char * baseName;
	int flags;
	GVarTypeData typeData;
	GTypeInfo baseType;

private:
	friend void fixupMetaType(GMetaType * type);
};

template <typename T>
GMetaType createMetaType()
{
	GMetaTypeData data;
	meta_internal::deduceMetaTypeData<T>(&data);
	return GMetaType(data, meta_internal::GMetaTypeDeduce<T>::getBaseType());
}

GMetaType createMetaTypeWithName(const GMetaType & type, const char * name);

void initializeMetaType(GMetaTypeData * data);

void fixupMetaType(GMetaType * type);


} // namespace cpgf



#endif
