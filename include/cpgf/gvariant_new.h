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

struct VariantTypeInfo
{
	int size;
};

extern VariantTypeInfo variantTypeInfo[];

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

inline bool vtIsFundamental(const uint16_t vt)
{
	return vt >= (uint16_t)GVariantType::vtFundamentalBegin && vt <= (uint16_t)GVariantType::vtFundamentalEnd;
}

inline bool vtIsPointerOrReference(const uint16_t vt)
{
	return (vt & (int)GVariantType::maskByPointerAndReference) != 0;
}

inline bool vtIsByPointer(const uint16_t vt)
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

inline bool vtIsByReference(const uint16_t vt)
{
	return (vt & (int)GVariantType::maskByReference) != 0;
}

inline bool vtIsTypedVar(const uint16_t vt) {
	return vt == (uint16_t)GVariantType::vtTypedVar;
}

inline bool vtIsEmpty(const uint16_t vt) {
	return vt == (uint16_t)GVariantType::vtEmpty;
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

inline void vtSetType(GVarTypeData & data, int vt) {
	data.vt = static_cast<uint16_t>(vt);
}

inline GVariantType vtGetBaseType(const uint16_t vt)
{
	return (GVariantType)(vt & (uint16_t)GVariantType::vtMask);
}

inline GVariantType vtGetBaseType(const GVariantType vt)
{
	return vtGetBaseType((uint16_t)vt);
}

inline GVariantType vtGetBaseType(const GVarTypeData & data)
{
	return vtGetBaseType(data.vt);
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

inline bool vtIsInterface(const uint16_t vt) {
	return vt == (uint16_t)GVariantType::vtInterface;
}

inline bool vtIsInterface(const GVariantType vt) {
	return vt == GVariantType::vtInterface;
}

inline bool vtIsBoolean(const uint16_t vt) {
	return vt == (uint16_t)GVariantType::vtBool;
}

inline bool vtIsBoolean(const GVariantType vt) {
	return vt == GVariantType::vtBool;
}

inline bool vtIsInteger(const uint16_t vt) {
	return vt >= (uint16_t)GVariantType::vtIntegerBegin && vt <= (uint16_t)GVariantType::vtIntegerEnd;
}

inline bool vtIsInteger(const GVariantType vt) {
	return vt >= GVariantType::vtIntegerBegin && vt <= GVariantType::vtIntegerEnd;
}

inline bool vtIsSignedInteger(const uint16_t vt) {
	return vt == (uint16_t)GVariantType::vtSignedChar
		|| vt == (uint16_t)GVariantType::vtSignedShort
		|| vt == (uint16_t)GVariantType::vtSignedInt
		|| vt == (uint16_t)GVariantType::vtSignedLong
		|| vt == (uint16_t)GVariantType::vtSignedLongLong
	;
}

inline bool vtIsSignedInteger(const GVariantType vt) {
	return vt == GVariantType::vtSignedChar
		|| vt == GVariantType::vtSignedShort
		|| vt == GVariantType::vtSignedInt
		|| vt == GVariantType::vtSignedLong
		|| vt == GVariantType::vtSignedLongLong
	;
}

inline bool vtIsUnsignedInteger(const uint16_t vt) {
	return vt == (uint16_t)GVariantType::vtUnsignedChar
		|| vt == (uint16_t)GVariantType::vtUnsignedShort
		|| vt == (uint16_t)GVariantType::vtUnsignedInt
		|| vt == (uint16_t)GVariantType::vtUnsignedLong
		|| vt == (uint16_t)GVariantType::vtUnsignedLongLong
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

inline bool vtIsReal(const uint16_t vt) {
	return vt >= (uint16_t)GVariantType::vtFloat && vt <= (uint16_t)GVariantType::vtLongDouble;
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
	GVariant(const T & value) : data()
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

struct VarantCastKeepConstRef {};
struct VarantCastCopyConstRef {};

#include "private/gvariant_from_p.h"

GVariant createStringVariant(const char * s);
GVariant createWideStringVariant(const wchar_t * s);
class GMetaType;
GVariant createTypedVariant(const GVariant & value, const GMetaType & type);

GVariant getVariantRealValue(const GVariant & value);

GVariant pointerToObjectVariant(void * p);
GVariant objectToVariant(void * object);

// Convert a pointer to reference.
GVariant variantPointerToLvalueReference(const GVariant & p);

template <typename T, typename Policy = VarantCastKeepConstRef>
typename variant_internal::VariantCastResult<T, Policy>::Result fromVariant(const GVariant & value,
		typename std::enable_if<! variant_internal::TypeListSame<
			cpgf::GTypeList<const GVariant &, GVariant &, GVariant, const volatile GVariant &, volatile GVariant>, T>::same>::type * = 0
	)
{
	using namespace variant_internal;

	auto vt = value.refData().typeData.vt;

	if(vtIsTypedVar(vt)) {
		return fromVariant<T>(getVariantRealValue(value));
	}
	else if(vtIsByPointer(vt)) {
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
typename variant_internal::VariantCastResult<T, Policy>::Result fromVariant(const GVariant & value,
		typename std::enable_if<variant_internal::TypeListSame<
			cpgf::GTypeList<const GVariant &, GVariant &, GVariant, const volatile GVariant &, volatile GVariant>, T>::same>::type * = 0
	)
{
	return (typename variant_internal::VariantCastResult<T, Policy>::Result)value;
}

template <typename T, typename Policy = VarantCastKeepConstRef>
bool canFromVariant(const GVariant & value)
{
	using namespace variant_internal;

	auto vt = value.refData().typeData.vt;
	
	if(vtIsTypedVar(vt)) {
		return canFromVariant<T, Policy>(getVariantRealValue(value));
	}
	else if(vtIsByPointer(vt)) {
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

template <typename T, typename V>
GVariant createVariant(const V & value, bool copyObject = false,
	typename std::enable_if<(std::is_copy_constructible<T>::value && (std::is_class<T>::value || std::is_union<T>::value))>::type * = 0
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
	typename std::enable_if<! (std::is_copy_constructible<T>::value && (std::is_class<T>::value || std::is_union<T>::value))>::type * = 0
)
{
	return GVariant::create<T>(value);
}

template <typename T>
GVariant createVariant(const GVariant & value, bool /*copyObject*/ = false)
{
	return value;
}

// TODO: the parameter Copyable is for backward compatibility
template <bool Copyable, typename T, typename V>
GVariant createVariant(const V & value, bool copyObject = false)
{
	return createVariant<T>(value, copyObject);
}

template <typename T>
GVariant copyVariantFromCopyable(const T & value)
{
	return createVariant<T>(value, true);
}

inline void * objectAddressFromVariant(const GVariant & v)
{
	return fromVariant<void *>(v);
}

// TODO: deduceVariantType is for back compatibility
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

inline GVariant createVariantFromData(const GVariantData & data)
{
	return GVariant(data);
}


} //namespace cpgf


#endif

