#ifndef CPGF_GVARIANT_H
#define CPGF_GVARIANT_H

#include "cpgf/gtypelist.h"
#include "cpgf/gerrorcode.h"
#include "cpgf/gexception.h"
#include "cpgf/ginterface.h"
#include "cpgf/gcompiler.h"
#include "cpgf/gapiutil.h"
#include "cpgf/gtypetraits.h"
#include "cpgf/gpp.h"

#include <type_traits>
#include <cstdint>
#include <memory>

namespace cpgf {

struct VarantCastKeepConstRef {};
struct VarantCastCopyConstRef {};

typedef uint16_t GVtType;
enum class GVariantType : GVtType {
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
	vtFloat = vtRealBegin, vtDouble = vtRealBegin + 1, vtLongDouble = vtRealBegin + 2,
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

inline GVariantType operator | (const GVariantType a, const GVariantType b)
{
	return (GVariantType)(static_cast<std::underlying_type<GVariantType>::type>(a) | static_cast<std::underlying_type<GVariantType>::type>(b));
}

inline GVariantType operator & (const GVariantType a, const GVariantType b)
{
	return (GVariantType)(static_cast<std::underlying_type<GVariantType>::type>(a) & static_cast<std::underlying_type<GVariantType>::type>(b));
}

inline GVariantType operator ~ (const GVariantType a)
{
	return (GVariantType)(~static_cast<std::underlying_type<GVariantType>::type>(a));
}

#pragma pack(push, 1)
#pragma pack(1)
struct GVarTypeData
{
	GVtType vt;
	uint8_t sizeAndPointers;
	uint8_t padding;
};
#pragma pack(pop)

typedef std::int64_t GVariantInteger;

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

		void * pointer;

		cpgf::IObject * valueInterface;

	};
};
#pragma pack(pop)

inline bool vtIsFundamental(const GVariantType vt)
{
	return vt >= GVariantType::vtFundamentalBegin && vt <= GVariantType::vtFundamentalEnd;
}

inline bool vtIsPointerOrReference(const GVariantType vt)
{
	return ((GVtType)vt & (GVtType)GVariantType::maskByPointerAndReference) != 0;
}

inline bool vtIsByPointer(const GVariantType vt)
{
	return ((GVtType)vt & (GVtType)GVariantType::byPointer) != 0;
}

inline bool vtIsLvalueReference(const GVariantType vt)
{
	return ((GVtType)vt & (GVtType)GVariantType::byLvalueReference) != 0;
}

inline bool vtIsRvalueReference(const GVariantType vt)
{
	return ((GVtType)vt & (GVtType)GVariantType::byRvalueReference) != 0;
}

inline bool vtIsByReference(const GVariantType vt)
{
	return ((GVtType)vt & (GVtType)GVariantType::maskByReference) != 0;
}

inline bool vtIsTypedVar(const GVariantType vt) {
	return vt == GVariantType::vtTypedVar;
}

inline bool vtIsEmpty(const GVariantType vt) {
	return vt == GVariantType::vtEmpty;
}

inline void vtInit(GVarTypeData & data) {
	data.vt = (GVtType)GVariantType::vtEmpty;
	data.sizeAndPointers = 0;
	data.padding = 0;
}

inline GVariantType vtGetType(const GVarTypeData & data) {
	return static_cast<GVariantType>(data.vt);
}

inline void vtSetType(GVarTypeData & data, GVariantType vt) {
	data.vt = static_cast<GVtType>(vt);
}

inline GVariantType vtGetBaseType(const GVariantType vt)
{
	return (GVariantType)((GVtType)vt & (GVtType)GVariantType::vtMask);
}

inline GVariantType vtGetBaseType(const GVarTypeData & data)
{
	return vtGetBaseType((GVariantType)data.vt);
}

inline int vtGetPointers(const GVarTypeData & data)
{
	return data.sizeAndPointers & 0x07;
}

inline void vtSetPointers(GVarTypeData & data, unsigned int pointers)
{
	data.sizeAndPointers = static_cast<uint8_t>((data.sizeAndPointers & 0xf8) | (pointers & 0x07));
}

inline int vtGetSize(const GVarTypeData & data)
{
	return (data.sizeAndPointers >> 3) & 0x1f;
}

inline void vtSetSize(GVarTypeData & data, unsigned int size)
{
	data.sizeAndPointers = static_cast<uint8_t>(((size & 0x1f) << 3) | (data.sizeAndPointers & 0x07));
}

