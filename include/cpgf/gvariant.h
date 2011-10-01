#ifndef __GVARIANT_H
#define __GVARIANT_H

#include "cpgf/gcompiler.h"
#include "cpgf/gassert.h"
#include "cpgf/gtypetraits.h"
#include "cpgf/gtypelist.h"
#include "cpgf/genableif.h"

#include <stdexcept>
#include <iostream>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4244)
#pragma warning(disable:4800) // warning C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
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

class GVariant;

template <typename T>
bool canFromVariant(const GVariant & v);

namespace variant_internal {

template <typename T>
void InitVariant(GVariant & v, GVariantType vt, const T & value);

template <typename T>
struct DeduceVariantType;

template <typename T>
struct ArrayToPointer;

template <typename T, typename Enabled = void>
struct DeducePassType
{
	typedef T Result;
	typedef const T & PassType;
};

template <typename T>
struct DeducePassType <T, typename GEnableIf<ArrayToPointer<T>::IsArray>::Result>
{
	typedef typename ArrayToPointer<T>::Result Result;
	typedef typename ArrayToPointer<T>::Result PassType;
};

class IVariantShadowObject
{
public:
	virtual void G_API_CC retain() = 0;
	virtual void G_API_CC release() = 0;
	virtual void * G_API_CC getObject() = 0;
};

template <typename T>
class GVariantShadowObject : public IVariantShadowObject
{
public:
	GVariantShadowObject(const T & obj) : refCount(1), obj(obj) {
	}

	virtual void G_API_CC retain() {
		++this->refCount;
	}

	virtual void G_API_CC release() {
		--this->refCount;
		if(!this->refCount) {
			delete this;
		}
	}

	virtual void * G_API_CC getObject() {
		return (void *)(&this->obj);
	}

private:
	int refCount;
	T obj;
};

struct GVariantDataHolder
{
	int a;
	int b;
	int c;
};


} // namespace variant_internal


inline bool vtIsEmpty(int vt) {
	return vt == vtEmpty;
}

inline bool vtIsFundamental(int vt) {
	return vt >= vtFundamentalBegin && vt <= vtFundamentalEnd;
}

inline bool vtIsBoolean(int vt) {
	return vt == vtBool;
}

inline bool vtIsInteger(int vt) {
	return vt >= vtIntegerBegin && vt <= vtIntegerEnd;
}

inline bool vtIsReal(int vt) {
	return vt >= vtFloat && vt <= vtLongDouble;
}

inline bool vtIsByPointer(int vt) {
	return (vt & byPointer) != 0;
}

inline bool vtIsByReference(int vt) {
	return (vt & byReference) != 0;
}

inline bool vtIsVoidPointer(int vt) {
	return vt == (byPointer | vtVoid);
}

#pragma pack(push, 1)
#pragma pack(1)
struct GVarData
{
	unsigned short type;
	unsigned char size;
	unsigned char padding;

	union {
		variant_internal::GVariantDataHolder holder;

		bool valueBool;
		char valueChar;
		wchar_t valueWchar;
		signed char valueSignedChar;
		unsigned char valueUnsignedChar;
		signed short valueSignedShort;
		unsigned short valueUnsignedShort;
		signed int valueSignedInt;
		unsigned int valueUnsignedInt;
		signed long valueSignedLong;
		unsigned long valueUnsignedLong;
		signed long long valueSignedLongLong;
		unsigned long long valueUnsignedLongLong;
		float valueFloat;
		double valueDouble;
		long double valueLongDouble;
		const volatile void * valuePointer;
		const volatile void * valueObject;

		const volatile bool * ptrBool;
		const volatile char * ptrChar;
		const volatile wchar_t * ptrWchar;
		const volatile signed char * ptrSignedChar;
		const volatile unsigned char * ptrUnsignedChar;
		const volatile signed short * ptrSignedShort;
		const volatile unsigned short * ptrUnsignedShort;;
		const volatile signed int * ptrSignedInt;
		const volatile unsigned int * ptrUnsignedInt;
		const volatile signed long * ptrSignedLong;
		const volatile unsigned long * ptrUnsignedLong;
		const volatile signed long long * ptrSignedLongLong;
		const volatile unsigned long long * ptrUnsignedLongLong;
		const volatile float * ptrFloat;
		const volatile double * ptrDouble;
		const volatile long double * ptrLongDouble;
		const volatile void * ptrPointer;
		const volatile void * ptrObject;

		variant_internal::IVariantShadowObject * shadowObject;

	};
};
#pragma pack(pop)

namespace variant_internal {

inline int getVariantTypeSize(GVariantType type);
inline void adjustVariantType(GVariant * data);

} // namespace variant_internal

class GVariant
{
//	GASSERT_STATIC(sizeof(GVarData) == 16);

public:
	GVariant() {
		data.type = vtEmpty;
		data.size = 0;
		data.holder.a = 0;
		data.holder.b = 0;
		data.holder.c = 0;
	}

	GVariant(const GVarData & data) : data(data) {
		if(this->data.size != variant_internal::getVariantTypeSize(static_cast<GVariantType>(this->data.type))) {
			variant_internal::adjustVariantType(this);
		}
	}

	template <typename T>
	GVariant(const T & value) {
		GVariantType vt = variant_internal::DeduceVariantType<T>::Result;
		variant_internal::InitVariant(*this, vt, static_cast<typename variant_internal::DeducePassType<T>::PassType>(value));
	}

	template <typename T>
	GVariant(GVariantType vt, const T & value) {
		variant_internal::InitVariant(*this, vt, value);
	}

	GVariant(const GVariant & other) : data(other.data) {
		if(this->data.type == vtShadow) {
			this->data.shadowObject->retain();
		}
	}

	~GVariant() {
		if(this->data.type == vtShadow) {
			this->data.shadowObject->release();
		}
	}

	GVariant & operator = (GVariant other) {
		this->swap(other);

		return *this;
	}

	void swap(GVariant & other) {
		using std::swap;

		swap(this->data.type, other.data.type);
		swap(this->data.size, other.data.size);
		swap(this->data.holder, other.data.holder);
	}

	GVariantType getType() const {
		return static_cast<GVariantType>(this->data.type);
	}

	bool isEmpty() const {
		return vtIsEmpty(this->getType());
	}

	GVarData getData() const {
		return this->data;
	}

	GVarData takeData() {
		GVarData result = this->data;

		this->data.type = vtEmpty;
		this->data.size = 0;

		return result;
	}

public:
	GVarData data;
};

inline void swap(GVariant & a, GVariant & b)
{
	a.swap(b);
}

class GVariantException : public std::runtime_error {
public:
	GVariantException(const std::string & message) : std::runtime_error(message) {
	}

};

inline void failedCast() {
	throw GVariantException("GVariant: cast failure");
}

inline void checkFailCast(bool success) {
	if(!success) {
		failedCast();
	}
}


namespace variant_internal {

template <typename T>
bool isPointer() {
	return IsPointer<typename RemoveReference<T>::Result>::Result;
}

template <typename T>
bool isNonPointer() {
	return !isPointer<T>();
}

template <typename T>
bool isPrimary() {
	return IsFundamental<typename RemoveReference<T>::Result>::Result;
}

template <typename T>
bool isNonPrimary() {
	return !isPrimary<T>();
}


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
};

template <typename T>
struct DeduceVariantType_Helper <T, typename GEnableIf<IsPointer<T>::Result>::Result> {
private:
	static const int temp = DeduceBasicVariantType<typename RemoveConstVolatile<typename RemovePointer<T>::Result>::Result>::Result;
public:
	static const GVariantType Result = static_cast<const GVariantType>(temp | byPointer);
};

template <typename T>
struct DeduceVariantType_Helper <T, typename GEnableIf<IsReference<T>::Result>::Result> {
private:
	static const int temp = DeduceBasicVariantType<typename RemoveConstVolatile<T>::Result>::Result;
public:
	static const GVariantType Result = static_cast<const GVariantType>(temp | byReference);
};

template <typename T>
struct DeduceVariantType_Helper <T, typename GEnableIf<MaybeEnum<T>::Result>::Result> {
	static const GVariantType Result = vtUnsignedInt;
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
	static const GVariantType Result = DeduceVariantType_Helper<typename ArrayToPointer<typename RemoveConstVolatile<T>::Result>::Result>::Result;
};

template <>
struct DeduceVariantType <void> {
	static const GVariantType Result = vtVoid;
};

template <typename From, typename To>
struct CastVariantSelector {
	static To cast(const From & v) {
		return static_cast<To>(v);
	}
};

#define DEF_CAST_VARIANT_SELECTOR(FromCV, ToCV) \
	template <typename From, typename To> \
	struct CastVariantSelector <FromCV From *, ToCV To *>{ \
		static ToCV To * cast(FromCV From * v) { \
			return static_cast<ToCV To *>(const_cast<ToCV From *>(v)); \
		} \
	};

DEF_CAST_VARIANT_SELECTOR(const, const)

DEF_CAST_VARIANT_SELECTOR(const, GPP_EMPTY())
DEF_CAST_VARIANT_SELECTOR(const, volatile)
DEF_CAST_VARIANT_SELECTOR(const, const volatile)

DEF_CAST_VARIANT_SELECTOR(GPP_EMPTY(), const)
DEF_CAST_VARIANT_SELECTOR(GPP_EMPTY(), volatile)
DEF_CAST_VARIANT_SELECTOR(GPP_EMPTY(), const volatile)

DEF_CAST_VARIANT_SELECTOR(volatile, GPP_EMPTY())
DEF_CAST_VARIANT_SELECTOR(volatile, volatile)
DEF_CAST_VARIANT_SELECTOR(volatile, const volatile)

