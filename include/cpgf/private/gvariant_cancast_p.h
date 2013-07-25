#ifndef __GVARIANT_CANCAST_P_H
#define __GVARIANT_CANCAST_P_H


namespace variant_internal {

template <typename From, typename To, typename Enabled = void>
struct VariantCanCast {
	G_STATIC_CONSTANT(bool, CanCast = false);
};

template <typename From, typename To>
struct VariantCanCast <From, To, typename GEnableIfResult<
	GOrResult<
		IsConvertible<From, To>,
		GAndResult<IsEnum<From>, IsConvertible<To, int> >, // for enum
		GAndResult<IsEnum<To>, IsConvertible<From, int> > // for enum
	>
	>::Result>
{
	G_STATIC_CONSTANT(bool, CanCast = true);
};

template <typename From, typename To, typename Enabled = void>
struct CanCastVariantHelper
{
	G_STATIC_CONSTANT(bool, CanCast = (VariantCanCast<From, To>::CanCast));
};

template <typename From, typename To>
struct CanCastVariantHelper <From *, To *>
{
	G_STATIC_CONSTANT(bool, CanCast = true);
};

template <typename From, typename To>
struct CanCastVariantHelper <From, To, typename GEnableIfResult<
	GAndResult<
		IsPointer<From>,
		GNotResult<IsPointer<To> >,
		IsVoid<typename RemovePointer<From>::Result>
	>
	>::Result>
{
	G_STATIC_CONSTANT(bool, CanCast = true);
};

template <typename From, typename To>
struct CanCastVariantHelper <From, To, typename GEnableIfResult<
	GAndResult<
		IsReference<To>,
		GNotResult<IsReference<From> >,
		GNotResult<IsPointer<From> >
	>
	>::Result
	>
{
	G_STATIC_CONSTANT(bool, CanCast = false);
};

template <typename From, typename To>
struct CanCastVariantHelper <From, To *, typename GEnableIfResult<IsInteger<From> >::Result>
{
	G_STATIC_CONSTANT(bool, CanCast = true);
};

template <typename From, typename To>
struct EnforceCanCastToPointer
{
	G_STATIC_CONSTANT(bool, CanCast = false);
};

template <typename From, typename To>
struct EnforceCanCastToPointer <From, To *> : public CanCastVariantHelper<From, To *> {};
template <typename From, typename To>
struct EnforceCanCastToPointer <From, const To *> : public CanCastVariantHelper<From, const To *> {};
template <typename From, typename To>
struct EnforceCanCastToPointer <From, volatile To *> : public CanCastVariantHelper<From, volatile To *> {};
template <typename From, typename To>
struct EnforceCanCastToPointer <From, const volatile To *> : public CanCastVariantHelper<From, const volatile To *> {};


template <typename T, typename Policy>
struct CanCastFromVariant;

#define M_CanCastFromVariant(T) template <typename Policy> struct CanCastFromVariant <T, Policy> { \
	static bool canCast(const GVariant &) { return true; } };

M_CanCastFromVariant(GVariant)
M_CanCastFromVariant(const GVariant)
M_CanCastFromVariant(volatile GVariant)
M_CanCastFromVariant(const volatile GVariant)
M_CanCastFromVariant(GVariant &)
M_CanCastFromVariant(const GVariant &)
M_CanCastFromVariant(volatile GVariant &)
M_CanCastFromVariant(const volatile GVariant &)

#undef M_CanCastFromVariant


template <typename T, typename Policy, typename Enabled = void>
struct CanCastFromReference
{
	typedef typename variant_internal::ArrayToPointer<typename CastResult<T, Policy>::Result>::Result ResultType;
	typedef typename variant_internal::ArrayToPointer<typename RemoveReference<T>::Result>::Result RefValueType;
	
	static bool canCast(const GVariant & v) {
		int vt = static_cast<int>(vtGetType(v.refData().typeData));
		switch(vt) {
			case vtBool | byReference:
				return variant_internal::CanCastVariantHelper<bool, RefValueType>::CanCast;

			case vtChar | byReference:
				return variant_internal::CanCastVariantHelper<char, RefValueType>::CanCast;

			case vtWchar | byReference:
				return variant_internal::CanCastVariantHelper<wchar_t, RefValueType>::CanCast;

			case vtSignedChar | byReference:
				return variant_internal::CanCastVariantHelper<signed char, RefValueType>::CanCast;

			case vtUnsignedChar | byReference:
				return variant_internal::CanCastVariantHelper<unsigned char, RefValueType>::CanCast;

			case vtSignedShort | byReference:
				return variant_internal::CanCastVariantHelper<signed short, RefValueType>::CanCast;

			case vtUnsignedShort | byReference:
				return variant_internal::CanCastVariantHelper<unsigned short, RefValueType>::CanCast;

			case vtSignedInt | byReference:
				return variant_internal::CanCastVariantHelper<signed int, RefValueType>::CanCast;

			case vtUnsignedInt | byReference:
				return variant_internal::CanCastVariantHelper<unsigned int, RefValueType>::CanCast;

			case vtSignedLong | byReference:
				return variant_internal::CanCastVariantHelper<signed long, RefValueType>::CanCast;

			case vtUnsignedLong | byReference:
				return variant_internal::CanCastVariantHelper<unsigned long, RefValueType>::CanCast;

			case vtSignedLongLong | byReference:
				return variant_internal::CanCastVariantHelper<signed long long, RefValueType>::CanCast;

			case vtUnsignedLongLong | byReference:
				return variant_internal::CanCastVariantHelper<unsigned long long, RefValueType>::CanCast;

			case vtFloat | byReference:
				return variant_internal::CanCastVariantHelper<float, RefValueType>::CanCast;

			case vtDouble | byReference:
				return variant_internal::CanCastVariantHelper<double, RefValueType>::CanCast;

			case vtLongDouble | byReference:
				return variant_internal::CanCastVariantHelper<long double, RefValueType>::CanCast;

			case vtVoid | byReference:
				return variant_internal::CanCastVariantHelper<const void *, RefValueType>::CanCast;

			case vtPointer | byReference:
				return variant_internal::CanCastVariantHelper<const void *, RefValueType>::CanCast;

			case vtObject | byReference:
				return variant_internal::CanCastVariantHelper<const void *, RefValueType>::CanCast;
		}

		return false;
	}
};

template <typename T, typename Policy>
struct CanCastFromReference <T, Policy, typename GEnableIf<GOrResult<IsReference<T>, IsPointer<T> >::Result>::Result>
{
	typedef typename variant_internal::ArrayToPointer<typename CastResult<T, Policy>::Result>::Result ResultType;
	typedef typename variant_internal::ArrayToPointer<typename RemoveReference<T>::Result>::Result RefValueType;

	static bool canCast(const GVariant & v) {
		int vt = static_cast<int>(vtGetType(v.refData().typeData));
		switch(vt) {
			case vtBool | byReference:
				return variant_internal::CanCastVariantHelper<const volatile bool *, const volatile RefValueType *>::CanCast;

			case vtChar | byReference:
				return variant_internal::CanCastVariantHelper<const volatile char *, const volatile RefValueType *>::CanCast;

			case vtWchar | byReference:
				return variant_internal::CanCastVariantHelper<const volatile wchar_t *, const volatile RefValueType *>::CanCast;

			case vtSignedChar | byReference:
				return variant_internal::CanCastVariantHelper<const volatile signed char *, const volatile RefValueType *>::CanCast;

			case vtUnsignedChar | byReference:
				return variant_internal::CanCastVariantHelper<const volatile unsigned char *, const volatile RefValueType *>::CanCast;

			case vtSignedShort | byReference:
				return variant_internal::CanCastVariantHelper<const volatile signed short *, const volatile RefValueType *>::CanCast;

			case vtUnsignedShort | byReference:
				return variant_internal::CanCastVariantHelper<const volatile unsigned short *, const volatile RefValueType *>::CanCast;

			case vtSignedInt | byReference:
				return variant_internal::CanCastVariantHelper<const volatile signed int *, const volatile RefValueType *>::CanCast;

			case vtUnsignedInt | byReference:
				return variant_internal::CanCastVariantHelper<const volatile unsigned int *, const volatile RefValueType *>::CanCast;

			case vtSignedLong | byReference:
				return variant_internal::CanCastVariantHelper<const volatile signed long *, const volatile RefValueType *>::CanCast;

			case vtUnsignedLong | byReference:
				return variant_internal::CanCastVariantHelper<const volatile unsigned long *, const volatile RefValueType *>::CanCast;

			case vtSignedLongLong | byReference:
				return variant_internal::CanCastVariantHelper<const volatile signed long long *, const volatile RefValueType *>::CanCast;

			case vtUnsignedLongLong | byReference:
				return variant_internal::CanCastVariantHelper<const volatile unsigned long long *, const volatile RefValueType *>::CanCast;

			case vtFloat | byReference:
				return variant_internal::CanCastVariantHelper<const volatile float *, const volatile RefValueType *>::CanCast;

			case vtDouble | byReference:
				return variant_internal::CanCastVariantHelper<const volatile double *, const volatile RefValueType *>::CanCast;

			case vtLongDouble | byReference:
				return variant_internal::CanCastVariantHelper<const volatile long double *, const volatile RefValueType *>::CanCast;

			case vtVoid | byReference:
				return variant_internal::CanCastVariantHelper<const volatile void *, const volatile RefValueType *>::CanCast;

			case vtPointer | byReference:
				return variant_internal::CanCastVariantHelper<const volatile void *, const volatile RefValueType *>::CanCast;

			case vtObject | byReference:
				return variant_internal::CanCastVariantHelper<const volatile void *, const volatile RefValueType *>::CanCast;
		}

		return false;
	}
};


