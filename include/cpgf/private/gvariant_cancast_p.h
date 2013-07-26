#ifndef __GVARIANT_CANCAST_P_H
#define __GVARIANT_CANCAST_P_H


namespace variant_internal {

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
				return variant_internal::CastVariantHelper<bool, RefValueType>::CanCast;

			case vtChar | byReference:
				return variant_internal::CastVariantHelper<char, RefValueType>::CanCast;

			case vtWchar | byReference:
				return variant_internal::CastVariantHelper<wchar_t, RefValueType>::CanCast;

			case vtSignedChar | byReference:
				return variant_internal::CastVariantHelper<signed char, RefValueType>::CanCast;

			case vtUnsignedChar | byReference:
				return variant_internal::CastVariantHelper<unsigned char, RefValueType>::CanCast;

			case vtSignedShort | byReference:
				return variant_internal::CastVariantHelper<signed short, RefValueType>::CanCast;

			case vtUnsignedShort | byReference:
				return variant_internal::CastVariantHelper<unsigned short, RefValueType>::CanCast;

			case vtSignedInt | byReference:
				return variant_internal::CastVariantHelper<signed int, RefValueType>::CanCast;

			case vtUnsignedInt | byReference:
				return variant_internal::CastVariantHelper<unsigned int, RefValueType>::CanCast;

			case vtSignedLong | byReference:
				return variant_internal::CastVariantHelper<signed long, RefValueType>::CanCast;

			case vtUnsignedLong | byReference:
				return variant_internal::CastVariantHelper<unsigned long, RefValueType>::CanCast;

			case vtSignedLongLong | byReference:
				return variant_internal::CastVariantHelper<signed long long, RefValueType>::CanCast;

			case vtUnsignedLongLong | byReference:
				return variant_internal::CastVariantHelper<unsigned long long, RefValueType>::CanCast;

			case vtFloat | byReference:
				return variant_internal::CastVariantHelper<float, RefValueType>::CanCast;

			case vtDouble | byReference:
				return variant_internal::CastVariantHelper<double, RefValueType>::CanCast;

			case vtLongDouble | byReference:
				return variant_internal::CastVariantHelper<long double, RefValueType>::CanCast;

			case vtVoid | byReference:
				return variant_internal::CastVariantHelper<const void *, RefValueType>::CanCast;

			case vtPointer | byReference:
				return variant_internal::CastVariantHelper<const void *, RefValueType>::CanCast;

			case vtObject | byReference:
				return variant_internal::CastVariantHelper<const void *, RefValueType>::CanCast;
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