DEF_CAST_VARIANT_SELECTOR(const volatile, GPP_EMPTY())
DEF_CAST_VARIANT_SELECTOR(const volatile, const)
DEF_CAST_VARIANT_SELECTOR(const volatile, volatile)
DEF_CAST_VARIANT_SELECTOR(const volatile, const volatile)

#undef DEF_CAST_VARIANT_SELECTOR



template <typename From, typename To, typename Enabled = void>
struct VariantCaster {
	enum { CanCast = false };

	static To cast(const From & v) {
		(void)v;

		failedCast();
		return *(typename RemoveReference<To>::Result *)0;
	}
};

template <typename From, typename To>
struct VariantCaster <From, To, typename GEnableIf<
		IsConvertible<From, To>::Result
			|| (MaybeEnum<From>::Result && IsInteger<To>::Result) // for enum
			|| (MaybeEnum<To>::Result && IsInteger<From>::Result) // for enum
	>::Result>
{
	enum { CanCast = true };

	static To cast(const From & v) {
		return CastVariantSelector<From, To>::cast(v);
	}
};

template <typename From, typename To, typename Enabled = void>
struct CastVariantHelper {
	enum { CanCast = VariantCaster<From, To>::CanCast };

	static To cast(const From & v) {
		return VariantCaster<From, To>::cast(v);
	}
};

template <typename From, typename To>
struct CastVariantHelper <From, To, typename GEnableIf<IsPointer<From>::Result && IsPointer<To>::Result>::Result> {
	enum { CanCast = true };

	static To cast(const From & v) {
		return (To)(v);
	}
};

template <typename From, typename To>
struct CastVariantHelper <From, To, typename GEnableIf<IsPointer<From>::Result && !IsPointer<To>::Result && IsVoid<typename RemovePointer<From>::Result>::Result>::Result> {
	enum { CanCast = true };

	static To cast(const From & v) {
		return (To)(*(typename RemoveReference<To>::Result *)(v));
	}
};

