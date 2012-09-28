#ifndef __GVARTYPEDATA_H
#define __GVARTYPEDATA_H

#include "cpgf/gcompiler.h"
#include "cpgf/gstdint.h"
#include "cpgf/gassert.h"
#include "cpgf/gtypetraits.h"
#include "cpgf/gtypelist.h"
#include "cpgf/genableif.h"
#include "cpgf/ginterface.h"


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

	vtPointer = 30, // T * &. If no reference, T * is type of T + byPointer
	vtObject = 31, // is equivalent to unkown type
	vtShadow = 32,
	vtString = 33,
	vtWideString = 34,
	vtInterface = 35, // IObject *
	vtTypedVar = 36,

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


inline GVariantType vtGetBaseType(GVariantType vt) {
	return static_cast<GVariantType>(vt & vtMask);
}

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

inline bool vtIsInterface(GVariantType vt) {
	return vt == vtInterface;
}

inline bool vtIsTypedVar(GVariantType vt) {
	return vt == vtTypedVar;
}

inline bool vtIsEmpty(GVariantType vt) {
	return vt == vtEmpty;
}

inline bool vtIsFundamental(GVariantType vt) {
	return vt >= vtFundamentalBegin && vt <= vtFundamentalEnd;
}

inline bool vtIsBoolean(GVariantType vt) {
	return vt == vtBool;
}

inline bool vtIsInteger(GVariantType vt) {
	return vt >= vtIntegerBegin && vt <= vtIntegerEnd;
}

inline bool vtIsSignedInteger(GVariantType vt) {
	return vt == vtSignedChar
		|| vt == vtSignedShort
		|| vt == vtSignedInt
		|| vt == vtSignedLong
		|| vt == vtSignedLongLong
	;
}

inline bool vtIsUnsignedInteger(GVariantType vt) {
	return vt == vtUnsignedChar
		|| vt == vtUnsignedShort
		|| vt == vtUnsignedInt
		|| vt == vtUnsignedLong
		|| vt == vtUnsignedLongLong
	;
}

inline bool vtIsReal(GVariantType vt) {
	return vt >= vtFloat && vt <= vtLongDouble;
}

inline bool vtIsByPointer(GVariantType vt) {
	return (vt & byPointer) != 0;
}

inline bool vtIsByReference(GVariantType vt) {
	return (vt & byReference) != 0;
}

inline bool vtIsVoidPointer(GVariantType vt) {
	return vt == (byPointer | vtVoid);
}

inline void vtInit(GVarTypeData & data) {
	data.vt = vtEmpty;
	data.sizeAndPointers = 0;
	data.padding = 0;
}

#include "cpgf/private/gvartypedata_p.h"

namespace variant_internal {
unsigned int getVariantTypeSize(GVariantType type);
} // namespace variant_internal

template <typename T>
void deduceVariantType(GVarTypeData & data, bool copyObject)
{
	GVariantType vt = variant_internal::DeduceVariantType<T>::Result;

	if(copyObject && vt == vtObject && variant_internal::DeduceVariantType<T>::Pointers == 0) {
		vt = vtShadow;
	}

	vtSetType(data, vt);
	vtSetPointers(data, variant_internal::DeduceVariantType<T>::Pointers);
	vtSetSize(data, variant_internal::getVariantTypeSize(vtGetType(data)));
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


