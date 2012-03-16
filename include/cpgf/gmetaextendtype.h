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
	union {
		uint32_t arraySize;
		uint32_t createFlags;
	};
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
	GMetaExtendType()
	{
		this->data.arraySize = 0;
	}

	explicit GMetaExtendType(const GMetaExtendTypeData & data)
		: data(data)
	{
	}

	GMetaExtendType(const GMetaExtendType & other)
		: data(other.data)
	{
	}

	GMetaExtendType & operator = (const GMetaExtendType & other) {
		this->data = other.data;
	}

public:
	GMetaExtendTypeData data;
};

template <typename T>
GMetaExtendType createMetaExtendType(uint32_t createFlags)
{
	GMetaExtendType type;

	meta_internal::deduceMetaExtendTypeData<T>(&type.data);

	return type;
}

template <typename T>
GMetaExtendType createMetaExtendType()
{
	return createMetaExtendType<T>(0xffffffff);
}

template <typename T>
void createMetaExtendType(GMetaExtendType * outExtendType)
{
	if(outExtendType != NULL) {
		*outExtendType = createMetaExtendType<T>(outExtendType->data.createFlags);
	}
}



} // namespace cpgf



#endif
