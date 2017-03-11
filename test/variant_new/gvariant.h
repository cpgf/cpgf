#ifndef GVARIANT_H
#define GVARIANT_H

#include "cpgf/gtypelist.h"
#include "cpgf/gerrorcode.h"
#include "cpgf/gexception.h"
#include "cpgf/gcompiler.h"

#include <type_traits>
#include <cstdint>
#include <memory>

namespace xcpgf {

inline void failedCast()
{
	cpgf::raiseCoreException(cpgf::Error_Variant_FailCast);
}

inline void checkFailCast(bool success)
{
	if(!success) {
		failedCast();
	}
}

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

typedef cpgf::GTypeList<
	bool,
	char, wchar_t,
	signed char, unsigned char,
	signed short, unsigned short,
	signed int, unsigned int,
	signed long, unsigned long,
	signed long long, unsigned long long,
	float, double, long double
> FundamentalTypeList;

constexpr int FundamentalTypeStartType = (int)GVariantType::vtFundamentalBegin;

struct VariantTypeInfo
{
	int size;
};

// TODO: need to move to .cpp file
VariantTypeInfo variantTypeInfo[] = {
	{ 0 }, // vtEmpty
	{ 0 }, // vtVoid

	{ sizeof(bool) }, // vtBool
	{ sizeof(char) },
	{ sizeof(wchar_t) },
	{ sizeof(signed char) },
	{ sizeof(unsigned char) },
	{ sizeof(signed short) },
	{ sizeof(unsigned short) },
	{ sizeof(signed int) },
	{ sizeof(unsigned int) },
	{ sizeof(signed long) },
	{ sizeof(unsigned long) },
	{ sizeof(signed long long) },
	{ sizeof(unsigned long long) },
	{ sizeof(float) },
	{ sizeof(double) },
	{ sizeof(long double) },

	{ sizeof(void *) },
	{ sizeof(void *) },
	{ sizeof(void *) },
	{ sizeof(void *) },
	{ sizeof(void *) },
	{ sizeof(void *) },
	{ sizeof(void *) },
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

typedef std::int64_t GVariantInteger;
typedef long double GVariantReal;

struct IObject;

#pragma pack(push, 1)
#pragma pack(1)
struct GVariantData
{
	GVarTypeData typeData;

	union {
		std::int8_t valueInt8;
		std::int16_t valueInt16;
		std::int32_t valueInt32;
		std::int64_t valueInt64;
		GVariantInteger valueInt;
	
		float valueFloat;
		double valueDouble;
		long double valueLongDouble;
		GVariantReal valueReal;
	
		void * pointer;
		
		IObject * valueInterface;

	};
};
#pragma pack(pop)

#include "private/gvariant_common_p.h"

#include "private/gvariant_create_p.h"

class GVariant
{
public:
	template <typename T>
	static GVariant create(const T & value)
	{
		GVariant v;
		deduceVariantType<T>(&v.data, value);
		return v;
	}
	
	GVariant() : data() {
	}

	template <typename T>
	GVariant(const T & value) : data()
	{
		deduceVariantType(&this->data, value);
	}
	
	const GVariantData & refData() const {
		return this->data;
	}

	GVariantData & refData() {
		return this->data;
	}

private:
	GVariantData data;
};

inline bool vtIsPointerOrReference(const uint16_t vt)
{
	return (vt & (int)GVariantType::maskByPointerAndReference) != 0;
}

inline bool vtIsPointer(const uint16_t vt)
{
	return (vt & (int)GVariantType::byPointer) != 0;
}

inline bool vtIsLvalueReference(const uint16_t vt)
{
	return (vt & (int)GVariantType::byLvalueReference) != 0;
}

inline bool vtIsRvalueReference(const uint16_t vt)
{
	return (vt & (int)GVariantType::byRvalueReference) != 0;
}

inline bool vtIsReference(const uint16_t vt)
{
	return (vt & (int)GVariantType::maskByReference) != 0;
}

inline uint16_t vtGetBaseType(const uint16_t vt)
{
	return (vt & (int)GVariantType::vtMask);
}

struct VarantCastKeepConstRef {};
struct VarantCastCopyConstRef {};

#include "private/gvariant_from_p.h"

template <typename T, typename Policy = VarantCastKeepConstRef>
typename VariantCastResult<T, Policy>::Result fromVariant(const GVariant & value)
{
	auto vt = value.refData().typeData.vt;
	if(vtIsPointer(vt)) {
		if(vtIsLvalueReference(vt)) {
			return CastVariant_Pointer_LvalueReference<T, Policy>::cast(value);
		}
		else if(vtIsRvalueReference(vt)) {
			return CastVariant_Pointer_RvalueReference<T, Policy>::cast(value);
		}
		else {
			return CastVariant_Pointer<T, Policy>::cast(value);
		}
	}
	else if(vtIsLvalueReference(vt)) {
		return CastVariant_LvalueReference<T, Policy>::cast(value);
	}
	else if(vtIsRvalueReference(vt)) {
		return CastVariant_RvalueReference<T, Policy>::cast(value);
	}
	else {
		return CastVariant_Value<T, Policy>::cast(value);
	}
}


template <typename T, typename Policy = VarantCastKeepConstRef>
bool canFromVariant(const GVariant & value)
{
	auto vt = value.refData().typeData.vt;
	if(vtIsPointer(vt)) {
		if(vtIsLvalueReference(vt)) {
			return CastVariant_Pointer_LvalueReference<T, Policy>::canCast(value);
		}
		else if(vtIsRvalueReference(vt)) {
			return CastVariant_Pointer_RvalueReference<T, Policy>::canCast(value);
		}
		else {
			return CastVariant_Pointer<T, Policy>::canCast(value);
		}
	}
	else if(vtIsLvalueReference(vt)) {
		return CastVariant_LvalueReference<T, Policy>::canCast(value);
	}
	else if(vtIsRvalueReference(vt)) {
		return CastVariant_RvalueReference<T, Policy>::canCast(value);
	}
	else {
		return CastVariant_Value<T, Policy>::canCast(value);
	}
}

template <bool Copyable, typename T>
GVariant createVariant(const T & value, bool copyObject = false)
{
	return GVariant(value);
}



} //namespace cpgf


#endif