inline void vtSetSizeAndPointers(GVarTypeData & data, unsigned int size, unsigned int pointer)
{
	data.sizeAndPointers = static_cast<uint8_t>(((size & 0x01f) << 3) | (pointer & 0x07));
}

inline bool vtIsInterface(const GVariantType vt) {
	return vt == GVariantType::vtInterface;
}

inline bool vtIsBoolean(const GVariantType vt) {
	return vt == GVariantType::vtBool;
}

inline bool vtIsInteger(const GVariantType vt) {
	return vt >= GVariantType::vtIntegerBegin && vt <= GVariantType::vtIntegerEnd;
}

inline bool vtIsSignedInteger(const GVariantType vt) {
	return vt == GVariantType::vtSignedChar
		|| vt == GVariantType::vtSignedShort
		|| vt == GVariantType::vtSignedInt
		|| vt == GVariantType::vtSignedLong
		|| vt == GVariantType::vtSignedLongLong
	;
}

inline bool vtIsUnsignedInteger(const GVariantType vt) {
	return vt == GVariantType::vtUnsignedChar
		|| vt == GVariantType::vtUnsignedShort
		|| vt == GVariantType::vtUnsignedInt
		|| vt == GVariantType::vtUnsignedLong
		|| vt == GVariantType::vtUnsignedLongLong
	;
}

inline bool vtIsReal(const GVariantType vt) {
	return vt >= GVariantType::vtFloat && vt <= GVariantType::vtLongDouble;
}


#include "private/gvariant_common_p.h"
#include "private/gvariant_create_p.h"

class GVariant
{
public:
	template <typename T, typename V>
	static GVariant create(const V & value)
	{
		GVariant v;
		variant_internal::variantDeduceAndSet<T>(&v.data, value);
		return v;
	}

	static GVariant create(const GVariant & value)
	{
		return value;
	}

	GVariant() : data() {
	}

	template <typename T>
	GVariant(const T & value,
		typename std::enable_if<! std::is_same<T, GVariantData>::value>::type * = 0)
		: data()
	{
		variant_internal::variantDeduceAndSet<T>(&this->data, value);
	}

	GVariant(const GVariant & other) : data(other.data)
	{
		variant_internal::retainVariantData(this->data);
	}

	GVariant(GVariant && other) : data()
	{
		this->swap(other);
	}

	explicit GVariant(const GVariantData & otherData) : data(otherData)
	{
		variant_internal::retainVariantData(this->data);
	}

	explicit GVariant(GVariantData && otherData) : data(otherData)
	{
		otherData.typeData.vt = (GVtType)GVariantType::vtEmpty;
	}

	GVariant & operator = (GVariant other)
	{
		this->swap(other);

		return *this;
	}

	~GVariant() {
		variant_internal::releaseVariantData(this->data);
	}

	GVariantType getType() const {
		return (GVariantType)this->data.typeData.vt;
	}

	bool isEmpty() const {
		return this->getType() == GVariantType::vtEmpty;
	}

	const GVariantData & refData() const {
		return this->data;
	}

	GVariantData & refData() {
		return this->data;
	}

	GVariantData takeData()
	{
		GVariantData result = this->data;
		this->data.typeData.vt = (GVtType)GVariantType::vtEmpty;
		return result;
	}

	void reset()
	{
		variant_internal::releaseVariantData(this->data);
		this->data.typeData.vt = (GVtType)GVariantType::vtEmpty;
	}

	void swap(GVariant & other) {
		using std::swap;

		swap(this->data, other.data);
	}

private:
	GVariantData data;
};

inline void swap(GVariant & a, GVariant & b)
{
	a.swap(b);
}

namespace variant_internal {

struct VariantCastTag {};
struct VariantCastTag_VariantSelf {};

template <typename T>
struct VariantCastTagTraits
{
	typedef typename std::conditional<
		TypeListSame<
			cpgf::GTypeList<
				GVariant,
				const GVariant,
				const volatile GVariant,
				volatile GVariant,
				GVariant &,
				const GVariant &,
				const volatile GVariant &,
				volatile GVariant &
			>,
		T>::same,
		VariantCastTag_VariantSelf,
		VariantCastTag
	>::type Tag;
};

} //namespace variant_internal

template <typename T, typename Policy>
auto doFromVariantData(const GVariantData & data, variant_internal::VariantCastTag)
	-> typename variant_internal::VariantCastResult<T, Policy>::Result;

// Policy is forced to be VarantCastCopyConstRef to avoid reference to local variable
template <typename T, typename Policy>
auto doFromVariantData(const GVariantData & data, variant_internal::VariantCastTag_VariantSelf)
	-> typename variant_internal::VariantCastResult<T, VarantCastCopyConstRef>::Result;

template <typename T, typename Policy>
auto fromVariantData(const GVariantData & data)
	-> decltype(
		doFromVariantData<typename variant_internal::VariantDecay<T>::Result, Policy>(
			data,
			typename variant_internal::VariantCastTagTraits<typename variant_internal::VariantDecay<T>::Result>::Tag()
		)
	);

template <typename T, typename Policy = VarantCastKeepConstRef>
bool canFromVariantData(const GVariantData & data);

inline GVariant createVariantFromData(const GVariantData & data)
{
	return GVariant(data);
}

bool variantIsString(const GVariant & v);
bool variantIsWideString(const GVariant & v);

// Is either string or wide string
bool variantIsAnyString(const GVariant & v);
// If the variant is a string, return it
// If the variant is a wide string, convert it to string
std::string stringFromVariant(const GVariant & v);
// If the variant is a wide string, return it
// If the variant is a string, convert it to wide string
std::wstring wideStringFromVariant(const GVariant & v);

// The result is vtString
GVariant createStringVariant(const char * s);

// The result is vtWideString
GVariant createWideStringVariant(const wchar_t * s);

class GMetaType;

// The result is vtTypedVar
GVariant createTypedVariant(const GVariant & value, const GMetaType & type);

GVariant getVariantRealValue(const GVariant & value);

// The result is a pointer to object (vtObject | vtPointer).
GVariant createObjectVariantFromPointer(void * p);

// The result is an object (vtObject).
GVariant createObjectVariant(void * object);

// Convert a pointer to reference.
GVariant variantPointerToLvalueReference(const GVariant & p);

#include "private/gvariant_from_p.h"

#ifdef G_COMPILER_GCC
#pragma GCC diagnostic push
// ignore warning: returning reference to temporary
// those "problematical" code is not executed on those types so it's safe to ignore the warning.
#pragma GCC diagnostic ignored "-Wreturn-local-addr"
#endif

#ifdef G_COMPILER_VC
#pragma warning(push)
#pragma warning(disable:4172) // warning C4172: returning address of local variable or temporar
#endif

template <typename T, typename Policy>
auto doFromVariantData(const GVariantData & data, variant_internal::VariantCastTag)
	-> typename variant_internal::VariantCastResult<T, Policy>::Result
{
	using namespace variant_internal;

	const GVariantType vt = vtGetType(data.typeData);

	if(vtIsByPointer(vt)) {
		if(vtIsLvalueReference(vt)) {
			return CastVariant_Pointer_LvalueReference<T, Policy>::cast(data);
		}
		else if(vtIsRvalueReference(vt)) {
			return CastVariant_Pointer_RvalueReference<T, Policy>::cast(data);
		}
		else {
			return CastVariant_Pointer<T, Policy>::cast(data);
		}
	}
	else if(vtIsLvalueReference(vt)) {
		return CastVariant_LvalueReference<T, Policy>::cast(data);
	}
	else if(vtIsRvalueReference(vt)) {
		return CastVariant_RvalueReference<T, Policy>::cast(data);
	}
	else {
		return CastVariant_Value<T, Policy>::cast(data);
	}
}

// Policy is forced to be VarantCastCopyConstRef to avoid reference to local variable
template <typename T, typename Policy>
auto doFromVariantData(const GVariantData & data, variant_internal::VariantCastTag_VariantSelf)
	-> typename variant_internal::VariantCastResult<T, VarantCastCopyConstRef>::Result
{
	return createVariantFromData(data);
}

template <typename T, typename Policy>
auto fromVariantData(const GVariantData & data)
	-> decltype(
		doFromVariantData<typename variant_internal::VariantDecay<T>::Result, Policy>(
			data,
			typename variant_internal::VariantCastTagTraits<typename variant_internal::VariantDecay<T>::Result>::Tag()
		)
	)
{
	typedef typename variant_internal::VariantDecay<T>::Result U;

	return doFromVariantData<U, Policy>(data, typename variant_internal::VariantCastTagTraits<U>::Tag());
}

#ifdef G_COMPILER_VC
#pragma warning(pop)
#endif

