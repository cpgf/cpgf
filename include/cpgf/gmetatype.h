#ifndef CPGF_GMETATYPE_H
#define CPGF_GMETATYPE_H

#include "cpgf/gvariant.h"
#include "cpgf/gassert.h"
#include "cpgf/gclassutil.h"


#include <typeinfo>

#include <cstdint>
#include <string.h>

namespace cpgf {

class GMetaItem;
class GMetaModule;

#pragma pack(push, 1)
#pragma pack(1)
struct GMetaTypeData
{
	GVarTypeData typeData;
	uint32_t flags;
	const char * baseName;
};
#pragma pack(pop)


GMAKE_FINAL(GTypeInfo)

class GTypeInfo : GFINAL_BASE(GTypeInfo)
{
public:
	GTypeInfo() : typeInfo(nullptr) {
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
		return this->typeInfo == nullptr;
	}

	const std::type_info & getStdTypeInfo() const {
		return *(this->typeInfo);
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


GMAKE_FINAL(GMetaType)

class GMetaType : GFINAL_BASE(GMetaType)
{
//	GASSERT_STATIC(sizeof(GMetaTypeData) == 8);

public:
	GMetaType();
	explicit GMetaType(const GMetaTypeData & data);
	GMetaType(const GMetaTypeData & data, const GTypeInfo & baseType);
	GMetaType(const GMetaType & other);
	~GMetaType();

	GMetaType & operator = (const GMetaType & other);
	
	bool operator == (const GMetaType & other) const;
	bool operator != (const GMetaType & other) const;

	bool isEmpty() const;

	const char * getBaseName() const;
	const GTypeInfo & getBaseType() const;

	bool baseIsClass() const;
	bool baseIsArray() const;
	
	bool baseIsStdString() const;
	bool baseIsStdWideString() const;
	
	bool isFundamental() const;
	
	bool isFunction() const;
	bool isConstFunction() const;
	bool isVolatileFunction() const;
	bool isConstVolatileFunction() const;

	bool isConst() const;
	bool isVolatile() const;
	bool isConstVolatile() const;

	bool isPointerToConst() const;
	bool isPointerToVolatile() const;
	bool isPointerToConstVolatile() const;

	bool isReferenceToConst() const;
	bool isReferenceToVolatile() const;
	bool isReferenceToConstVolatile() const;

	bool isPointer() const;

	bool isReference() const;
	
	bool isString() const;
	bool isWideString() const;

	unsigned int getPointerDimension() const;
	
	GVariantType getVariantType() const;
	size_t getVariantSize() const;

	const GMetaTypeData & refData() const;
	GMetaTypeData & refData();

	void addPointer();
	void addPointerToConst();
	void addPointerToVolatile();
	void addPointerToConstVolatile();
	void addConst();
	void removeReference();

private:
	bool hasFlag(unsigned int flag) const;

private:
	GMetaTypeData data;
	GTypeInfo baseType;
};

template <typename T>
GMetaType createMetaType()
{
	GMetaTypeData data = GMetaTypeData();
	meta_internal::deduceMetaTypeData<T>(&data);
	return GMetaType(data, meta_internal::GMetaTypeDeduce<T>::getBaseType());
}

GMetaType createMetaTypeWithName(const GMetaType & type, const char * name);

void initializeMetaType(GMetaTypeData * data);

void fixupMetaType(GMetaType * type);
void fixupMetaType(GMetaType * type, const GMetaItem * metaItem);
void fixupMetaType(GMetaType * type, const GMetaModule * module);


} // namespace cpgf



#endif