template <typename T, typename Policy>
struct CanCastFromVariant
{
	typedef typename CastResult<T, Policy>::Result ResultType;
	typedef typename RemoveReference<T>::Result RefValueType;

	static bool canCast(const GVariant & v) {
		(void)v;

		int vt = static_cast<int>(vtGetType(v.refData().typeData));
		switch(vt) {
			case vtBool:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CanCastVariantHelper<bool, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueBool == 0)
				;

			case vtChar:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CanCastVariantHelper<char, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueChar == 0)
				;

			case vtWchar:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CanCastVariantHelper<wchar_t, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueWchar == 0)
				;

			case vtSignedChar:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CanCastVariantHelper<signed char, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueSignedChar == 0)
				;

			case vtUnsignedChar:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CanCastVariantHelper<unsigned char, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueUnsignedChar == 0)
				;

			case vtSignedShort:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CanCastVariantHelper<signed short, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueSignedShort == 0)
				;

			case vtUnsignedShort:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CanCastVariantHelper<unsigned short, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueUnsignedShort == 0)
				;

			case vtSignedInt:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CanCastVariantHelper<signed int, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueSignedInt == 0)
				;

			case vtUnsignedInt:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CanCastVariantHelper<unsigned int, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueUnsignedInt == 0)
				;

			case vtSignedLong:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CanCastVariantHelper<signed long, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueSignedLong == 0)
				;

			case vtUnsignedLong:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CanCastVariantHelper<unsigned long, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueUnsignedLong == 0)
				;

			case vtSignedLongLong:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CanCastVariantHelper<signed long long, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueSignedLongLong == 0)
				;

			case vtUnsignedLongLong:
				return (variant_internal::isNotPointer<ResultType>() && variant_internal::CanCastVariantHelper<unsigned long long, ResultType>::CanCast)
					|| (IsPointer<ResultType>::Result && v.refData().valueUnsignedLongLong == 0)
				;

			case vtFloat:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CanCastVariantHelper<float, ResultType>::CanCast;

			case vtDouble:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CanCastVariantHelper<double, ResultType>::CanCast;

			case vtLongDouble:
				return variant_internal::isNotPointer<ResultType>() && variant_internal::CanCastVariantHelper<long double, ResultType>::CanCast;

			case vtPointer:
				return variant_internal::CanCastVariantHelper<const volatile void *, ResultType>::CanCast;

			case vtObject:
				return variant_internal::CanCastVariantHelper<const volatile void *, typename RemoveReference<ResultType>::Result *>::CanCast;

			case vtShadow:
				return variant_internal::CanCastVariantHelper<const volatile void *, typename RemoveReference<ResultType>::Result *>::CanCast;

			case vtString:
				return IsConvertible<char *, ResultType>::Result || IsConvertible<const char *, ResultType>::Result;

			case vtWideString:
				return IsConvertible<wchar_t *, ResultType>::Result || IsConvertible<const wchar_t *, ResultType>::Result;

			case vtInterface:
				return variant_internal::EnforceCanCastToPointer<IObject *, typename RemoveReference<ResultType>::Result>::CanCast;
			
			case vtTypedVar:
				return CanCastFromVariant<T, Policy>::canCast(getVariantRealValue(v));

			case vtBool | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<bool *, ResultType>::CanCast;

			case vtChar | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<char *, ResultType>::CanCast;

			case vtWchar | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<wchar_t *, ResultType>::CanCast;

			case vtSignedChar | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<signed char *, ResultType>::CanCast;

			case vtUnsignedChar | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<unsigned char *, ResultType>::CanCast;

			case vtSignedShort | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<signed short *, ResultType>::CanCast;

			case vtUnsignedShort | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<unsigned short *, ResultType>::CanCast;

			case vtSignedInt | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<signed int *, ResultType>::CanCast;

			case vtUnsignedInt | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<unsigned int *, ResultType>::CanCast;

			case vtSignedLong | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<signed long *, ResultType>::CanCast;

			case vtUnsignedLong | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<unsigned long *, ResultType>::CanCast;

			case vtSignedLongLong | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<signed long long *, ResultType>::CanCast;

			case vtUnsignedLongLong | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<unsigned long long *, ResultType>::CanCast;

			case vtFloat | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<float *, ResultType>::CanCast;

			case vtDouble | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<double *, ResultType>::CanCast;

			case vtLongDouble | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<long double *, ResultType>::CanCast;

			case vtVoid | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<void *, ResultType>::CanCast;

			case vtPointer | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<void *, ResultType>::CanCast;

			case vtObject | byPointer:
				return variant_internal::isNotFundamental<ResultType>() && variant_internal::CanCastVariantHelper<void *, ResultType>::CanCast;

			default:
				if((vt & byReference) != 0) {
					return CanCastFromReference<T, Policy>::canCast(v);
				}
				break;
		}

		return false;
	}
};


} // namespace variant_internal


#endif