#ifdef G_COMPILER_GCC
#pragma GCC diagnostic pop
#endif


template <typename T, typename Policy>
auto doFromVariant(const GVariant & value, variant_internal::VariantCastTag)
	-> typename variant_internal::VariantCastResult<typename variant_internal::VariantDecay<T>::Result, Policy>::Result
{
	return fromVariantData<T, Policy>(value.refData());
}

template <typename T, typename Policy>
auto doFromVariant(const GVariant & value, variant_internal::VariantCastTag_VariantSelf)
	-> typename variant_internal::VariantCastResult<typename variant_internal::VariantDecay<T>::Result, VarantCastCopyConstRef>::Result
{
	typedef typename variant_internal::VariantDecay<T>::Result U;
	return (typename variant_internal::VariantCastResult<U, VarantCastCopyConstRef>::Result)value;
}

template <typename T, typename Policy = VarantCastKeepConstRef>
auto fromVariant(const GVariant & value)
	-> decltype(
		doFromVariant<T, Policy>(
			value,
			typename variant_internal::VariantCastTagTraits<typename variant_internal::VariantDecay<T>::Result>::Tag()
		)
	)
{
	typedef typename variant_internal::VariantDecay<T>::Result U;

	return doFromVariant<U, Policy>(value, typename variant_internal::VariantCastTagTraits<U>::Tag());
}

template <typename T, typename Policy>
bool canFromVariantData(const GVariantData & data)
{
	typedef typename variant_internal::VariantDecay<T>::Result U;

	using namespace variant_internal;

	const GVariantType vt = vtGetType(data.typeData);

	if(vtIsByPointer(vt)) {
		if(vtIsLvalueReference(vt)) {
			return CastVariant_Pointer_LvalueReference<U, Policy>::canCast(data);
		}
		else if(vtIsRvalueReference(vt)) {
			return CastVariant_Pointer_RvalueReference<U, Policy>::canCast(data);
		}
		else {
			return CastVariant_Pointer<U, Policy>::canCast(data);
		}
	}
	else if(vtIsLvalueReference(vt)) {
		return CastVariant_LvalueReference<U, Policy>::canCast(data);
	}
	else if(vtIsRvalueReference(vt)) {
		return CastVariant_RvalueReference<U, Policy>::canCast(data);
	}
	else {
		return CastVariant_Value<U, Policy>::canCast(data);
	}
}

template <typename T, typename Policy = VarantCastKeepConstRef>
bool canFromVariant(const GVariant & value)
{
	return canFromVariantData<T, Policy>(value.refData());
}

template <typename T, typename V>
GVariant createVariant(const V & value, bool copyObject = false,
		typename std::enable_if<
			(std::is_copy_constructible<T>::value && (std::is_class<T>::value || std::is_union<T>::value))
		>::type * = 0
	)
{
	if(copyObject) {
		GVariant v;
		GVariantData & data = v.refData();

		data.typeData.vt = (GVtType)GVariantType::vtShadow;
		vtSetSizeAndPointers(data.typeData, sizeof(void *), 0);
		data.valueInterface = new variant_internal::GVariantShadowObject<T>(value);

		return v;
	}
	else {
		return GVariant::create<T>(value);
	}
}

template <typename T, typename V>
GVariant createVariant(const V & value, bool /*copyObject*/ = false,
		typename std::enable_if<
			! (std::is_copy_constructible<T>::value && (std::is_class<T>::value || std::is_union<T>::value))
		>::type * = 0
)
{
	return GVariant::create<T>(value);
}

template <typename T>
GVariant createVariant(const GVariant & value, bool /*copyObject*/ = false)
{
	return value;
}

template <typename T>
GVariant copyVariantFromCopyable(const T & value)
{
	return createVariant<typename std::remove_reference<T>::type>(value, true);
}

inline void * objectAddressFromVariant(const GVariant & v)
{
	return fromVariant<void *>(v);
}

template <typename T>
void deduceVariantType(GVarTypeData & data, bool /*copyObject*/)
{
	GVariantData variantData;
	variant_internal::variantDeduceType<T>(&variantData);
	data = variantData.typeData;
}

template <>
inline void deduceVariantType<void>(GVarTypeData & data, bool /*copyObject*/)
{
	data.vt = (GVtType)GVariantType::vtVoid;
}

template <typename T>
void deduceVariantType(GVarTypeData & data)
{
	deduceVariantType<T>(data, false);
}


} //namespace cpgf


#endif
