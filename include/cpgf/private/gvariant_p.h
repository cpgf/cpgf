#ifndef __GVARIANT_P_H
#define __GVARIANT_P_H

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
		return (To)(v);
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
	G_STATIC_CONSTANT(bool, CanCast = false);

	static To cast(const From & v) {
		(void)v;

		failedCast();
		return *(typename RemoveReference<To>::Result *)0;
	}
};

template <typename From, typename To>
struct VariantCaster <From, To, typename GEnableIfResult<
	GOrResult3<
		IsConvertible<From, To>,
		GAndResult2<MaybeEnum<From>, IsInteger<To> >, // for enum
		GAndResult2<MaybeEnum<To>, IsInteger<From> > // for enum
	>
	>::Result>
{
	G_STATIC_CONSTANT(bool, CanCast = true);

	static To cast(const From & v) {
		return CastVariantSelector<From, To>::cast(v);
	}
};

template <typename From, typename To, typename Enabled = void>
struct CastVariantHelper
{
	G_STATIC_CONSTANT(bool, CanCast = (VariantCaster<From, To>::CanCast));

	static To cast(const From & v) {
		return VariantCaster<From, To>::cast(v);
	}
};

template <typename From, typename To>
struct CastVariantHelper <From, To, typename GEnableIfResult<
	GAndResult2<
		IsPointer<From>,
		IsPointer<To>
	>
	>::Result>
{
	G_STATIC_CONSTANT(bool, CanCast = true);

	static To cast(const From & v) {
		return (To)(v);
	}
};

template <typename From, typename To>
struct CastVariantHelper <From, To, typename GEnableIfResult<
	GAndResult3<
		IsPointer<From>,
		GNotResult<IsPointer<To> >,
		IsVoid<typename RemovePointer<From>::Result>
	>
	>::Result>
{
	G_STATIC_CONSTANT(bool, CanCast = true);

	static To cast(const From & v) {
		return (To)(*(typename RemoveReference<To>::Result *)(v));
	}
};

template <typename From, typename To>
struct CastVariantHelper <From, To, typename GEnableIfResult<
	GAndResult3<
		IsReference<To>,
		GNotResult<IsReference<From> >,
		GNotResult<IsPointer<From> >
	>
	>::Result
	>
{
	G_STATIC_CONSTANT(bool, CanCast = false);

	static To cast(const From & v) {
		(void)v;

		raiseCoreException(Error_Variant_CantReferenceToTemp);
		return *static_cast<typename RemoveReference<To>::Result *>(0);
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
	(void)v;
	(void)value;

	raiseCoreException(Error_Variant_FailCopyObject);
}

template <bool CanShadow, typename T, typename Enable = void>
struct InitVariantSelector
{
	typedef typename RemoveReference<T>::Result NoRef;

	static void init(GVariant & v, const GVarTypeData & typeData, const NoRef & value) {
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
				v.data.valueObject = variant_internal::CastVariantHelper<NoRef *, const volatile void *>::cast(const_cast<NoRef *>(&value));
				break;

			case vtShadow:
				initShadowObject<CanShadow>(v, value);
				break;

			case vtInterface:
				v.data.valueInterface = variant_internal::CastVariantHelper<T, IObject *>::cast(value);
				if(v.data.valueInterface != NULL) {
					v.data.valueInterface->addReference();
				}
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
};

template <bool CanShadow, typename T>
struct InitVariantSelector <CanShadow, T, typename GEnableIfResult<IsSameType<T, GVariant> >::Result>
{
	static void init(GVariant & v, const GVarTypeData & typeData, const T & value) {
		(void)typeData;
		v = value;
	}
};

template <bool CanShadow, typename T>
void InitVariant(GVariant & v, const GVarTypeData & typeData, const typename RemoveReference<T>::Result & value)
{
	InitVariantSelector<CanShadow, T>::init(v, typeData, value);
}


template <typename T, typename Policy>
struct CastResult {
	typedef T Result;
};

template <typename T, typename Policy>
struct CastResult <T &, Policy> {
	typedef T & Result;
};

template <typename T, typename Policy>
struct CastResult <const T &, Policy> {
	typedef T Result;
};

#if G_SUPPORT_RVALUE_REFERENCE
template <typename T, typename Policy>
struct CastResult <T &&, Policy> {
	typedef T Result;
};
#endif

template <typename T>
struct CastResult <const T &, VarantCastCopyConstRef> {
	typedef T Result;
};

template <typename T>
struct CastResult <const T &, VarantCastKeepConstRef> {
	typedef typename GIfElse<IsFundamental<T>::Result, T, const T &>::Result Result;
};

template <typename T, typename Policy>
struct CanCastFromVariant
{
	typedef typename CastResult<T, Policy>::Result ResultType;
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
				return variant_internal::CastVariantHelper<const volatile void *, typename RemoveReference<ResultType>::Result *>::CanCast;

			case vtShadow:
				return variant_internal::CastVariantHelper<const volatile void *, typename RemoveReference<ResultType>::Result *>::CanCast;

			case vtString:
				return IsConvertible<char *, typename RemoveReference<ResultType>::Result>::Result || IsConvertible<const char *, typename RemoveReference<ResultType>::Result>::Result;

			case vtInterface:
				return variant_internal::CastVariantHelper<IObject *, typename RemoveReference<ResultType>::Result *>::CanCast;

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
				return variant_internal::CastVariantHelper<const volatile void *, const volatile RefValueType *>::CanCast;
		}

		return false;
	}
};

template <typename T>
struct CheckIsConvertibleToCharPointer
{
	G_STATIC_CONSTANT(bool, Result = (IsConvertible<char *, T>::Result || IsConvertible<const char *, T>::Result));
};

template <typename T>
T castFromString(const char * s, typename GEnableIfResult<CheckIsConvertibleToCharPointer<T> >::Result * = 0)
{
	return T(s);
}

template <typename T>
T castFromString(const char * s, typename GDisableIfResult<CheckIsConvertibleToCharPointer<T> >::Result * = 0)
{
	(void)s;

	raiseCoreException(Error_Variant_FailCast);
	return *(typename RemoveReference<T>::Result *)(0);
}

template <typename T>
T castFromObject(const volatile void * const & obj, typename GEnableIfResult<IsPointer<typename RemoveReference<T>::Result> >::Result * = 0)
{
	return (T)(obj);
}

template <typename T>
T castFromObject(const volatile void * obj, typename GDisableIfResult<IsPointer<typename RemoveReference<T>::Result> >::Result * = 0)
{
	return (T)(*(typename RemoveReference<T>::Result *)obj);
}

template <typename T, typename Policy>
struct CastFromVariant
{
	typedef typename CastResult<T, Policy>::Result ResultType;
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
				return castFromObject<T>(v.data.ptrObject);

			case vtShadow:
				return *variant_internal::CastVariantHelper<const volatile void *, typename RemoveReference<ResultType>::Result *>::cast(v.data.shadowObject->getObject());
			
			case vtString:
				return castFromString<ResultType>(static_cast<std::string *>(v.data.shadowObject->getObject())->c_str());

			case vtInterface:
				return variant_internal::CastVariantHelper<IObject *, ResultType>::cast(v.data.valueInterface);

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
				return castFromObject<T>(v.data.ptrObject);
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
		case vtString:
		case vtInterface:
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
		raiseCoreException(Error_Variant_FailAdjustTypeSize);
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

	InitVariant<true, unsigned long long>(*var, var->getTypeData(), value);
}



} // namespace variant_internal




#endif