template <typename T>
void InitVariant(GVariant & v, GVariantType vt, const T & value) {
	v.data.type = static_cast<unsigned short>(vt);
	v.data.size = getVariantTypeSize(vt);

	switch(static_cast<int>(vt)) {
		case vtBool:
			v.data.valueBool = variant_internal::CastVariantHelper<T, bool>::cast(value);
			break;

		case vtChar:
			v.data.valueChar = variant_internal::CastVariantHelper<T, char>::cast(value);
			break;

		case vtWchar:
			v.data.valueWchar = variant_internal::CastVariantHelper<T, wchar_t>::cast(value);
			break;

		case vtSignedChar:
			v.data.valueSignedChar = variant_internal::CastVariantHelper<T, signed char>::cast(value);
			break;
		case vtUnsignedChar:
			v.data.valueUnsignedChar = variant_internal::CastVariantHelper<T, unsigned char>::cast(value);
			break;

		case vtSignedShort:
			v.data.valueSignedShort = variant_internal::CastVariantHelper<T, signed short>::cast(value);
			break;
		case vtUnsignedShort:
			v.data.valueUnsignedShort = variant_internal::CastVariantHelper<T, unsigned short>::cast(value);
			break;

		case vtSignedInt:
			v.data.valueSignedInt = variant_internal::CastVariantHelper<T, signed int>::cast(value);
			break;
		case vtUnsignedInt:
			v.data.valueUnsignedInt = variant_internal::CastVariantHelper<T, unsigned int>::cast(value);
			break;

		case vtSignedLong:
			v.data.valueSignedLong = variant_internal::CastVariantHelper<T, signed long>::cast(value);
			break;
		case vtUnsignedLong:
			v.data.valueUnsignedLong = variant_internal::CastVariantHelper<T, unsigned long>::cast(value);
			break;

		case vtSignedLongLong:
			v.data.valueSignedLongLong = variant_internal::CastVariantHelper<T, signed long long>::cast(value);
			break;
		case vtUnsignedLongLong:
			v.data.valueUnsignedLongLong = variant_internal::CastVariantHelper<T, unsigned long long>::cast(value);
			break;

		case vtFloat:
			v.data.valueFloat = variant_internal::CastVariantHelper<T, float>::cast(value);
			break;
		case vtDouble:
			v.data.valueDouble = variant_internal::CastVariantHelper<T, double>::cast(value);
			break;
		case vtLongDouble:
			v.data.valueLongDouble = variant_internal::CastVariantHelper<T, long double>::cast(value);
			break;

		case vtPointer:
			v.data.valuePointer = variant_internal::CastVariantHelper<T, const volatile void *>::cast(value);
			break;

		case vtObject:
			v.data.valueObject = variant_internal::CastVariantHelper<T *, const volatile void *>::cast(const_cast<T *>(&value));
			break;

		case vtShadow:
			v.data.shadowObject = new variant_internal::GVariantShadowObject<T>(value);
			break;


		case vtBool | byPointer:
			v.data.ptrBool = variant_internal::CastVariantHelper<T, bool *>::cast(value);
			break;

		case vtChar | byPointer:
			v.data.ptrChar = variant_internal::CastVariantHelper<T, char *>::cast(value);
			break;

		case vtWchar | byPointer:
			v.data.ptrWchar = variant_internal::CastVariantHelper<T, wchar_t *>::cast(value);
			break;

		case vtSignedChar | byPointer:
			v.data.ptrSignedChar = variant_internal::CastVariantHelper<T, signed char *>::cast(value);
			break;
		case vtUnsignedChar | byPointer:
			v.data.ptrUnsignedChar = variant_internal::CastVariantHelper<T, unsigned char *>::cast(value);
			break;

		case vtSignedShort | byPointer:
			v.data.ptrSignedShort = variant_internal::CastVariantHelper<T, signed short *>::cast(value);
			break;
		case vtUnsignedShort | byPointer:
			v.data.ptrUnsignedShort = variant_internal::CastVariantHelper<T, unsigned short *>::cast(value);
			break;

		case vtSignedInt | byPointer:
			v.data.ptrSignedInt = variant_internal::CastVariantHelper<T, signed int *>::cast(value);
			break;
		case vtUnsignedInt | byPointer:
			v.data.ptrUnsignedInt = variant_internal::CastVariantHelper<T, unsigned int *>::cast(value);
			break;

		case vtSignedLong | byPointer:
			v.data.ptrSignedLong = variant_internal::CastVariantHelper<T, signed long *>::cast(value);
			break;
		case vtUnsignedLong | byPointer:
			v.data.ptrUnsignedLong = variant_internal::CastVariantHelper<T, unsigned long *>::cast(value);
			break;

		case vtSignedLongLong | byPointer:
			v.data.ptrSignedLongLong = variant_internal::CastVariantHelper<T, signed long long *>::cast(value);
			break;
		case vtUnsignedLongLong | byPointer:
			v.data.ptrUnsignedLongLong = variant_internal::CastVariantHelper<T, unsigned long long *>::cast(value);
			break;

		case vtFloat | byPointer:
			v.data.ptrFloat = variant_internal::CastVariantHelper<T, float *>::cast(value);
			break;
		case vtDouble | byPointer:
			v.data.ptrDouble = variant_internal::CastVariantHelper<T, double *>::cast(value);
			break;
		case vtLongDouble | byPointer:
			v.data.ptrLongDouble = variant_internal::CastVariantHelper<T, long double *>::cast(value);
			break;

		case vtVoid | byPointer:
			v.data.ptrPointer = variant_internal::CastVariantHelper<T, const volatile void *>::cast(value);
			break;

		case vtPointer | byPointer:
			v.data.ptrPointer = variant_internal::CastVariantHelper<T, const volatile void *>::cast(value);
			break;

		case vtObject | byPointer:
			v.data.ptrObject = variant_internal::CastVariantHelper<T, const volatile void *>::cast(value);
			break;


		case vtBool | byReference:
			v.data.ptrBool = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile bool *>::cast(&value);
			break;

		case vtChar | byReference:
			v.data.ptrChar = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile char *>::cast(&value);
			break;

		case vtWchar | byReference:
			v.data.ptrWchar = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile wchar_t *>::cast(&value);
			break;

		case vtSignedChar | byReference:
			v.data.ptrSignedChar = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile signed char *>::cast(&value);
			break;
		case vtUnsignedChar | byReference:
			v.data.ptrUnsignedChar = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile unsigned char *>::cast(&value);
			break;

		case vtSignedShort | byReference:
			v.data.ptrSignedShort = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile signed short *>::cast(&value);
			break;
		case vtUnsignedShort | byReference:
			v.data.ptrUnsignedShort = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile unsigned short *>::cast(&value);
			break;

		case vtSignedInt | byReference:
			v.data.ptrSignedInt = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile signed int *>::cast(&value);
			break;
		case vtUnsignedInt | byReference:
			v.data.ptrUnsignedInt = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile unsigned int *>::cast(&value);
			break;

		case vtSignedLong | byReference:
			v.data.ptrSignedLong = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile signed long *>::cast(&value);
			break;
		case vtUnsignedLong | byReference:
			v.data.ptrUnsignedLong = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile unsigned long *>::cast(&value);
			break;

		case vtSignedLongLong | byReference:
			v.data.ptrSignedLongLong = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile signed long long *>::cast(&value);
			break;
		case vtUnsignedLongLong | byReference:
			v.data.ptrUnsignedLongLong = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile unsigned long long *>::cast(&value);
			break;

		case vtFloat | byReference:
			v.data.ptrFloat = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile float *>::cast(&value);
			break;
		case vtDouble | byReference:
			v.data.ptrDouble = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile double *>::cast(&value);
			break;
		case vtLongDouble | byReference:
			v.data.ptrLongDouble = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile long double *>::cast(&value);
			break;

		case vtVoid | byReference:
			v.data.ptrPointer = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile void *>::cast(&value);
			break;

		case vtPointer | byReference:
			v.data.ptrPointer = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile void *>::cast(&value);
			break;

		case vtObject | byReference:
			v.data.ptrObject = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile void *>::cast(const_cast<const volatile typename RemoveReference<T>::Result *>(&value));
			break;

		default:
			failedCast();
			break;

	}
}


