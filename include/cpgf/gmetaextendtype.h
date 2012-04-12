#ifndef __GMETAEXTENDTYPE_H
#define __GMETAEXTENDTYPE_H

#include "cpgf/gstdint.h"
#include "cpgf/gclassutil.h"
#include "cpgf/gifelse.h"
#include "cpgf/gtypetraits.h"
#include "cpgf/gmetamodule.h"

#include "cpgf/metatraits/gmetatraitsparam.h"
#include "cpgf/metatraits/gmetaconverter.h"
#include "cpgf/metatraits/gmetaserializer.h"

#include <string.h>


namespace cpgf {

class GMetaItem;

const uint32_t GExtendTypeCreateFlag_Converter = 1 << 0;
const uint32_t GExtendTypeCreateFlag_Serializer = 1 << 1;

#pragma pack(push, 1)
#pragma pack(1)
struct GMetaExtendTypeData
{
	uint32_t arraySize;
	IMetaConverter * converter;
	IMetaSerializer * serializer;
};
#pragma pack(pop)


namespace meta_internal {

template <typename T>
struct WrapExtendType
{
private:
	typedef typename ExtractRawType<T>::Result Raw;

public:
	typedef
		typename GIfElse<IsVoid<Raw>::Result, int, Raw>::Result
		Result;
};

template <typename T>
void deduceMetaExtendTypeData(GMetaExtendTypeData * data, uint32_t createFlags, const GMetaModule * module)
{
	using namespace cpgf;

	data->arraySize = ArraySize<T>::Result;
	
	if((createFlags & GExtendTypeCreateFlag_Converter) != 0) {
		GMetaTraitsParam param;
		param.module = module;
		typename WrapExtendType<T>::Result * p = 0;
		data->converter = metaTraitsCreateConverter(*p, param);
	}
	else {
		data->converter = NULL;
	}
	
	if((createFlags & GExtendTypeCreateFlag_Serializer) != 0) {
		GMetaTraitsParam param;
		param.module = module;
		typename WrapExtendType<T>::Result * p = 0;
		data->serializer = metaTraitsCreateSerializer(*p, param);
	}
	else {
		data->serializer = NULL;
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
	
	GMetaExtendTypeData takeData();

private:
	void doRetainInterfaces();
	void doReleaseInterfaces();

private:
	GMetaExtendTypeData data;
};

template <typename T>
GMetaExtendType createMetaExtendType(uint32_t createFlags, const GMetaItem * metaItem)
{
	GMetaExtendTypeData typeData;

	meta_internal::deduceMetaExtendTypeData<T>(&typeData, createFlags, getItemModule(metaItem));

	return GMetaExtendType(typeData);
}

template <typename T>
GMetaExtendType createMetaExtendType(uint32_t createFlags, const GMetaModule * module)
{
	GMetaExtendTypeData typeData;

	meta_internal::deduceMetaExtendTypeData<T>(&typeData, createFlags, module);

	return GMetaExtendType(typeData);
}

template <typename T>
GMetaExtendType createMetaExtendType(uint32_t createFlags)
{
	return createMetaExtendType<T>(createFlags, (const GMetaItem *)NULL);
}

template <typename T>
GMetaExtendType createMetaExtendType()
{
	return createMetaExtendType<T>(0);
}



} // namespace cpgf



#endif
