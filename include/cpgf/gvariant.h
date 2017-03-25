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
	uint16_t vt;
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
	return ((uint16_t)vt & (uint16_t)GVariantType::maskByPointerAndReference) != 0;
}

inline bool vtIsByPointer(const GVariantType vt)
{
	return ((uint16_t)vt & (uint16_t)GVariantType::byPointer) != 0;
}

inline bool vtIsLvalueReference(const GVariantType vt)
{
	return ((uint16_t)vt & (uint16_t)GVariantType::byLvalueReference) != 0;
}

inline bool vtIsRvalueReference(const GVariantType vt)
{
	return ((uint16_t)vt & (uint16_t)GVariantType::byRvalueReference) != 0;
}

inline bool vtIsByReference(const GVariantType vt)
{
	return ((uint16_t)vt & (uint16_t)GVariantType::maskByReference) != 0;
}

inline bool vtIsTypedVar(const GVariantType vt) {
	return vt == GVariantType::vtTypedVar;
}

inline bool vtIsEmpty(const GVariantType vt) {
	return vt == GVariantType::vtEmpty;
}

inline void vtInit(GVarTypeData & data) {
	data.vt = (uint16_t)GVariantType::vtEmpty;
	data.sizeAndPointers = 0;
	data.padding = 0;
}

inline GVariantType vtGetType(const GVarTypeData & data) {
	return static_cast<GVariantType>(data.vt);
}

inline void vtSetType(GVarTypeData & data, GVariantType vt) {
	data.vt = static_cast<uint16_t>(vt);
}

inline GVariantType vtGetBaseType(const GVariantType vt)
{
	return (GVariantType)((uint16_t)vt & (uint16_t)GVariantType::vtMask);
}

inline GVariantType vtGetBaseType(const GVarTypeData & data)
{
	return vtGetBaseType((GVariantType)data.vt);
}

inline int vtGetPointers(const GVarTypeData & data)
{
	return data.sizeAndPointers & 0x0f;
}

inline void vtSetPointers(GVarTypeData & data, unsigned int pointers)
{
	data.sizeAndPointers = static_cast<uint8_t>((data.sizeAndPointers & 0xf0) | (pointers & 0x0f));
}

inline int vtGetSize(const GVarTypeData & data)
{
	return (data.sizeAndPointers >> 4) & 0x0f;
}

inline void vtSetSize(GVarTypeData & data, unsigned int size)
{
	data.sizeAndPointers = static_cast<uint8_t>(((size & 0x0f) << 4) | (data.sizeAndPointers & 0x0f));
}

inline void vtSetSizeAndPointers(GVarTypeData & data, unsigned int size, unsigned int pointer)
{
	data.sizeAndPointers = static_cast<uint8_t>(((size & 0x0f) << 4) | (pointer & 0x0f));
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
		otherData.typeData.vt = (uint16_t)GVariantType::vtEmpty;
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
		this->data.typeData.vt = (uint16_t)GVariantType::vtEmpty;
		return result;
	}

	void reset()
	{
		variant_internal::releaseVariantData(this->data);
		this->data.typeData.vt = (uint16_t)GVariantType::vtEmpty;
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

inline bool variantIsString(const GVariant & v)
{
	const GVariantData & data = v.refData();
	return data.typeData.vt == (uint16_t)GVariantType::vtString
		|| ((vtGetPointers(data.typeData) == 1 && vtGetBaseType(data.typeData) == GVariantType::vtChar));
}

inline bool variantIsWideString(const GVariant & v)
{
	const GVariantData & data = v.refData();
	return data.typeData.vt == (uint16_t)GVariantType::vtWideString
		|| ((vtGetPointers(data.typeData) == 1 && vtGetBaseType(data.typeData) == GVariantType::vtWchar));
}

inline GVariant createVariantFromData(const GVariantData & data)
{
	return GVariant(data);
}

template <typename T, typename Policy>
typename variant_internal::VariantCastResult<T, Policy>::Result fromVariantData(const GVariantData & data,
	typename std::enable_if<! variant_internal::TypeListSame<
		cpgf::GTypeList<
			GVariant,
			const GVariant,
			const volatile GVariant,
			volatile GVariant,
			GVariant &,
			const GVariant &,
			const volatile GVariant &,
			volatile GVariant &
		>, T>::same>::type * = 0
);

// Policy is forced to be VarantCastCopyConstRef to avoid reference to local variable
template <typename T, typename Policy>
typename variant_internal::VariantCastResult<T, VarantCastCopyConstRef>::Result fromVariantData(const GVariantData & data,
	typename std::enable_if<variant_internal::TypeListSame<
		cpgf::GTypeList<
			GVariant,
			const GVariant,
			const volatile GVariant,
			volatile GVariant,
			GVariant &,
			const GVariant &,
			const volatile GVariant &,
			volatile GVariant &
		>, T>::same>::type * = 0
);

template <typename T, typename Policy = VarantCastKeepConstRef>
bool canFromVariantData(const GVariantData & data);

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

template <typename T, typename Policy>
typename variant_internal::VariantCastResult<T, Policy>::Result fromVariantData(const GVariantData & data,
		typename std::enable_if<! variant_internal::TypeListSame<
			cpgf::GTypeList<
				GVariant,
				const GVariant,
				const volatile GVariant,
				volatile GVariant,
				GVariant &,
				const GVariant &,
				const volatile GVariant &,
				volatile GVariant &
			>, T>::same>::type *
	)
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
typename variant_internal::VariantCastResult<T, VarantCastCopyConstRef>::Result fromVariantData(const GVariantData & data,
		typename std::enable_if<variant_internal::TypeListSame<
			cpgf::GTypeList<
				GVariant,
				const GVariant,
				const volatile GVariant,
				volatile GVariant,
				GVariant &,
				const GVariant &,
				const volatile GVariant &,
				volatile GVariant &
			>, T>::same>::type *
	)
{
	return createVariantFromData(data);
}

#ifdef G_COMPILER_VC
#pragma warning(pop)
#endif

#ifdef G_COMPILER_GCC
#pragma GCC diagnostic pop
#endif


template <typename T, typename Policy = VarantCastKeepConstRef>
typename variant_internal::VariantCastResult<T, Policy>::Result fromVariant(const GVariant & value,
		typename std::enable_if<! variant_internal::TypeListSame<
			cpgf::GTypeList<
				GVariant,
				const GVariant,
				const volatile GVariant,
				volatile GVariant,
				GVariant &,
				const GVariant &,
				const volatile GVariant &,
				volatile GVariant &
			>, T>::same>::type * = 0
	)
{
	return fromVariantData<T, Policy>(value.refData());
}

template <typename T, typename Policy = VarantCastKeepConstRef>
typename variant_internal::VariantCastResult<T, Policy>::Result fromVariant(const GVariant & value,
		typename std::enable_if<variant_internal::TypeListSame<
			cpgf::GTypeList<
				GVariant,
				const GVariant,
				const volatile GVariant,
				volatile GVariant,
				GVariant &,
				const GVariant &,
				const volatile GVariant &,
				volatile GVariant &
			>, T>::same>::type * = 0
	)
{
	return (typename variant_internal::VariantCastResult<T, Policy>::Result)value;
}

template <typename T, typename Policy>
bool canFromVariantData(const GVariantData & data)
{
	using namespace variant_internal;

	const GVariantType vt = vtGetType(data.typeData);

	if(vtIsByPointer(vt)) {
		if(vtIsLvalueReference(vt)) {
			return CastVariant_Pointer_LvalueReference<T, Policy>::canCast(data);
		}
		else if(vtIsRvalueReference(vt)) {
			return CastVariant_Pointer_RvalueReference<T, Policy>::canCast(data);
		}
		else {
			return CastVariant_Pointer<T, Policy>::canCast(data);
		}
	}
	else if(vtIsLvalueReference(vt)) {
		return CastVariant_LvalueReference<T, Policy>::canCast(data);
	}
	else if(vtIsRvalueReference(vt)) {
		return CastVariant_RvalueReference<T, Policy>::canCast(data);
	}
	else {
		return CastVariant_Value<T, Policy>::canCast(data);
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

		data.typeData.vt = (uint16_t)GVariantType::vtShadow;
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
	data.vt = (uint16_t)GVariantType::vtVoid;
}

template <typename T>
void deduceVariantType(GVarTypeData & data)
{
	deduceVariantType<T>(data, false);
}


} //namespace cpgf


#endif

