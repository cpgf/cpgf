#ifndef __GMETAEXTENDTYPE_H
#define __GMETAEXTENDTYPE_H

#include "cpgf/gvariant.h"
#include "cpgf/gstdint.h"
#include "cpgf/gassert.h"
#include "cpgf/gclassutil.h"


#include <typeinfo>

#include <string.h>


namespace cpgf {

#pragma pack(push, 1)
#pragma pack(1)
struct GMetaExtendTypeData
{
	uint32_t arraySize;
};
#pragma pack(pop)


namespace meta_internal {

template <typename T, typename Enable = void>
struct ArraySize
{
	enum { Result = 0 };
};

template <typename T>
struct ArraySize <T, typename GEnableIfResult<IsArray<T> >::Result>
{
private:
	static T arrayData;

public:
	enum { Result = (sizeof(arrayData) / sizeof(arrayData[0])) };
};

template <typename T>
void deduceMetaExtendTypeData(GMetaExtendTypeData * data)
{
	data->arraySize = ArraySize<T>::Result;
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

public:
	GMetaExtendTypeData data;
};

template <typename T>
GMetaExtendType createMetaExtendType(uint32_t createFlags)
{
	(void)createFlags;

	GMetaExtendType type;

	meta_internal::deduceMetaExtendTypeData<T>(&type.data);

	return type;
}

template <typename T>
GMetaExtendType createMetaExtendType()
{
	return createMetaExtendType<T>(0);
}



} // namespace cpgf



#endif
