#ifndef __GVARTYPEDATA_P_H
#define __GVARTYPEDATA_P_H

#include "cpgf/gtypetraits.h"
#include "cpgf/genableif.h"


namespace variant_internal {


template <typename T, typename Enabled = void>
struct DeduceBasicVariantType {
	static const GVariantType Result = IsPointer<T>::Result ? vtPointer : vtObject;
};

#define DEDUCEVT(RT, VT) template <> struct DeduceBasicVariantType <RT> { static const GVariantType Result = VT; }
DEDUCEVT(bool, vtBool);
DEDUCEVT(char, vtChar);
template <typename T>
struct DeduceBasicVariantType <T,
	typename GEnableIf<
		IsSameType<T, wchar_t>::Result
		&& !IsSameType<wchar_t, unsigned short>::Result
		&& !IsSameType<wchar_t, unsigned int>::Result>::Result
	>
{ static const GVariantType Result = vtWchar; };
//DEDUCEVT(wchar_t, vtWchar);
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
struct DeduceVariantType_Helper <T, typename GEnableIfResult<IsPointer<T> >::Result> {
private:
	static const int temp = DeduceBasicVariantType<typename ExtractRawType<T>::Result>::Result;
public:
	static const GVariantType Result = static_cast<GVariantType>(temp | byPointer);
	static const int Pointers = PointerDimension<
		typename RemoveConstVolatile<
			typename RemoveReference<
				typename RemoveConstVolatile<T>::Result
			>::Result
		>::Result
	>::Result;
};

template <typename T>
struct DeduceVariantType_Helper <T, typename GEnableIfResult<IsReference<T> >::Result> {
private:
	static const int temp = DeduceBasicVariantType<typename RemoveConstVolatile<typename RemoveReference<T>::Result>::Result>::Result;
public:
	static const GVariantType Result = static_cast<GVariantType>(temp | byReference);
	static const int Pointers = 0;
};

template <typename T>
struct DeduceVariantType_Helper <T, typename GEnableIfResult<IsEnum<T> >::Result> {
	static const GVariantType Result = vtUnsignedInt;
	static const int Pointers = 0;
};

template <typename T>
struct ArrayToPointer
{
	typedef T Result;

	G_STATIC_CONSTANT(bool, IsArray = false);
};

#define DEF_ARRAY_TO_PTR(CV) \
	template <typename T> struct ArrayToPointer <CV T[]> { typedef const volatile typename ArrayToPointer<T>::Result * Result; G_STATIC_CONSTANT(bool, IsArray = true); }; \
	template <typename T> struct ArrayToPointer <CV T (*) []> { typedef const volatile typename ArrayToPointer<T>::Result * Result; G_STATIC_CONSTANT(bool, IsArray = true); };

DEF_ARRAY_TO_PTR(GPP_EMPTY())
DEF_ARRAY_TO_PTR(const)
DEF_ARRAY_TO_PTR(volatile)
DEF_ARRAY_TO_PTR(const volatile)

#undef DEF_ARRAY_TO_PTR

#ifndef G_COMPILER_CPPBUILDER
#define DEF_ARRAY_TO_PTR(CV) \
	template <typename T, unsigned int N> struct ArrayToPointer <CV T[N]> { typedef const volatile typename ArrayToPointer<T>::Result * Result; G_STATIC_CONSTANT(bool, IsArray = true); }; \
	template <typename T, unsigned int N> struct ArrayToPointer <CV T (*) [N]> { typedef const volatile typename ArrayToPointer<T>::Result * Result; G_STATIC_CONSTANT(bool, IsArray = true); }; \
	template <typename T, unsigned int N> struct ArrayToPointer <CV T (&) [N]> { typedef const volatile typename ArrayToPointer<T>::Result * Result; G_STATIC_CONSTANT(bool, IsArray = true); };

DEF_ARRAY_TO_PTR(GPP_EMPTY())
DEF_ARRAY_TO_PTR(const)
DEF_ARRAY_TO_PTR(volatile)
DEF_ARRAY_TO_PTR(const volatile)

#undef DEF_ARRAY_TO_PTR
#endif

template <typename T, typename Enable = void>
struct DeduceVariantType
{
private:
	typedef DeduceVariantType_Helper<typename ArrayToPointer<typename RemoveConstVolatile<T>::Result>::Result> Deducer;
public:
	static const GVariantType Result = Deducer::Result;
	static const int Pointers = Deducer::Pointers;
};

template <>
struct DeduceVariantType <void>
{
	static const GVariantType Result = vtVoid;
	static const int Pointers = 0;
};

template <typename T>
struct DeduceVariantType <T *, typename GEnableIfResult<IsConvertible<const typename RemoveConstVolatile<T>::Result &, const IObject &> >::Result>
{
	static const GVariantType Result = vtInterface;
	static const int Pointers = 1;
};

template <typename T>
struct ArrayToPointer;

template <typename T>
struct CheckIsArray {
	G_STATIC_CONSTANT(bool, Result = T::IsArray);
};

template <typename T, typename Enabled = void>
struct DeducePassType
{
	typedef T Result;
    typedef typename RemoveReference<T>::Result PassType;
};

template <typename T>
struct DeducePassType <T, typename GEnableIfResult<CheckIsArray<ArrayToPointer<T> > >::Result>
{
	typedef typename ArrayToPointer<T>::Result Result;
	typedef typename ArrayToPointer<T>::Result PassType;
};

inline bool isVtUsingShadow(int vt) {
	return vt == vtShadow || vt == vtString || vt == vtWideString;
}



} // namespace variant_internal




#endif