template <typename T>
struct CastResult {
	typedef T Result;
};

template <typename T>
struct CastResult <T &> {
	typedef T & Result;
};

template <typename T>
struct CastResult <const T & > {
	typedef T Result;
};

template <typename T>
struct CanCastFromVariant
{
	typedef typename CastResult<T>::Result ResultType;
	typedef typename RemoveReference<T>::Result RefValueType;

	static bool canCast(int vt) {
		switch(static_cast<int>(vt)) {
			case vtBool:
				return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<bool, ResultType>::CanCast;

			case vtChar:
				return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<char, ResultType>::CanCast;

			case vtWchar:
				return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<wchar_t, ResultType>::CanCast;

			case vtSignedChar:
				return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<signed char, ResultType>::CanCast;

			case vtUnsignedChar:
				return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<unsigned char, ResultType>::CanCast;

			case vtSignedShort:
				return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<signed short, ResultType>::CanCast;

			case vtUnsignedShort:
				return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<unsigned short, ResultType>::CanCast;

			case vtSignedInt:
				return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<signed int, ResultType>::CanCast;

			case vtUnsignedInt:
				return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<unsigned int, ResultType>::CanCast;

			case vtSignedLong:
				return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<signed long, ResultType>::CanCast;

			case vtUnsignedLong:
				return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<unsigned long, ResultType>::CanCast;

			case vtSignedLongLong:
				return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<signed long long, ResultType>::CanCast;

			case vtUnsignedLongLong:
				return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<unsigned long long, ResultType>::CanCast;

			case vtFloat:
				return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<float, ResultType>::CanCast;

			case vtDouble:
				return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<double, ResultType>::CanCast;

			case vtLongDouble:
				return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<long double, ResultType>::CanCast;

			case vtPointer:
				return variant_internal::CastVariantHelper<const volatile void *, ResultType>::CanCast;

			case vtObject:
				return variant_internal::isNonPointer<ResultType>() && variant_internal::CastVariantHelper<const volatile void *, typename RemoveReference<ResultType>::Result *>::CanCast;

			case vtShadow:
				return variant_internal::CastVariantHelper<const volatile void *, typename RemoveReference<ResultType>::Result *>::CanCast;

			case vtBool | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<bool *, ResultType>::CanCast;

			case vtChar | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<char *, ResultType>::CanCast;

			case vtWchar | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<wchar_t *, ResultType>::CanCast;

			case vtSignedChar | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<signed char *, ResultType>::CanCast;

			case vtUnsignedChar | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<unsigned char *, ResultType>::CanCast;

			case vtSignedShort | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<signed short *, ResultType>::CanCast;

			case vtUnsignedShort | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<unsigned short *, ResultType>::CanCast;

			case vtSignedInt | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<signed int *, ResultType>::CanCast;

			case vtUnsignedInt | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<unsigned int *, ResultType>::CanCast;

			case vtSignedLong | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<signed long *, ResultType>::CanCast;

			case vtUnsignedLong | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<unsigned long *, ResultType>::CanCast;

			case vtSignedLongLong | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<signed long long *, ResultType>::CanCast;

			case vtUnsignedLongLong | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<unsigned long long *, ResultType>::CanCast;

			case vtFloat | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<float *, ResultType>::CanCast;

			case vtDouble | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<double *, ResultType>::CanCast;

			case vtLongDouble | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<long double *, ResultType>::CanCast;

			case vtVoid | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<void *, ResultType>::CanCast;

			case vtPointer | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<void *, ResultType>::CanCast;

			case vtObject | byPointer:
				return variant_internal::isNonPrimary<ResultType>() && variant_internal::CastVariantHelper<void *, ResultType>::CanCast;

			case vtBool | byReference:
				return variant_internal::CastVariantHelper<const volatile bool *, const volatile RefValueType *>::CanCast;

			case vtChar | byReference:
				return variant_internal::CastVariantHelper<const volatile char *, const volatile RefValueType *>::CanCast;

			case vtWchar | byReference:
				return variant_internal::CastVariantHelper<const volatile wchar_t *, const volatile RefValueType *>::CanCast;

			case vtSignedChar | byReference:
				return variant_internal::CastVariantHelper<const volatile signed char *, const volatile RefValueType *>::CanCast;

			case vtUnsignedChar | byReference:
				return variant_internal::CastVariantHelper<const volatile unsigned char *, const volatile RefValueType *>::CanCast;

			case vtSignedShort | byReference:
				return variant_internal::CastVariantHelper<const volatile signed short *, const volatile RefValueType *>::CanCast;

			case vtUnsignedShort | byReference:
				return variant_internal::CastVariantHelper<const volatile unsigned short *, const volatile RefValueType *>::CanCast;

			case vtSignedInt | byReference:
				return variant_internal::CastVariantHelper<const volatile signed int *, const volatile RefValueType *>::CanCast;

			case vtUnsignedInt | byReference:
				return variant_internal::CastVariantHelper<const volatile unsigned int *, const volatile RefValueType *>::CanCast;

			case vtSignedLong | byReference:
				return variant_internal::CastVariantHelper<const volatile signed long *, const volatile RefValueType *>::CanCast;

			case vtUnsignedLong | byReference:
				return variant_internal::CastVariantHelper<const volatile unsigned long *, const volatile RefValueType *>::CanCast;

			case vtSignedLongLong | byReference:
				return variant_internal::CastVariantHelper<const volatile signed long long *, const volatile RefValueType *>::CanCast;

			case vtUnsignedLongLong | byReference:
				return variant_internal::CastVariantHelper<const volatile unsigned long long *, const volatile RefValueType *>::CanCast;

			case vtFloat | byReference:
				return variant_internal::CastVariantHelper<const volatile float *, const volatile RefValueType *>::CanCast;

			case vtDouble | byReference:
				return variant_internal::CastVariantHelper<const volatile double *, const volatile RefValueType *>::CanCast;

			case vtLongDouble | byReference:
				return variant_internal::CastVariantHelper<const volatile long double *, const volatile RefValueType *>::CanCast;

			case vtVoid | byReference:
				return variant_internal::CastVariantHelper<const volatile void *, const volatile RefValueType *>::CanCast;

			case vtPointer | byReference:
				return variant_internal::CastVariantHelper<const volatile void *, const volatile RefValueType *>::CanCast;

			case vtObject | byReference:
				return variant_internal::isNonPointer<RefValueType>() && variant_internal::CastVariantHelper<const volatile void *, const volatile RefValueType *>::CanCast;
		}

		return false;
	}
};

template <typename T>
struct CastFromVariant
{
	typedef typename CastResult<T>::Result ResultType;
	typedef typename RemoveReference<T>::Result RefValueType;

	static ResultType cast(const GVariant & v) {
		checkFailCast(canFromVariant<T>(v));

		switch(static_cast<int>(v.data.type)) {
			case vtBool:
				return variant_internal::CastVariantHelper<bool, ResultType>::cast(v.data.valueBool);

			case vtChar:
				return variant_internal::CastVariantHelper<char, ResultType>::cast(v.data.valueChar);

			case vtWchar:
				return variant_internal::CastVariantHelper<wchar_t, ResultType>::cast(v.data.valueWchar);

			case vtSignedChar:
				return variant_internal::CastVariantHelper<signed char, ResultType>::cast(v.data.valueSignedChar);

			case vtUnsignedChar:
				return variant_internal::CastVariantHelper<unsigned char, ResultType>::cast(v.data.valueUnsignedChar);

			case vtSignedShort:
				return variant_internal::CastVariantHelper<signed short, ResultType>::cast(v.data.valueSignedShort);

			case vtUnsignedShort:
				return variant_internal::CastVariantHelper<unsigned short, ResultType>::cast(v.data.valueUnsignedShort);

			case vtSignedInt:
				return variant_internal::CastVariantHelper<signed int, ResultType>::cast(v.data.valueSignedInt);

			case vtUnsignedInt:
				return variant_internal::CastVariantHelper<unsigned int, ResultType>::cast(v.data.valueUnsignedInt);

			case vtSignedLong:
				return variant_internal::CastVariantHelper<signed long, ResultType>::cast(v.data.valueSignedLong);

			case vtUnsignedLong:
				return variant_internal::CastVariantHelper<unsigned long, ResultType>::cast(v.data.valueUnsignedLong);

			case vtSignedLongLong:
				return variant_internal::CastVariantHelper<signed long long, ResultType>::cast(v.data.valueSignedLongLong);

			case vtUnsignedLongLong:
				return variant_internal::CastVariantHelper<unsigned long long, ResultType>::cast(v.data.valueUnsignedLongLong);

			case vtFloat:
				return variant_internal::CastVariantHelper<float, ResultType>::cast(v.data.valueFloat);

			case vtDouble:
				return variant_internal::CastVariantHelper<double, ResultType>::cast(v.data.valueDouble);

			case vtLongDouble:
				return variant_internal::CastVariantHelper<long double, ResultType>::cast(v.data.valueLongDouble);

			case vtPointer:
				return variant_internal::CastVariantHelper<const volatile void *, ResultType>::cast(v.data.valuePointer);

			case vtObject:
				return *variant_internal::CastVariantHelper<const volatile void *, typename RemoveReference<ResultType>::Result *>::cast(v.data.valueObject);

			case vtShadow:
				return *variant_internal::CastVariantHelper<const volatile void *, typename RemoveReference<ResultType>::Result *>::cast(v.data.shadowObject->getObject());

			case vtBool | byPointer:
				return variant_internal::CastVariantHelper<bool *, ResultType>::cast(const_cast<bool *>(v.data.ptrBool));

			case vtChar | byPointer:
				return variant_internal::CastVariantHelper<char *, ResultType>::cast(const_cast<char *>(v.data.ptrChar));

			case vtWchar | byPointer:
				return variant_internal::CastVariantHelper<wchar_t *, ResultType>::cast(const_cast<wchar_t *>(v.data.ptrWchar));

			case vtSignedChar | byPointer:
				return variant_internal::CastVariantHelper<signed char *, ResultType>::cast(const_cast<signed char *>(v.data.ptrSignedChar));

			case vtUnsignedChar | byPointer:
				return variant_internal::CastVariantHelper<unsigned char *, ResultType>::cast(const_cast<unsigned char *>(v.data.ptrUnsignedChar));

			case vtSignedShort | byPointer:
				return variant_internal::CastVariantHelper<signed short *, ResultType>::cast(const_cast<signed short *>(v.data.ptrSignedShort));

			case vtUnsignedShort | byPointer:
				return variant_internal::CastVariantHelper<unsigned short *, ResultType>::cast(const_cast<unsigned short *>(v.data.ptrUnsignedShort));

			case vtSignedInt | byPointer:
				return variant_internal::CastVariantHelper<signed int *, ResultType>::cast(const_cast<signed int *>(v.data.ptrSignedInt));

			case vtUnsignedInt | byPointer:
				return variant_internal::CastVariantHelper<unsigned int *, ResultType>::cast(const_cast<unsigned int *>(v.data.ptrUnsignedInt));

			case vtSignedLong | byPointer:
				return variant_internal::CastVariantHelper<signed long *, ResultType>::cast(const_cast<signed long *>(v.data.ptrSignedLong));

			case vtUnsignedLong | byPointer:
				return variant_internal::CastVariantHelper<unsigned long *, ResultType>::cast(const_cast<unsigned long *>(v.data.ptrUnsignedLong));

			case vtSignedLongLong | byPointer:
				return variant_internal::CastVariantHelper<signed long long *, ResultType>::cast(const_cast<signed long long *>(v.data.ptrSignedLongLong));

			case vtUnsignedLongLong | byPointer:
				return variant_internal::CastVariantHelper<unsigned long long *, ResultType>::cast(const_cast<unsigned long long *>(v.data.ptrUnsignedLongLong));

			case vtFloat | byPointer:
				return variant_internal::CastVariantHelper<float *, ResultType>::cast(const_cast<float *>(v.data.ptrFloat));

			case vtDouble | byPointer:
				return variant_internal::CastVariantHelper<double *, ResultType>::cast(const_cast<double *>(v.data.ptrDouble));

			case vtLongDouble | byPointer:
				return variant_internal::CastVariantHelper<long double *, ResultType>::cast(const_cast<long double *>(v.data.ptrLongDouble));

			case vtVoid | byPointer:
				return variant_internal::CastVariantHelper<void *, ResultType>::cast(const_cast<void *>(v.data.ptrPointer));

			case vtPointer | byPointer:
				return variant_internal::CastVariantHelper<void *, ResultType>::cast(const_cast<void *>(v.data.ptrPointer));

			case vtObject | byPointer:
				return variant_internal::CastVariantHelper<void *, ResultType>::cast(const_cast<void *>(v.data.ptrObject));

			case vtBool | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile bool *, const volatile RefValueType *>::cast(v.data.ptrBool));

			case vtChar | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile char *, const volatile RefValueType *>::cast(v.data.ptrChar));

			case vtWchar | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile wchar_t *, const volatile RefValueType *>::cast(v.data.ptrWchar));

			case vtSignedChar | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile signed char *, const volatile RefValueType *>::cast(v.data.ptrSignedChar));

			case vtUnsignedChar | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile unsigned char *, const volatile RefValueType *>::cast(v.data.ptrUnsignedChar));

			case vtSignedShort | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile signed short *, const volatile RefValueType *>::cast(v.data.ptrSignedShort));

			case vtUnsignedShort | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile unsigned short *, const volatile RefValueType *>::cast(v.data.ptrUnsignedShort));

			case vtSignedInt | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile signed int *, const volatile RefValueType *>::cast(v.data.ptrSignedInt));

			case vtUnsignedInt | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile unsigned int *, const volatile RefValueType *>::cast(v.data.ptrUnsignedInt));

			case vtSignedLong | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile signed long *, const volatile RefValueType *>::cast(v.data.ptrSignedLong));

			case vtUnsignedLong | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile unsigned long *, const volatile RefValueType *>::cast(v.data.ptrUnsignedLong));

			case vtSignedLongLong | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile signed long long *, const volatile RefValueType *>::cast(v.data.ptrSignedLongLong));

			case vtUnsignedLongLong | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile unsigned long long *, const volatile RefValueType *>::cast(v.data.ptrUnsignedLongLong));

			case vtFloat | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile float *, const volatile RefValueType *>::cast(v.data.ptrFloat));

			case vtDouble | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile double *, const volatile RefValueType *>::cast(v.data.ptrDouble));

			case vtLongDouble | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile long double *, const volatile RefValueType *>::cast(v.data.ptrLongDouble));

			case vtVoid | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile void *, const volatile RefValueType *>::cast(v.data.ptrPointer));

			case vtPointer | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile void *, const volatile RefValueType *>::cast(v.data.ptrPointer));

			case vtObject | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile void *, const volatile RefValueType *>::cast(v.data.ptrObject));
		}

		failedCast();
		return *(typename RemoveReference<T>::Result *)0;
	}
};

inline int getVariantTypeSize(GVariantType type)
{
	switch(static_cast<int>(type)) {
		case vtEmpty:
			return 0;

		case vtBool:
			return sizeof(bool);

		case vtChar:
			return sizeof(char);

		case vtWchar:
			return sizeof(wchar_t);

		case vtSignedChar:
			return sizeof(signed char);

		case vtUnsignedChar:
			return sizeof(unsigned char);

		case vtSignedShort:
			return sizeof(signed short);

		case vtUnsignedShort:
			return sizeof(unsigned short);

		case vtSignedInt:
			return sizeof(signed int);

		case vtUnsignedInt:
			return sizeof(unsigned int);

		case vtSignedLong:
			return sizeof(signed long);

		case vtUnsignedLong:
			return sizeof(unsigned long);

		case vtSignedLongLong:
			return sizeof(signed long long);

		case vtUnsignedLongLong:
			return sizeof(unsigned long long);

		case vtFloat:
			return sizeof(float);

		case vtDouble:
			return sizeof(double);

		case vtLongDouble:
			return sizeof(long double);

		case vtShadow:
		case vtObject:
		case vtPointer:
			return sizeof(void *);

		default:
			if(vtIsByPointer(type) || vtIsByReference(type)) {
				return sizeof(void *);
			}

	}

	GASSERT_MSG(false, "Can't detect variant type size.");

	return 0;
}

inline void adjustVariantType(GVariant * var)
{
	if(! vtIsInteger(var->data.type) || var->data.size > sizeof(unsigned long long)) {
		throw GVariantException("GVariant: inconsistent type.");
	}

	unsigned long long value = var->data.valueUnsignedLongLong;
	switch(var->data.size) {
		case 1:
			value &= 0xff;
			break;

		case 2:
			value &= 0xffff;
			break;

		case 4:
			value &= 0xffffffffu;
			break;

		case 8:
			value &= 0xffffffffffffffffull;
			break;

		default:
			break;

	}

	InitVariant(*var, var->getType(), value);
}



} // namespace variant_internal



template <typename T>
bool canFromVariant(const GVariant & v)
{
	return variant_internal::CanCastFromVariant<typename RemoveConstVolatile<T>::Result>::canCast(v.data.type);
}

template <typename T>
typename variant_internal::CastFromVariant<T>::ResultType fromVariant(const GVariant & v)
{
	return variant_internal::CastFromVariant<typename RemoveConstVolatile<T>::Result>::cast(v);
}

template <typename T>
GVariant pointerToRefVariant(T * p)
{
	GVariant v(p);

	if(vtIsByPointer(v.getType())) {
		v.data.type = (v.data.type & ~byPointer) | byReference;
	}

	return v;
}

inline GVariant pointerToRefVariant(const GVariant & p)
{
	GVariant v(p);

	if(vtIsByPointer(v.getType())) {
		v.data.type = (v.data.type & ~byPointer) | byReference;
	}

	return v;
}

inline GVariant pointerToObjectVariant(void * p)
{
	GVariant result;

	result.data.type = vtObject | byPointer;
	result.data.ptrObject = p;
	result.data.size = sizeof(void *);

	return result;
}

inline void * objectAddressFromVariant(const GVariant & v)
{
	if(v.getType() == vtShadow) {
		return v.data.shadowObject->getObject();
	}

	return fromVariant<void *>(v);
}

inline void * referenceAddressFromVariant(const GVariant & v)
{
	if(vtIsByReference(v.getType())) {
		return const_cast<void *>(v.data.ptrObject);
	}
	else {
		return objectAddressFromVariant(v);
	}
}

template <typename T>
GVariantType deduceVariantType(bool allowShadow)
{
	GVariantType vt = variant_internal::DeduceVariantType<T>::Result;

	if(allowShadow && vt == vtObject) {
		vt = vtShadow;
	}

	return vt;
}

template <typename T>
GVariantType deduceVariantType()
{
	return deduceVariantType<T>(false);
}

inline void initializeVarData(GVarData * data)
{
	data->type = vtEmpty;
}



} // namespace cpgf


#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif

