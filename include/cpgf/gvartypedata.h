#ifndef __GVARTYPEDATA_H
#define __GVARTYPEDATA_H

#include "cpgf/gcompiler.h"
#include "cpgf/gstdint.h"
#include "cpgf/gassert.h"
#include "cpgf/gtypetraits.h"
#include "cpgf/gtypelist.h"
#include "cpgf/genableif.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
#endif


namespace cpgf {


enum GVariantType {
	vtEmpty = 0,
	vtVoid = 1,

	vtFundamentalBegin = 2,
	vtIntegerBegin = vtFundamentalBegin,
	vtBool = vtFundamentalBegin,
	vtChar = vtFundamentalBegin + 1, vtWchar = vtFundamentalBegin + 2,
	vtSignedChar = vtFundamentalBegin + 3, vtUnsignedChar = vtFundamentalBegin + 4,
	vtSignedShort = vtFundamentalBegin + 5, vtUnsignedShort = vtFundamentalBegin + 6,
	vtSignedInt = vtFundamentalBegin + 7, vtUnsignedInt = vtFundamentalBegin + 8,
	vtSignedLong = vtFundamentalBegin + 9, vtUnsignedLong = vtFundamentalBegin + 10,
	vtSignedLongLong = vtFundamentalBegin + 11, vtUnsignedLongLong = vtFundamentalBegin + 12,
	vtIntegerEnd = vtUnsignedLongLong,
	vtFloat = vtFundamentalBegin + 13, vtDouble = vtFundamentalBegin + 14, vtLongDouble = vtFundamentalBegin + 15,
	vtFundamentalEnd = vtLongDouble,

	vtPointer = 30,
	vtObject = 31, // is equivalent to unkown type
	vtShadow = 32,
	vtString = 33,

	vtUserBegin = 0xff,
	vtUserEnd = 0x0fff,

	vtMask = 0x0fff,

	byPointer = 0x1000,
	byReference = 0x2000,
};

inline GVariantType operator + (GVariantType vt, int n)
{
	return static_cast<GVariantType>(static_cast<int>(vt) + n);
}

#pragma pack(push, 1)
#pragma pack(1)
struct GVarTypeData
{
	uint16_t vt;
	uint8_t sizeAndPointers;
	uint8_t padding;
};
#pragma pack(pop)


inline GVariantType vtGetBaseType(const GVarTypeData & data) {
	return static_cast<GVariantType>(data.vt & vtMask);
}

inline GVariantType vtGetType(const GVarTypeData & data) {
	return static_cast<GVariantType>(data.vt);
}

inline void vtSetType(GVarTypeData & data, GVariantType vt) {
	data.vt = static_cast<uint16_t>(vt);
}

inline void vtSetType(GVarTypeData & data, int vt) {
	data.vt = static_cast<uint16_t>(vt);
}

inline unsigned int vtGetSize(const GVarTypeData & data) {
	return (data.sizeAndPointers >> 4) & 0xf;
}

inline void vtSetSize(GVarTypeData & data, unsigned int size) {
	GASSERT_MSG(size < 16, "GVarTypeData: too big size");
	
	data.sizeAndPointers = static_cast<uint8_t>((data.sizeAndPointers & 0xf) + (size << 4));
}

inline int vtGetPointers(const GVarTypeData & data) {
	return data.sizeAndPointers & 0xf;
}

inline void vtSetPointers(GVarTypeData & data, unsigned int pointers) {
	GASSERT_MSG(pointers < 16, "GVarTypeData: too many pointers");

	data.sizeAndPointers = static_cast<uint8_t>((data.sizeAndPointers & 0xf0) + pointers);
}

inline bool vtIsShadow(int vt) {
	return vt == vtShadow || vt == vtString;
}

inline void vtInit(GVarTypeData & data) {
	data.vt = vtEmpty;
	data.sizeAndPointers = 0;
	data.padding = 0;
}

#include "cpgf/private/gvartypedata_p.h"

template <typename T>
void deduceVariantType(GVarTypeData & data, bool allowShadow)
{
	GVariantType vt = variant_internal::DeduceVariantType<T>::Result;

	if(allowShadow && vt == vtObject && variant_internal::DeduceVariantType<T>::Pointers == 0) {
		vt = vtShadow;
	}

	vtSetType(data, vt);
	vtSetPointers(data, variant_internal::DeduceVariantType<T>::Pointers);
}

template <typename T>
void deduceVariantType(GVarTypeData & data)
{
	deduceVariantType<T>(data, false);
}




} // namespace cpgf



#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif


