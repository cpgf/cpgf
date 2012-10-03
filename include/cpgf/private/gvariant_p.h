#ifndef __GVARIANT_P_H
#define __GVARIANT_P_H

namespace variant_internal {

unsigned int getVariantTypeSize(GVariantType type);
void adjustVariantType(GVariant * var);

struct IVariantShadowObject : public IObject
{
public:
	virtual void * G_API_CC getObject() = 0;
};

template <typename T>
class GVariantShadowObject : public IVariantShadowObject
{
private:
	typedef GVariantShadowObject<T> ThisType;

	G_INTERFACE_IMPL_OBJECT

protected:
	virtual void * G_API_CC getObject() {
		return (void *)(&this->obj);
	}

public:
	GVariantShadowObject(const T & obj) : obj(obj) {
	}

private:
	T obj;
};


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
private:
	typedef typename GIfElse<IsEnum<To>::Result, long long, To>::Result U;
public:
	static To cast(const From & v) {
		return (To)((U)(const_cast<From &>(v)));
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

	static To cast(const From & /*v*/) {
		failedCast();
		return *(typename RemoveReference<To>::Result *)0xffffff;
	}
};

template <typename From, typename To>
struct VariantCaster <From, To, typename GEnableIfResult<
	GOrResult<
		IsConvertible<From, To>,
		GAndResult<IsEnum<From>, IsConvertible<To, int> >, // for enum
		GAndResult<IsEnum<To>, IsConvertible<From, int> > // for enum
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
struct CastVariantHelper <From *, To *>
{
	G_STATIC_CONSTANT(bool, CanCast = true);

	static To * cast(From * v) {
		return (To *)(v);
	}
};

template <typename From, typename To>
struct CastVariantHelper <From, To, typename GEnableIfResult<
	GAndResult<
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
	GAndResult<
		IsReference<To>,
		GNotResult<IsReference<From> >,
		GNotResult<IsPointer<From> >
	>
	>::Result
	>
{
	G_STATIC_CONSTANT(bool, CanCast = false);

	static To cast(const From & /*v*/) {
		raiseCoreException(Error_Variant_CantReferenceToTemp);
		return *(typename RemoveReference<To>::Result *)0xffffff;
	}
};

template <typename From, typename To>
struct CastVariantHelper <From, To *, typename GEnableIfResult<IsInteger<From> >::Result>
{
	G_STATIC_CONSTANT(bool, CanCast = true);

	static To * cast(const From & v) {
		if(v == 0) {
			return 0;
		}
		return VariantCaster<From, To *>::cast(v);
	}
};

template <typename From, typename To>
struct EnforceCastToPointer
{
	G_STATIC_CONSTANT(bool, CanCast = false);

	static typename AddReference<To>::Result cast(const From & /*v*/) {
		raiseCoreException(Error_Variant_FailCast);
		return *(typename RemoveReference<To>::Result *)0xffffff;
	}
};

template <typename From, typename To>
struct EnforceCastToPointer <From, To *> : public CastVariantHelper<From, To *> {};
template <typename From, typename To>
struct EnforceCastToPointer <From, const To *> : public CastVariantHelper<From, const To *> {};
template <typename From, typename To>
struct EnforceCastToPointer <From, volatile To *> : public CastVariantHelper<From, volatile To *> {};
template <typename From, typename To>
struct EnforceCastToPointer <From, const volatile To *> : public CastVariantHelper<From, const volatile To *> {};

template <bool Copyable, typename T>
void initShadowObject(GVariant & v, const T & value, typename GEnableIf<Copyable>::Result * = 0)
{
	v.refData().shadowObject = new variant_internal::GVariantShadowObject<T>(value);
}

template <bool Copyable, typename T>
void initShadowObject(GVariant & /*v*/, const T & /*value*/, typename GDisableIf<Copyable>::Result * = 0)
{
	raiseCoreException(Error_Variant_FailCopyObject);
}

template <bool Copyable, typename T, typename Enable = void>
struct InitVariantSelector
{
	static void init(GVariant & v, const GVarTypeData & typeData, const T & value) {
		v.refData().typeData = typeData;
		vtSetSize(v.refData().typeData, getVariantTypeSize(vtGetType(typeData)));

		switch(static_cast<int>(vtGetType(typeData))) {
			case vtBool:
				// convert to long long then convert to bool because C++ allows bool to be convert to int
				// and that may cause ambiguous for a class that has cast operator to int, pointer, but no cast to bool.
				v.refData().valueBool = (bool)variant_internal::CastVariantHelper<T, long long>::cast(value);
				break;

			case vtChar:
				v.refData().valueChar = variant_internal::CastVariantHelper<T, char>::cast(value);
				break;

			case vtWchar:
				v.refData().valueWchar = variant_internal::CastVariantHelper<T, wchar_t>::cast(value);
				break;

			case vtSignedChar:
				v.refData().valueSignedChar = variant_internal::CastVariantHelper<T, signed char>::cast(value);
				break;
			case vtUnsignedChar:
				v.refData().valueUnsignedChar = variant_internal::CastVariantHelper<T, unsigned char>::cast(value);
				break;

			case vtSignedShort:
				v.refData().valueSignedShort = variant_internal::CastVariantHelper<T, signed short>::cast(value);
				break;
			case vtUnsignedShort:
				v.refData().valueUnsignedShort = variant_internal::CastVariantHelper<T, unsigned short>::cast(value);
				break;

			case vtSignedInt:
				v.refData().valueSignedInt = variant_internal::CastVariantHelper<T, signed int>::cast(value);
				break;
			case vtUnsignedInt:
				v.refData().valueUnsignedInt = variant_internal::CastVariantHelper<T, unsigned int>::cast(value);
				break;

			case vtSignedLong:
				v.refData().valueSignedLong = variant_internal::CastVariantHelper<T, signed long>::cast(value);
				break;
			case vtUnsignedLong:
				v.refData().valueUnsignedLong = variant_internal::CastVariantHelper<T, unsigned long>::cast(value);
				break;

			case vtSignedLongLong:
				v.refData().valueSignedLongLong = variant_internal::CastVariantHelper<T, signed long long>::cast(value);
				break;
			case vtUnsignedLongLong:
				v.refData().valueUnsignedLongLong = variant_internal::CastVariantHelper<T, unsigned long long>::cast(value);
				break;

			case vtFloat:
				v.refData().valueFloat = variant_internal::CastVariantHelper<T, float>::cast(value);
				break;
			case vtDouble:
				v.refData().valueDouble = variant_internal::CastVariantHelper<T, double>::cast(value);
				break;
			case vtLongDouble:
				v.refData().valueLongDouble = variant_internal::CastVariantHelper<T, long double>::cast(value);
				break;

			case vtPointer:
				v.refData().valuePointer = variant_internal::CastVariantHelper<T, const volatile void *>::cast(value);
				break;

			case vtObject:
				v.refData().valueObject = variant_internal::CastVariantHelper<T *, const volatile void *>::cast(const_cast<T *>(&value));
				break;

			case vtShadow:
				initShadowObject<Copyable && !IsArray<T>::Result && !IsReference<T>::Result>(v, value);
				break;

			case vtInterface:
				v.refData().valueInterface = variant_internal::CastVariantHelper<T, IObject *>::cast(value);
				if(v.refData().valueInterface != NULL) {
					v.refData().valueInterface->addReference();
				}
				break;

			case vtTypedVar:
				v.refData().valueTypedVar = variant_internal::CastVariantHelper<T, variant_internal::IVariantTypedVar *>::cast(value);
				break;

			case vtBool | byPointer:
				v.refData().ptrBool = variant_internal::CastVariantHelper<T, bool *>::cast(value);
				break;

			case vtChar | byPointer:
				v.refData().ptrChar = variant_internal::CastVariantHelper<T, char *>::cast(value);
				break;

			case vtWchar | byPointer:
				v.refData().ptrWchar = variant_internal::CastVariantHelper<T, wchar_t *>::cast(value);
				break;

			case vtSignedChar | byPointer:
				v.refData().ptrSignedChar = variant_internal::CastVariantHelper<T, signed char *>::cast(value);
				break;
			case vtUnsignedChar | byPointer:
				v.refData().ptrUnsignedChar = variant_internal::CastVariantHelper<T, unsigned char *>::cast(value);
				break;

			case vtSignedShort | byPointer:
				v.refData().ptrSignedShort = variant_internal::CastVariantHelper<T, signed short *>::cast(value);
				break;
			case vtUnsignedShort | byPointer:
				v.refData().ptrUnsignedShort = variant_internal::CastVariantHelper<T, unsigned short *>::cast(value);
				break;

			case vtSignedInt | byPointer:
				v.refData().ptrSignedInt = variant_internal::CastVariantHelper<T, signed int *>::cast(value);
				break;
			case vtUnsignedInt | byPointer:
				v.refData().ptrUnsignedInt = variant_internal::CastVariantHelper<T, unsigned int *>::cast(value);
				break;

			case vtSignedLong | byPointer:
				v.refData().ptrSignedLong = variant_internal::CastVariantHelper<T, signed long *>::cast(value);
				break;
			case vtUnsignedLong | byPointer:
				v.refData().ptrUnsignedLong = variant_internal::CastVariantHelper<T, unsigned long *>::cast(value);
				break;

			case vtSignedLongLong | byPointer:
				v.refData().ptrSignedLongLong = variant_internal::CastVariantHelper<T, signed long long *>::cast(value);
				break;
			case vtUnsignedLongLong | byPointer:
				v.refData().ptrUnsignedLongLong = variant_internal::CastVariantHelper<T, unsigned long long *>::cast(value);
				break;

			case vtFloat | byPointer:
				v.refData().ptrFloat = variant_internal::CastVariantHelper<T, float *>::cast(value);
				break;
			case vtDouble | byPointer:
				v.refData().ptrDouble = variant_internal::CastVariantHelper<T, double *>::cast(value);
				break;
			case vtLongDouble | byPointer:
				v.refData().ptrLongDouble = variant_internal::CastVariantHelper<T, long double *>::cast(value);
				break;

			case vtVoid | byPointer:
				v.refData().ptrPointer = variant_internal::CastVariantHelper<T, const volatile void *>::cast(value);
				break;

			case vtPointer | byPointer:
				v.refData().ptrPointer = variant_internal::CastVariantHelper<T, const volatile void *>::cast(value);
				break;

			case vtObject | byPointer:
				v.refData().ptrObject = variant_internal::CastVariantHelper<T, const volatile void *>::cast(value);
				break;


			case vtBool | byReference:
				v.refData().ptrBool = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile bool *>::cast(&value);
				break;

			case vtChar | byReference:
				v.refData().ptrChar = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile char *>::cast(&value);
				break;

			case vtWchar | byReference:
				v.refData().ptrWchar = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile wchar_t *>::cast(&value);
				break;

			case vtSignedChar | byReference:
				v.refData().ptrSignedChar = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile signed char *>::cast(&value);
				break;
			case vtUnsignedChar | byReference:
				v.refData().ptrUnsignedChar = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile unsigned char *>::cast(&value);
				break;

			case vtSignedShort | byReference:
				v.refData().ptrSignedShort = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile signed short *>::cast(&value);
				break;
			case vtUnsignedShort | byReference:
				v.refData().ptrUnsignedShort = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile unsigned short *>::cast(&value);
				break;

			case vtSignedInt | byReference:
				v.refData().ptrSignedInt = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile signed int *>::cast(&value);
				break;
			case vtUnsignedInt | byReference:
				v.refData().ptrUnsignedInt = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile unsigned int *>::cast(&value);
				break;

			case vtSignedLong | byReference:
				v.refData().ptrSignedLong = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile signed long *>::cast(&value);
				break;
			case vtUnsignedLong | byReference:
				v.refData().ptrUnsignedLong = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile unsigned long *>::cast(&value);
				break;

			case vtSignedLongLong | byReference:
				v.refData().ptrSignedLongLong = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile signed long long *>::cast(&value);
				break;
			case vtUnsignedLongLong | byReference:
				v.refData().ptrUnsignedLongLong = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile unsigned long long *>::cast(&value);
				break;

			case vtFloat | byReference:
				v.refData().ptrFloat = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile float *>::cast(&value);
				break;
			case vtDouble | byReference:
				v.refData().ptrDouble = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile double *>::cast(&value);
				break;
			case vtLongDouble | byReference:
				v.refData().ptrLongDouble = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile long double *>::cast(&value);
				break;

			case vtVoid | byReference:
				v.refData().ptrPointer = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile void *>::cast(&value);
				break;

			case vtPointer | byReference:
				v.refData().ptrPointer = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile void *>::cast(&value);
				break;

			case vtObject | byReference:
				v.refData().ptrObject = variant_internal::CastVariantHelper<const volatile typename RemoveReference<T>::Result *, const volatile void *>::cast(const_cast<const volatile typename RemoveReference<T>::Result *>(&value));
				break;

			default:
				failedCast();
				break;

		}
	}
};

template <bool Copyable, typename T>
struct InitVariantSelector <Copyable, T, typename GEnableIfResult<IsSameType<T, GVariant> >::Result>
{
	static void init(GVariant & v, const GVarTypeData & /*typeData*/, const T & value) {
		v = value;
	}
};

template <bool Copyable, typename T>
void InitVariant(GVariant & v, const GVarTypeData & typeData, const typename RemoveReference<T>::Result & value)
{
	InitVariantSelector<Copyable && !IsReference<T>::Result, typename RemoveReference<T>::Result>::init(v, typeData, value);
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

	static bool canCast(const GVariant & v) {
		(void)v;

		switch(static_cast<int>(vtGetType(v.refData().typeData))) {
			case vtBool:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<bool, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueBool == 0)
				;

			case vtChar:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<char, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueChar == 0)
				;

			case vtWchar:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<wchar_t, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueWchar == 0)
				;

			case vtSignedChar:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<signed char, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueSignedChar == 0)
				;

			case vtUnsignedChar:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<unsigned char, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueUnsignedChar == 0)
				;

			case vtSignedShort:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<signed short, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueSignedShort == 0)
				;

			case vtUnsignedShort:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<unsigned short, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueUnsignedShort == 0)
				;

			case vtSignedInt:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<signed int, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueSignedInt == 0)
				;

			case vtUnsignedInt:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<unsigned int, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueUnsignedInt == 0)
				;

			case vtSignedLong:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<signed long, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueSignedLong == 0)
				;

			case vtUnsignedLong:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<unsigned long, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueUnsignedLong == 0)
				;

			case vtSignedLongLong:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<signed long long, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueSignedLongLong == 0)
				;

			case vtUnsignedLongLong:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CastVariantHelper<unsigned long long, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueUnsignedLongLong == 0)
				;

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
				return IsConvertible<char *, ResultType>::Result || IsConvertible<const char *, ResultType>::Result;

			case vtWideString:
				return IsConvertible<wchar_t *, ResultType>::Result || IsConvertible<const wchar_t *, ResultType>::Result;

			case vtInterface:
				return variant_internal::EnforceCastToPointer<IObject *, typename RemoveReference<ResultType>::Result>::CanCast;
			
			case vtTypedVar:
				return CanCastFromVariant<T, Policy>::canCast(getVariantRealValue(v));

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
T castFromString(char * s, typename GEnableIfResult<CheckIsConvertibleToCharPointer<T> >::Result * = 0)
{
	return T(s);
}

template <typename T>
T castFromString(char * /*s*/, typename GDisableIfResult<CheckIsConvertibleToCharPointer<T> >::Result * = 0)
{
	raiseCoreException(Error_Variant_FailCast);
	return *(typename RemoveReference<T>::Result *)(0);
}

template <typename T>
struct CheckIsConvertibleToWideCharPointer
{
	G_STATIC_CONSTANT(bool, Result = (IsConvertible<wchar_t *, T>::Result || IsConvertible<const wchar_t *, T>::Result));
};

template <typename T>
T castFromWideString(wchar_t * s, typename GEnableIfResult<CheckIsConvertibleToWideCharPointer<T> >::Result * = 0)
{
	return T(s);
}

template <typename T>
T castFromWideString(wchar_t * /*s*/, typename GDisableIfResult<CheckIsConvertibleToWideCharPointer<T> >::Result * = 0)
{
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
	typedef typename variant_internal::ArrayToPointer<typename CastResult<T, Policy>::Result>::Result ResultType;
	typedef typename variant_internal::ArrayToPointer<typename RemoveReference<T>::Result>::Result RefValueType;

	static ResultType cast(const GVariant & v) {
		checkFailCast(canFromVariant<T>(v));

		switch(static_cast<int>(vtGetType(v.refData().typeData))) {
			case vtBool:
				return variant_internal::CastVariantHelper<bool, ResultType>::cast(v.refData().valueBool);

			case vtChar:
				return variant_internal::CastVariantHelper<char, ResultType>::cast(v.refData().valueChar);

			case vtWchar:
				return variant_internal::CastVariantHelper<wchar_t, ResultType>::cast(v.refData().valueWchar);

			case vtSignedChar:
				return variant_internal::CastVariantHelper<signed char, ResultType>::cast(v.refData().valueSignedChar);

			case vtUnsignedChar:
				return variant_internal::CastVariantHelper<unsigned char, ResultType>::cast(v.refData().valueUnsignedChar);

			case vtSignedShort:
				return variant_internal::CastVariantHelper<signed short, ResultType>::cast(v.refData().valueSignedShort);

			case vtUnsignedShort:
				return variant_internal::CastVariantHelper<unsigned short, ResultType>::cast(v.refData().valueUnsignedShort);

			case vtSignedInt:
				return variant_internal::CastVariantHelper<signed int, ResultType>::cast(v.refData().valueSignedInt);

			case vtUnsignedInt:
				return variant_internal::CastVariantHelper<unsigned int, ResultType>::cast(v.refData().valueUnsignedInt);

			case vtSignedLong:
				return variant_internal::CastVariantHelper<signed long, ResultType>::cast(v.refData().valueSignedLong);

			case vtUnsignedLong:
				return variant_internal::CastVariantHelper<unsigned long, ResultType>::cast(v.refData().valueUnsignedLong);

			case vtSignedLongLong:
				return variant_internal::CastVariantHelper<signed long long, ResultType>::cast(v.refData().valueSignedLongLong);

			case vtUnsignedLongLong:
				return variant_internal::CastVariantHelper<unsigned long long, ResultType>::cast(v.refData().valueUnsignedLongLong);

			case vtFloat:
				return variant_internal::CastVariantHelper<float, ResultType>::cast(v.refData().valueFloat);

			case vtDouble:
				return variant_internal::CastVariantHelper<double, ResultType>::cast(v.refData().valueDouble);

			case vtLongDouble:
				return variant_internal::CastVariantHelper<long double, ResultType>::cast(v.refData().valueLongDouble);

			case vtPointer:
				return variant_internal::CastVariantHelper<const volatile void *, ResultType>::cast(v.refData().valuePointer);

			case vtObject:
				return castFromObject<T>(v.refData().ptrObject);

			case vtShadow:
				return castFromObject<T>(v.refData().shadowObject->getObject());
			
			case vtString:
				return castFromString<ResultType>(const_cast<char *>(static_cast<std::string *>(v.refData().shadowObject->getObject())->c_str()));

			case vtWideString:
				return castFromWideString<ResultType>(const_cast<wchar_t *>(static_cast<std::wstring *>(v.refData().shadowObject->getObject())->c_str()));

			case vtInterface:
				return variant_internal::EnforceCastToPointer<IObject *, ResultType>::cast(v.refData().valueInterface);
			
			case vtTypedVar:
				return CastFromVariant<T, Policy>::cast(getVariantRealValue(v));

			case vtBool | byPointer:
				return variant_internal::CastVariantHelper<bool *, ResultType>::cast(const_cast<bool *>(v.refData().ptrBool));

			case vtChar | byPointer:
				return variant_internal::CastVariantHelper<char *, ResultType>::cast(const_cast<char *>(v.refData().ptrChar));

			case vtWchar | byPointer:
				return variant_internal::CastVariantHelper<wchar_t *, ResultType>::cast(const_cast<wchar_t *>(v.refData().ptrWchar));

			case vtSignedChar | byPointer:
				return variant_internal::CastVariantHelper<signed char *, ResultType>::cast(const_cast<signed char *>(v.refData().ptrSignedChar));

			case vtUnsignedChar | byPointer:
				return variant_internal::CastVariantHelper<unsigned char *, ResultType>::cast(const_cast<unsigned char *>(v.refData().ptrUnsignedChar));

			case vtSignedShort | byPointer:
				return variant_internal::CastVariantHelper<signed short *, ResultType>::cast(const_cast<signed short *>(v.refData().ptrSignedShort));

			case vtUnsignedShort | byPointer:
				return variant_internal::CastVariantHelper<unsigned short *, ResultType>::cast(const_cast<unsigned short *>(v.refData().ptrUnsignedShort));

			case vtSignedInt | byPointer:
				return variant_internal::CastVariantHelper<signed int *, ResultType>::cast(const_cast<signed int *>(v.refData().ptrSignedInt));

			case vtUnsignedInt | byPointer:
				return variant_internal::CastVariantHelper<unsigned int *, ResultType>::cast(const_cast<unsigned int *>(v.refData().ptrUnsignedInt));

			case vtSignedLong | byPointer:
				return variant_internal::CastVariantHelper<signed long *, ResultType>::cast(const_cast<signed long *>(v.refData().ptrSignedLong));

			case vtUnsignedLong | byPointer:
				return variant_internal::CastVariantHelper<unsigned long *, ResultType>::cast(const_cast<unsigned long *>(v.refData().ptrUnsignedLong));

			case vtSignedLongLong | byPointer:
				return variant_internal::CastVariantHelper<signed long long *, ResultType>::cast(const_cast<signed long long *>(v.refData().ptrSignedLongLong));

			case vtUnsignedLongLong | byPointer:
				return variant_internal::CastVariantHelper<unsigned long long *, ResultType>::cast(const_cast<unsigned long long *>(v.refData().ptrUnsignedLongLong));

			case vtFloat | byPointer:
				return variant_internal::CastVariantHelper<float *, ResultType>::cast(const_cast<float *>(v.refData().ptrFloat));

			case vtDouble | byPointer:
				return variant_internal::CastVariantHelper<double *, ResultType>::cast(const_cast<double *>(v.refData().ptrDouble));

			case vtLongDouble | byPointer:
				return variant_internal::CastVariantHelper<long double *, ResultType>::cast(const_cast<long double *>(v.refData().ptrLongDouble));

			case vtVoid | byPointer:
				return variant_internal::CastVariantHelper<void *, ResultType>::cast(const_cast<void *>(v.refData().ptrPointer));

			case vtPointer | byPointer:
				return variant_internal::CastVariantHelper<void *, ResultType>::cast(const_cast<void *>(v.refData().ptrPointer));

			case vtObject | byPointer:
				return variant_internal::CastVariantHelper<void *, ResultType>::cast(const_cast<void *>(v.refData().ptrObject));

			case vtBool | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile bool *, const volatile RefValueType *>::cast(v.refData().ptrBool));

			case vtChar | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile char *, const volatile RefValueType *>::cast(v.refData().ptrChar));

			case vtWchar | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile wchar_t *, const volatile RefValueType *>::cast(v.refData().ptrWchar));

			case vtSignedChar | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile signed char *, const volatile RefValueType *>::cast(v.refData().ptrSignedChar));

			case vtUnsignedChar | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile unsigned char *, const volatile RefValueType *>::cast(v.refData().ptrUnsignedChar));

			case vtSignedShort | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile signed short *, const volatile RefValueType *>::cast(v.refData().ptrSignedShort));

			case vtUnsignedShort | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile unsigned short *, const volatile RefValueType *>::cast(v.refData().ptrUnsignedShort));

			case vtSignedInt | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile signed int *, const volatile RefValueType *>::cast(v.refData().ptrSignedInt));

			case vtUnsignedInt | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile unsigned int *, const volatile RefValueType *>::cast(v.refData().ptrUnsignedInt));

			case vtSignedLong | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile signed long *, const volatile RefValueType *>::cast(v.refData().ptrSignedLong));

			case vtUnsignedLong | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile unsigned long *, const volatile RefValueType *>::cast(v.refData().ptrUnsignedLong));

			case vtSignedLongLong | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile signed long long *, const volatile RefValueType *>::cast(v.refData().ptrSignedLongLong));

			case vtUnsignedLongLong | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile unsigned long long *, const volatile RefValueType *>::cast(v.refData().ptrUnsignedLongLong));

			case vtFloat | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile float *, const volatile RefValueType *>::cast(v.refData().ptrFloat));

			case vtDouble | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile double *, const volatile RefValueType *>::cast(v.refData().ptrDouble));

			case vtLongDouble | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile long double *, const volatile RefValueType *>::cast(v.refData().ptrLongDouble));

			case vtVoid | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile void *, const volatile RefValueType *>::cast(v.refData().ptrPointer));

			case vtPointer | byReference:
				return const_cast<RefValueType &>(*variant_internal::CastVariantHelper<const volatile void *, const volatile RefValueType *>::cast(v.refData().ptrPointer));

			case vtObject | byReference:
				return castFromObject<T>(v.refData().ptrObject);
		}

		failedCast();
		return *(typename RemoveReference<T>::Result *)0xffffff;
	}
};



} // namespace variant_internal




#endif

