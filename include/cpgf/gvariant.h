#ifndef __GVARIANT_H
#define __GVARIANT_H

#include "cpgf/gvartypedata.h"
#include "cpgf/gexception.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4244)
#pragma warning(disable:4800) // warning C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
#endif

namespace cpgf {

class GVariant;

template <typename T>
bool canFromVariant(const GVariant & v);

namespace variant_internal {

template <bool CanShadow, typename T>
void InitVariant(GVariant & v, const GVarTypeData & typeData, const T & value);

template <typename T>
struct ArrayToPointer;

template <typename T, typename Enabled = void>
struct DeducePassType
{
	typedef T Result;
	typedef const typename RemoveReference<T>::Result & PassType;
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
struct GVariantData
{
	GVarTypeData typeData;

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

inline unsigned int getVariantTypeSize(GVariantType type);
inline void adjustVariantType(GVariant * data);

} // namespace variant_internal

class GMetaVariant;

class GVariant
{
//	GASSERT_STATIC(sizeof(GVariantData) == 16);

public:
	GVariant() {
		vtInit(data.typeData);
		data.holder.a = 0;
		data.holder.b = 0;
		data.holder.c = 0;
	}

	GVariant(const GVariantData & data) : data(data) {
		if(vtGetSize(this->data.typeData) != variant_internal::getVariantTypeSize(static_cast<GVariantType>(vtGetType(this->data.typeData)))) {
			variant_internal::adjustVariantType(this);
		}
	}

	template <typename T>
	GVariant(const T & value) {
		GVarTypeData typeData;
		deduceVariantType<T>(typeData);
		variant_internal::InitVariant<true>(*this, typeData, static_cast<typename variant_internal::DeducePassType<T>::PassType>(value));
	}

	template <typename T>
	GVariant(const GVarTypeData & typeData, const T & value) {
		variant_internal::InitVariant<true>(*this, typeData, value);
	}

	GVariant(const GVariant & other) : data(other.data) {
		if(vtGetType(this->data.typeData) == vtShadow) {
			this->data.shadowObject->retain();
		}
	}

	~GVariant() {
		if(vtGetType(this->data.typeData) == vtShadow) {
			this->data.shadowObject->release();
		}
	}

	GVariant & operator = (GVariant other) {
		this->swap(other);

		return *this;
	}

	void swap(GVariant & other) {
		using std::swap;

		swap(this->data.typeData, other.data.typeData);
		swap(this->data.holder, other.data.holder);
	}

	const GVarTypeData & getTypeData() const {
		return this->data.typeData;
	}

	GVariantType getType() const {
		return vtGetType(this->data.typeData);
	}

	GVariantType getBaseType() const {
		return vtGetBaseType(this->data.typeData);
	}

	int getPointers() const {
		return vtGetPointers(this->data.typeData);
	}

	bool isEmpty() const {
		return vtIsEmpty(this->getType());
	}

	GVariantData getData() const {
		return this->data;
	}

	GVariantData takeData() {
		GVariantData result = this->data;

		vtInit(this->data.typeData);

		return result;
	}

public:
	GVariantData data;

private:
	GVariant(const GMetaVariant &); //disable
};

inline void swap(GVariant & a, GVariant & b)
{
	a.swap(b);
}

inline void failedCast() {
	raiseException(Error_Variant_FailCast, "GVariant: cast failure");
}

inline void checkFailCast(bool success) {
	if(!success) {
		failedCast();
	}
}


namespace variant_internal {

template <typename T>
bool isNotPointer() {
	return ! IsPointer<typename RemoveReference<T>::Result>::Result;
}

template <typename T>
bool isNotFundamental() {
	return ! IsFundamental<typename RemoveReference<T>::Result>::Result;
}

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

template <bool CanShadow, typename T>
void initShadowObject(GVariant & v, const T & value, typename GEnableIf<CanShadow>::Result * = 0)
{
	v.data.shadowObject = new variant_internal::GVariantShadowObject<T>(value);
}

template <bool CanShadow, typename T>
void initShadowObject(GVariant & v, const T & value, typename GEnableIf<! CanShadow>::Result * = 0)
{
	raiseException(Error_Variant_FailCopyObject, "GVariant: can't create shadow object for noncopyable object.");
}

template <bool CanShadow, typename T>
void InitVariant(GVariant & v, const GVarTypeData & typeData, const T & value)
{
	v.data.typeData = typeData;
	vtSetSize(v.data.typeData, getVariantTypeSize(vtGetType(typeData)));

	switch(static_cast<int>(vtGetType(typeData))) {
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
			initShadowObject<CanShadow>(v, value);
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


template <typename T, bool KeepConstRef>
struct CastResult {
	typedef T Result;
};

template <typename T, bool KeepConstRef>
struct CastResult <T &, KeepConstRef> {
	typedef T & Result;
};

template <typename T, bool KeepConstRef>
struct CastResult <const T &, KeepConstRef> {
	typedef T Result;
};

template <typename T>
struct CastResult <const T &, false> {
	typedef T Result;
};

template <typename T>
struct CastResult <const T &, true> {
	typedef const T & Result;
};

template <typename T, bool KeepConstRef>
struct CanCastFromVariant
{
	typedef typename CastResult<T, KeepConstRef>::Result ResultType;
	typedef typename RemoveReference<T>::Result RefValueType;

	static bool canCast(int vt) {
		switch(static_cast<int>(vt)) {
			case vtBool:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<bool, ResultType>::CanCast;

			case vtChar:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<char, ResultType>::CanCast;

			case vtWchar:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<wchar_t, ResultType>::CanCast;

			case vtSignedChar:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<signed char, ResultType>::CanCast;

			case vtUnsignedChar:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<unsigned char, ResultType>::CanCast;

			case vtSignedShort:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<signed short, ResultType>::CanCast;

			case vtUnsignedShort:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<unsigned short, ResultType>::CanCast;

			case vtSignedInt:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<signed int, ResultType>::CanCast;

			case vtUnsignedInt:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<unsigned int, ResultType>::CanCast;

			case vtSignedLong:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<signed long, ResultType>::CanCast;

			case vtUnsignedLong:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<unsigned long, ResultType>::CanCast;

			case vtSignedLongLong:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<signed long long, ResultType>::CanCast;

			case vtUnsignedLongLong:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<unsigned long long, ResultType>::CanCast;

			case vtFloat:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<float, ResultType>::CanCast;

			case vtDouble:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<double, ResultType>::CanCast;

			case vtLongDouble:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<long double, ResultType>::CanCast;

			case vtPointer:
				return variant_internal::CastVariantHelper<const volatile void *, ResultType>::CanCast;

			case vtObject:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<const volatile void *, typename RemoveReference<ResultType>::Result *>::CanCast;

			case vtShadow:
				return variant_internal::CastVariantHelper<const volatile void *, typename RemoveReference<ResultType>::Result *>::CanCast;

			case vtBool | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<bool *, ResultType>::CanCast;

			case vtChar | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<char *, ResultType>::CanCast;

			case vtWchar | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<wchar_t *, ResultType>::CanCast;

			case vtSignedChar | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<signed char *, ResultType>::CanCast;

			case vtUnsignedChar | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<unsigned char *, ResultType>::CanCast;

			case vtSignedShort | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<signed short *, ResultType>::CanCast;

			case vtUnsignedShort | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<unsigned short *, ResultType>::CanCast;

			case vtSignedInt | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<signed int *, ResultType>::CanCast;

			case vtUnsignedInt | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<unsigned int *, ResultType>::CanCast;

			case vtSignedLong | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<signed long *, ResultType>::CanCast;

			case vtUnsignedLong | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<unsigned long *, ResultType>::CanCast;

			case vtSignedLongLong | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<signed long long *, ResultType>::CanCast;

			case vtUnsignedLongLong | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<unsigned long long *, ResultType>::CanCast;

			case vtFloat | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<float *, ResultType>::CanCast;

			case vtDouble | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<double *, ResultType>::CanCast;

			case vtLongDouble | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<long double *, ResultType>::CanCast;

			case vtVoid | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<void *, ResultType>::CanCast;

			case vtPointer | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<void *, ResultType>::CanCast;

			case vtObject | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CastVariantHelper<void *, ResultType>::CanCast;

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
				return variant_internal::isNotPointer<RefValueType>() && variant_internal::CastVariantHelper<const volatile void *, const volatile RefValueType *>::CanCast;
		}

		return false;
	}
};

template <typename T, bool KeepConstRef>
struct CastFromVariant
{
	typedef typename CastResult<T, KeepConstRef>::Result ResultType;
	typedef typename RemoveReference<T>::Result RefValueType;

	static ResultType cast(const GVariant & v) {
		checkFailCast(canFromVariant<T>(v));

		switch(static_cast<int>(vtGetType(v.data.typeData))) {
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

inline unsigned int getVariantTypeSize(GVariantType type)
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
	if(! vtIsInteger(vtGetType(var->data.typeData)) || vtGetSize(var->data.typeData) > sizeof(unsigned long long)) {
		raiseException(Error_Variant_FailAdjustTypeSize, "GVariant: can't adjust size for inconsistent type.");
	}

	unsigned long long value = var->data.valueUnsignedLongLong;
	switch(vtGetSize(var->data.typeData)) {
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

	InitVariant<true>(*var, var->getTypeData(), value);
}



} // namespace variant_internal


template <bool CanShadow, typename T>
GVariant createVariant(const T & value, bool allowShadow = false)
{
	GVarTypeData typeData;
	deduceVariantType<T>(typeData, allowShadow);
	GVariant v;
	variant_internal::InitVariant<CanShadow>(v, typeData, static_cast<typename variant_internal::DeducePassType<T>::PassType>(value));
	return v;
}

template <typename T>
GVariant createVariantFromCopyable(const T & value, bool allowShadow)
{
	return createVariant<true>(value, allowShadow);
}

template <typename T>
GVariant createVariantFromCopyable(const T & value)
{
	return createVariant<true>(value, false);
}

template <typename T>
bool canFromVariant(const GVariant & v)
{
	return variant_internal::CanCastFromVariant<typename RemoveConstVolatile<T>::Result, false>::canCast(vtGetType(v.data.typeData));
}

template <typename T, bool KeepConstRef>
bool canFromVariant(const GVariant & v)
{
	return variant_internal::CanCastFromVariant<typename RemoveConstVolatile<T>::Result, KeepConstRef>::canCast(vtGetType(v.data.typeData));
}

template <typename T>
typename variant_internal::CastFromVariant<T, false>::ResultType fromVariant(const GVariant & v)
{
	return variant_internal::CastFromVariant<typename RemoveConstVolatile<T>::Result, false>::cast(v);
}

template <typename T, bool KeepConstRef>
typename variant_internal::CastFromVariant<T, KeepConstRef>::ResultType fromVariant(const GVariant & v)
{
	return variant_internal::CastFromVariant<typename RemoveConstVolatile<T>::Result, KeepConstRef>::cast(v);
}

template <typename T>
GVariant pointerToRefVariant(T * p)
{
	GVariant v(p);

	if(vtIsByPointer(v.getType())) {
		vtSetType(v.data.typeData, (vtGetType(v.data.typeData) & ~byPointer) | byReference);
	}

	return v;
}

inline GVariant pointerToRefVariant(const GVariant & p)
{
	GVariant v(p);

	if(vtIsByPointer(v.getType())) {
		vtSetType(v.data.typeData, (vtGetType(v.data.typeData) & ~byPointer) | byReference);
	}

	return v;
}

inline GVariant pointerToObjectVariant(void * p)
{
	GVariant result;

	vtSetType(result.data.typeData, vtObject | byPointer);
	result.data.ptrObject = p;
	vtSetSize(result.data.typeData, sizeof(void *));

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

inline void initializeVarData(GVariantData * data)
{
	vtInit(data->typeData);
}



} // namespace cpgf


#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif

