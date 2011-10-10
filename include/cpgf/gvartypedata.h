#ifndef __GVARTYPEDATA_H
#define __GVARTYPEDATA_H

#include "cpgf/gcompiler.h"
#include "cpgf/gstdint.h"
#include "cpgf/gassert.h"
#include "cpgf/gtypetraits.h"
#include "cpgf/gtypelist.h"
#include "cpgf/genableif.h"


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
};
#pragma pack(pop)


inline GVariantType vtGetBaseType(const GVarTypeData & data) {
	return static_cast<GVariantType>(data.vt & vtMask);
}

inline GVariantType vtGetType(const GVarTypeData & data) {
	return static_cast<GVariantType>(data.vt);
}

inline void vtSetType(GVarTypeData & data, GVariantType vt) {
	data.vt = vt;
}

inline void vtSetType(GVarTypeData & data, int vt) {
	data.vt = static_cast<GVariantType>(vt);
}

inline int vtGetSize(const GVarTypeData & data) {
	return (data.sizeAndPointers >> 4) & 0xf;
}

inline void vtSetSize(GVarTypeData & data, unsigned int size) {
	GASSERT_MSG(size < 16, "GVarTypeData: too big size");
	
	data.sizeAndPointers = (data.sizeAndPointers & 0xf) + (size << 4);
}

inline int vtGetPointers(const GVarTypeData & data) {
	return data.sizeAndPointers & 0xf;
}

inline void vtSetPointers(GVarTypeData & data, unsigned int pointers) {
	GASSERT_MSG(pointers < 16, "GVarTypeData: too many pointers");

	data.sizeAndPointers = (data.sizeAndPointers & 0xf0) + pointers;
}

inline void vtInit(GVarTypeData & data) {
	data.vt = vtEmpty;
	data.sizeAndPointers = 0;
}



namespace variant_internal {


template <typename T>
struct MaybeEnum
{
	enum { Result = IsConvertible<T, int>::Result && !IsFundamental<T>::Result && !IsClass<T>::Result && !IsReference<T>::Result && !IsPointer<T>::Result };
};

template <typename T>
struct DeduceBasicVariantType {
	static const GVariantType Result = IsPointer<T>::Result ? vtPointer : vtObject;
};

#define DEDUCEVT(RT, VT) template <> struct DeduceBasicVariantType <RT> { static const GVariantType Result = VT; }
DEDUCEVT(bool, vtBool);
DEDUCEVT(char, vtChar);
DEDUCEVT(wchar_t, vtWchar);
DEDUCEVT(signed char, vtSignedChar);
DEDUCEVT(unsigned char, vtUnsignedChar);
DEDUCEVT(signed short, vtSignedShort);
DEDUCEVT(unsigned short, vtUnsignedShort);
DEDUCEVT(signed int, vtSignedInt);
DEDUCEVT(unsigned int, vtUnsignedInt);
DEDUCEVT(long, vtSignedLong);
DEDUCEVT(unsigned long, vtUnsignedLong);
DEDUCEVT(signed long long, vtSignedLongLong);
DEDUCEVT(unsigned long long, vtUnsignedLongLong);
DEDUCEVT(float, vtFloat);
DEDUCEVT(double, vtDouble);
DEDUCEVT(long double, vtLongDouble);
DEDUCEVT(void, vtVoid);
#undef DEDUCEVT

template <typename T, typename Enabled = void>
struct DeduceVariantType_Helper {
	static const GVariantType Result = DeduceBasicVariantType<typename RemoveConstVolatile<T>::Result>::Result;
	static const int Pointers = 0;
};

template <typename T>
struct DeduceVariantType_Helper <T, typename GEnableIf<IsPointer<T>::Result>::Result> {
private:
	static const int temp = DeduceBasicVariantType<typename RemoveConstVolatile<typename RemovePointer<T>::Result>::Result>::Result;
public:
	static const GVariantType Result = static_cast<const GVariantType>(temp | byPointer);
	static const int Pointers = PointerDimension<
		typename RemoveConstVolatile<
			typename RemoveReference<
				typename RemoveConstVolatile<T>::Result
			>::Result
		>::Result
	>::Result;
};

template <typename T>
struct DeduceVariantType_Helper <T, typename GEnableIf<IsReference<T>::Result>::Result> {
private:
	static const int temp = DeduceBasicVariantType<typename RemoveConstVolatile<T>::Result>::Result;
public:
	static const GVariantType Result = static_cast<const GVariantType>(temp | byReference);
	static const int Pointers = 0;
};

template <typename T>
struct DeduceVariantType_Helper <T, typename GEnableIf<MaybeEnum<T>::Result>::Result> {
	static const GVariantType Result = vtUnsignedInt;
	static const int Pointers = 0;
};

template <typename T>
struct ArrayToPointer
{
	typedef T Result;
	enum { IsArray = false };
};

#define DEF_ARRAY_TO_PTR(CV) \
	template <typename T> struct ArrayToPointer <CV T[]> { typedef const volatile typename ArrayToPointer<T>::Result * Result; enum { IsArray = true }; }; \
	template <typename T> struct ArrayToPointer <CV T (*) []> { typedef const volatile typename ArrayToPointer<T>::Result * Result; enum { IsArray = true }; };

DEF_ARRAY_TO_PTR(GPP_EMPTY())
DEF_ARRAY_TO_PTR(const)
DEF_ARRAY_TO_PTR(volatile)
DEF_ARRAY_TO_PTR(const volatile)

#undef DEF_ARRAY_TO_PTR

#define DEF_ARRAY_TO_PTR(CV) \
	template <typename T, unsigned int N> struct ArrayToPointer <CV T[N]> { typedef const volatile typename ArrayToPointer<T>::Result * Result; enum { IsArray = true }; }; \
	template <typename T, unsigned int N> struct ArrayToPointer <CV T (*) [N]> { typedef const volatile typename ArrayToPointer<T>::Result * Result; enum { IsArray = true }; }; \
	template <typename T, unsigned int N> struct ArrayToPointer <CV T (&) [N]> { typedef const volatile typename ArrayToPointer<T>::Result * Result; enum { IsArray = true }; };

DEF_ARRAY_TO_PTR(GPP_EMPTY())
DEF_ARRAY_TO_PTR(const)
DEF_ARRAY_TO_PTR(volatile)
DEF_ARRAY_TO_PTR(const volatile)

#undef DEF_ARRAY_TO_PTR

template <typename T>
struct DeduceVariantType {
private:
	typedef DeduceVariantType_Helper<typename ArrayToPointer<typename RemoveConstVolatile<T>::Result>::Result> Deducer;
public:
	static const GVariantType Result = Deducer::Result;
	static const int Pointers = Deducer::Pointers;
};

template <>
struct DeduceVariantType <void> {
	static const GVariantType Result = vtVoid;
	static const int Pointers = 0;
};

} // namespace variant_internal

template <typename T>
void deduceVariantType(GVarTypeData & data, bool allowShadow)
{
	GVariantType vt = variant_internal::DeduceVariantType<T>::Result;

	if(allowShadow && vt == vtObject) {
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



#endif

