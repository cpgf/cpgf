#ifndef CPGF_GVARTYPEDATA_H
#define CPGF_GVARTYPEDATA_H

#include "cpgf/gcompiler.h"
#include "cpgf/gassert.h"
#include "cpgf/gtypetraits.h"
#include "cpgf/gtypelist.h"
#include "cpgf/genableif.h"
#include "cpgf/ginterface.h"

#include <cstdint>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
#endif


namespace cpgf {


enum class GVariantType : uint16_t {
	vtEmpty = 0,
	vtVoid = 1,

	vtFundamentalBegin = 2,

	vtIntegerBegin = vtFundamentalBegin, // 2
	vtBool = vtIntegerBegin,
	vtChar = vtIntegerBegin + 1, vtWchar = vtIntegerBegin + 2,
	vtSignedChar = vtIntegerBegin + 3, vtUnsignedChar = vtIntegerBegin + 4,
	vtSignedShort = vtIntegerBegin + 5, vtUnsignedShort = vtIntegerBegin + 6,
	vtSignedInt = vtIntegerBegin + 7, vtUnsignedInt = vtIntegerBegin + 8,
	vtSignedLong = vtIntegerBegin + 9, vtUnsignedLong = vtIntegerBegin + 10,
	vtSignedLongLong = vtIntegerBegin + 11, vtUnsignedLongLong = vtIntegerBegin + 12,
	vtIntegerEnd = vtUnsignedLongLong,

	vtRealBegin = vtIntegerEnd + 1, // 15
	vtFloat = vtRealBegin, vtDouble = vtRealBegin + 2, vtLongDouble = vtRealBegin + 3,
	vtRealEnd = vtLongDouble,

	vtFundamentalEnd = vtLongDouble,

	vtObject = 31, // is equivalent to unkown type
	
	// special types
	vtInterfaceBegin = 32,
	vtShadow = vtInterfaceBegin,
	vtString = vtInterfaceBegin + 1,
	vtWideString = vtInterfaceBegin + 2,
	vtInterface = vtInterfaceBegin + 3, // IObject *
	vtTypedVar = vtInterfaceBegin + 4,
	vtInterfaceEnd = vtTypedVar,

	vtUserBegin = 0xff,
	vtUserEnd = 0x0fff,

	vtMask = 0x0fff,

	// when there are both pointer and reference, it's always reference to pointer since we can't have pointer to reference
	byPointer = 0x1000,
	byLvalueReference = 0x2000,
	byRvalueReference = 0x4000,
	
	maskByPointerAndReference = byPointer | byLvalueReference | byRvalueReference,
	maskByReference = byLvalueReference | byRvalueReference,
};

#pragma pack(push, 1)
#pragma pack(1)
struct GVarTypeData
{
	uint16_t vt;
	uint8_t sizeAndPointers;
	uint8_t padding;
};
#pragma pack(pop)


} // namespace cpgf



#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif


