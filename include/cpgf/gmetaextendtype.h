#ifndef CPGF_GMETAEXTENDTYPE_H
#define CPGF_GMETAEXTENDTYPE_H

#include "cpgf/gclassutil.h"
#include "cpgf/gifelse.h"
#include "cpgf/gtypetraits.h"
#include "cpgf/gmetamodule.h"

#include "cpgf/metatraits/gmetatraitsparam.h"
#include "cpgf/metatraits/gmetaconverter.h"
#include "cpgf/metatraits/gmetaserializer.h"
#include "cpgf/metatraits/gmetascriptwrapper.h"
#include "cpgf/metatraits/gmetasharedptrtraits.h"
#include "cpgf/metatraits/gmetaobjectlifemanager.h"

#include <cstdint>
#include <string.h>

namespace cpgf {

class GMetaItem;

const uint32_t GExtendTypeCreateFlag_Converter = 1 << 0;
const uint32_t GExtendTypeCreateFlag_Serializer = 1 << 1;
const uint32_t GExtendTypeCreateFlag_ScriptWrapper = 1 << 2;
const uint32_t GExtendTypeCreateFlag_SharedPointerTraits = 1 << 3;
const uint32_t GExtendTypeCreateFlag_ObjectLifeManager = 1 << 4;

#pragma pack(push, 1)
#pragma pack(1)
struct GMetaExtendTypeData
{
	uint32_t arraySize;
	IMetaConverter * converter;
	IMetaSerializer * serializer;
	IMetaScriptWrapper * scriptWrapper;
	IMetaSharedPointerTraits * sharedPointerTraits;
	IMetaObjectLifeManager * objectLifeManager;
};
#pragma pack(pop)


namespace meta_internal {

template <typename T>
struct WrapRawExtendType
{
private:
	typedef typename ExtractRawType<T>::Result Raw;

public:
	typedef typename GIfElse<IsVoid<Raw>::Result, int, Raw>::Result Result;
};

template <typename T>
struct WrapExtendType
{
private:
	typedef typename RemoveReference<T>::Result U;

public:
	typedef typename GIfElse<IsVoid<U>::Result, int, U>::Result Result;
};

template <typename T>
void deduceMetaExtendTypeData(GMetaExtendTypeData * data, uint32_t createFlags, const GMetaModule * module)
{
	data->arraySize = ArraySize<T>::Result;
	
	if((createFlags & GExtendTypeCreateFlag_Converter) != 0) {
		GMetaTraitsParam param;
		param.module = module;
		typename WrapExtendType<T>::Result * p = 0;
		data->converter = createConverterFromMetaTraits(param, p);
	}
	else {
		data->converter = nullptr;
	}

	if((createFlags & GExtendTypeCreateFlag_Serializer) != 0) {
		GMetaTraitsParam param;
		param.module = module;
		typename WrapRawExtendType<T>::Result * p = 0;
		data->serializer = createSerializerFromMetaTraits(param, p);
	}
	else {
		data->serializer = nullptr;
	}

	if((createFlags & GExtendTypeCreateFlag_ScriptWrapper) != 0) {
		GMetaTraitsParam param;
		param.module = module;
		typename WrapExtendType<T>::Result * p = 0;
		data->scriptWrapper = metaTraitsCreateScriptWrapper<typename WrapExtendType<T>::Result>(param, p);
	}
	else {
		data->scriptWrapper = nullptr;
	}

	if((createFlags & GExtendTypeCreateFlag_SharedPointerTraits) != 0) {
		GMetaTraitsParam param;
		param.module = module;
		typename WrapExtendType<T>::Result * p = 0;
		data->sharedPointerTraits = createSharedPointerTraitsFromMetaTraits(param, p);
	}
	else {
		data->sharedPointerTraits = nullptr;
	}

	if((createFlags & GExtendTypeCreateFlag_ObjectLifeManager) != 0) {
		GMetaTraitsParam param;
		param.module = module;
		typename WrapRawExtendType<T>::Result * p = 0;
		data->objectLifeManager = createObjectLifeManagerFromMetaTraits(param, p);
	}
	else {
		data->objectLifeManager = nullptr;
	}
}


} // namespace meta_internal


GMAKE_FINAL(GMetaExtendType)

class GMetaExtendType : GFINAL_BASE(GMetaExtendType)
{
public:
	GMetaExtendType();
	explicit GMetaExtendType(const GMetaExtendTypeData & data);
	GMetaExtendType(const GMetaExtendType & other);
	~GMetaExtendType();

	GMetaExtendType & operator = (GMetaExtendType other);
	
	void swap(GMetaExtendType & other);

	uint32_t getArraySize() const;
	IMetaConverter * getConverter() const;
	IMetaSerializer * getSerializer() const;
	IMetaScriptWrapper * getScriptWrapper() const;
	IMetaSharedPointerTraits * getSharedPointerTraits() const;
	IMetaObjectLifeManager * getObjectLifeManager() const;
	
	const GMetaExtendTypeData & refData() const;
	GMetaExtendTypeData & refData();
	GMetaExtendTypeData takeData();

private:
	GMetaExtendTypeData data;
};

template <typename T>
GMetaExtendType createMetaExtendType(uint32_t createFlags, const GMetaItem * metaItem)
{
	GMetaExtendType type;

	meta_internal::deduceMetaExtendTypeData<T>(&type.refData(), createFlags, getItemModule(metaItem));

	return type;
}

template <typename T>
GMetaExtendType createMetaExtendType(uint32_t createFlags, const GMetaModule * module)
{
	GMetaExtendType type;

	meta_internal::deduceMetaExtendTypeData<T>(&type.refData(), createFlags, module);

	return type;
}

template <typename T>
GMetaExtendType createMetaExtendType(uint32_t createFlags)
{
	return createMetaExtendType<T>(createFlags, (const GMetaItem *)nullptr);
}

template <typename T>
GMetaExtendType createMetaExtendType()
{
	return createMetaExtendType<T>(0);
}

void retainExtendTypeData(GMetaExtendTypeData * data);
void releaseExtendTypeData(GMetaExtendTypeData * data);


} // namespace cpgf



#endif
