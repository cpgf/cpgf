#ifndef __GTYPETRAITS_H
#define __GTYPETRAITS_H

#include "cpgf/gcompiler.h"
#include "cpgf/genableif.h"
#include "cpgf/gtypelist.h"
#include "cpgf/gfunctiontraits.h"

namespace cpgf {

template <typename T>
struct PointerDimension
{
	enum { Result = 0 };
};

template <typename T> struct PointerDimension <T *> { enum { Result = 1 + PointerDimension<T>::Result }; };
template <typename T> struct PointerDimension <T const> { enum { Result = PointerDimension<T>::Result }; };
template <typename T> struct PointerDimension <T volatile> { enum { Result = PointerDimension<T>::Result }; };
template <typename T> struct PointerDimension <T const volatile> { enum { Result = PointerDimension<T>::Result }; };


template <typename T>
struct IsPointer
{
	enum { Result = PointerDimension<T>::Result > 0 };
};


template <typename T>
struct RemovePointer
{
	typedef T Result;
};

template <typename T> struct RemovePointer <T *> { typedef T Result; };
template <typename T> struct RemovePointer <T * const> { typedef T Result; };
template <typename T> struct RemovePointer <T * volatile> { typedef T Result; };
template <typename T> struct RemovePointer <T * const volatile> { typedef T Result; };


template <typename T, unsigned int N = 1>
struct AddPointer
{
	typedef typename AddPointer<T, N - 1>::Result * Result;
};

template <typename T>
struct AddPointer <T, 0>
{
	typedef T Result;
};



template <typename T>
struct IsReference
{
	enum { Result = false };
};

template <typename T>
struct IsReference <T &>
{
	enum { Result = true };
};

#if G_SUPPORT_RVALUE_REFERENCE
template <typename T>
struct IsReference <T &&>
{
	enum { Result = true };
};
#endif


template <typename T>
struct RemoveReference
{
	typedef T Result;
};

template <typename T>
struct RemoveReference <T &>
{
	typedef T Result;
};

#if G_SUPPORT_RVALUE_REFERENCE
template <typename T>
struct RemoveReference <T &&>
{
	typedef T Result;
};
#endif


template <typename T>
struct IsLValueReference
{
	enum { Result = false };
};

template <typename T>
struct IsLValueReference <T &>
{
	enum { Result = true };
};


template <typename T>
struct RemoveLValueReference
{
	typedef T Result;
};

template <typename T>
struct RemoveLValueReference <T &>
{
	typedef T Result;
};


template <typename T>
struct IsRValueReference
{
	enum { Result = false };
};

#if G_SUPPORT_RVALUE_REFERENCE
template <typename T>
struct IsRValueReference <T &&>
{
	enum { Result = true };
};
#endif


template <typename T>
struct RemoveRValueReference
{
	typedef T Result;
};

#if G_SUPPORT_RVALUE_REFERENCE
template <typename T>
struct RemoveRValueReference <T &&>
{
	typedef T Result;
};
#endif


template <typename T>
struct IsArray
{
	enum { Result = false };
};

template <typename T> struct IsArray <T []> { enum { Result = true }; };
template <typename T> struct IsArray <T const []> { enum { Result = true }; };
template <typename T> struct IsArray <T volatile []> { enum { Result = true }; };
template <typename T> struct IsArray <T const volatile []> { enum { Result = true }; };
template <typename T, unsigned int N> struct IsArray <T [N]> { enum { Result = true }; };
template <typename T, unsigned int N> struct IsArray <T const [N]> { enum { Result = true }; };
template <typename T, unsigned int N> struct IsArray <T volatile [N]> { enum { Result = true }; };
template <typename T, unsigned int N> struct IsArray <T const volatile [N]> { enum { Result = true }; };


template <typename T>
struct AddReference
{
	typedef T & Result;
};

template <typename T>
struct AddReference <T &>
{
	typedef T & Result;
};



template <typename T>
struct IsConst
{
	enum { Result = false };
};

template <typename T>
struct IsConst <T const>
{
	enum { Result = true };
};


template <typename T>
struct RemoveConst
{
	typedef T Result;
};

template <typename T>
struct RemoveConst <T const>
{
	typedef T Result;
};


template <typename T>
struct AddConst
{
	typedef const T Result;
};




template <typename T>
struct IsVolatile
{
	enum { Result = false };
};

template <typename T>
struct IsVolatile <T volatile>
{
	enum { Result = true };
};


template <typename T>
struct RemoveVolatile
{
	typedef T Result;
};

template <typename T>
struct RemoveVolatile <T volatile>
{
	typedef T Result;
};


template <typename T>
struct IsConstVolatile
{
	enum { Result = false };
};

template <typename T>
struct IsConstVolatile <T const volatile>
{
	enum { Result = true };
};


template <typename T>
struct AddVolatile
{
	typedef volatile T Result;
};



template <typename T>
struct RemoveConstVolatile
{
	typedef typename RemoveConst<typename RemoveVolatile<T>::Result>::Result Result;
};


// remove all CV, pointer, reference
template <typename T>
struct ExtractRawType
{
	typedef T Result;
};

template <typename T> struct ExtractRawType <T const> { typedef typename ExtractRawType<T>::Result Result; };
template <typename T> struct ExtractRawType <T volatile> { typedef typename ExtractRawType<T>::Result Result; };
template <typename T> struct ExtractRawType <T const volatile> { typedef typename ExtractRawType<T>::Result Result; };
template <typename T> struct ExtractRawType <T *> { typedef typename ExtractRawType<T>::Result Result; };
template <typename T> struct ExtractRawType <T &> { typedef typename ExtractRawType<T>::Result Result; };



template <typename T1, typename T2>
struct IsSameType
{
	enum { Result = false };
};

template <typename T>
struct IsSameType <T, T>
{
	enum { Result = true };
};


template <typename T>
struct IsVoid
{
	enum { Result = TypeList_IndexOf< TypeList_Make<void, void const, void volatile, void const volatile>::Result, T>::Result >= 0 };
};


typedef TypeList_Make<float, double, long double>::Result FloatTypes;

template <typename T>
struct IsFloat
{
	enum { Result = TypeList_IndexOf<FloatTypes, T>::Result >= 0 };
};


typedef TypeList_Make<signed char, short, int, long, long long>::Result SignedTypes;
typedef TypeList_Make<unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long>::Result UnsignedTypes;
typedef TypeList_Make<bool, char, wchar_t>::Result UnknownSignTypes;


template <typename T>
struct IsSigned
{
	enum { Result = TypeList_IndexOf<SignedTypes, T>::Result >= 0 };
};


template <typename T>
struct IsUnsigned
{
	enum { Result = TypeList_IndexOf<UnsignedTypes, T>::Result >= 0 };
};


template <typename T>
struct IsUnknownSign
{
	enum { Result = TypeList_IndexOf<UnknownSignTypes, T>::Result >= 0 };
};


template <typename T>
struct IsInteger
{
	enum { Result = (IsSigned<T>::Result || IsUnsigned<T>::Result || IsUnknownSign<T>::Result) };
};


template <typename T>
struct IsFundamental
{
	enum { Result = (IsInteger<T>::Result || IsFloat<T>::Result) };
};


namespace typetraits_internal {
	typedef char YesType;
	struct NoType { char a[8]; };
} // namespace typetraits_internal


template <typename T>
class IsClass
{
public:
	template <typename C>
	static typetraits_internal::YesType test(int C::*);
	template <typename C>
	static typetraits_internal::NoType test(...);

	static const bool Result = (sizeof(test<T>(0)) == sizeof(typetraits_internal::YesType));
};

template <typename T>
struct IsAbstractClass
{
private:
	template <typename U>
	static typetraits_internal::NoType check(U (*)[1]);
	template <typename U>
	static typetraits_internal::YesType check(...);

public:
	enum { Result = (sizeof(T) != 0 && IsClass<T>::Result && sizeof(check<T>(0)) == sizeof(typetraits_internal::YesType)) };
};

template <>
struct IsAbstractClass <void>
{
	enum { Result = false };
};


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4244)
#pragma warning(disable:4800) // warning C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable:4267) // weird warning when reflecting std::vector in VC, disable it.
#endif

template <typename From, typename To, typename Enabled = void>
struct IsConvertible
{
private:

	static typetraits_internal::NoType check(...);
	static typetraits_internal::YesType check(To);

	static From makeFrom();

public:
	enum { Result = IsSameType<From, To>::Result
		|| (sizeof(check(makeFrom())) == sizeof(typetraits_internal::YesType)) };
};

template <typename From, typename To>
struct IsConvertible <From, To, typename GEnableIf<
	IsVoid<From>::Result
	|| IsVoid<To>::Result
	|| IsAbstractClass<From>::Result
	|| IsArray<From>::Result
	|| IsArray<To>::Result
	>::Result>
{
public:
	enum { Result = false };
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif


template <typename T>
struct IsFunction
{
	enum { Result = GFunctionTraits<T>::IsFunction };
};


#if defined(_MSC_VER)
#pragma warning(push)
// 'class1' : inherits 'class2::member' via dominance
#pragma warning(disable:4250)
#endif

// may cause compiler error "no unique final overrider for" on GCC
template <typename Derived, typename Base, typename Enabled = void>
struct IsVirtualBase
{
	enum { Result = false };
};

template <typename Derived, typename Base>
struct IsVirtualBase <Derived, Base, typename GEnableIf<IsClass<Derived>::Result && IsClass<Base>::Result>::Result>
{
private:
	struct A : public Derived, public virtual Base {
		A();
		A(const A &);
		A & operator = (const A &);
		~A();
	};

	struct B : public Derived {
		B();
		B(const B &);
		B & operator = (const B &);
		~B();
	};

public:
	enum { Result = (!IsSameType<Derived, Base>::Result
		&& IsConvertible<const volatile Derived *, const volatile Base *>::Result
		&& IsConvertible<const volatile Derived &, const volatile Base &>::Result
		&& sizeof(A) == sizeof(B)) };
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif


template <typename T, typename Enabled = void>
struct IsPolymorphic
{
	enum { Result = false };
};

template <typename T>
struct IsPolymorphic <T, typename GEnableIf<IsClass<T>::Result>::Result>
{
private:
	struct A : public T {
		A();
		A(const A &);
		A & operator = (const A &);
		virtual ~A();
		char n[256];
		struct V {};
		virtual void x(const V *);
	};

	struct B : public T {
		B();
		B(const B &);
		B & operator = (const B &);
		~B();
		char n[256];
		struct V {};
	};

public:
	enum { Result = (sizeof(A) == sizeof(B)) };
};

template <typename T>
struct MemberDataTrait
{
	enum { IsMemberData = false };
};

template <typename OT, typename FT>
struct MemberDataTrait <FT OT::*>
{
	enum { IsMemberData = true };
	typedef OT ObjectType;
	typedef FT FieldType;
	typedef FT OT::* DataType;
};

template <typename OT, typename FT>
struct MemberDataTrait <FT OT::* const>
{
	enum { IsMemberData = true };
	typedef OT ObjectType;
	typedef FT FieldType;
	typedef FT OT::* const DataType;
};

template <typename OT, typename FT>
struct MemberDataTrait <FT OT::* volatile>
{
	enum { IsMemberData = true };
	typedef OT ObjectType;
	typedef FT FieldType;
	typedef FT OT::* volatile DataType;
};

template <typename OT, typename FT>
struct MemberDataTrait <FT OT::* const volatile>
{
	enum { IsMemberData = true };
	typedef OT ObjectType;
	typedef FT FieldType;
	typedef FT OT::* const volatile DataType;
};


template <typename T, typename Enabled = void>
struct GArgumentTraits
{
	typedef const T & Result;
};

template <typename T>
struct GArgumentTraits <T, typename GEnableIf<IsFundamental<T>::Result>::Result>
{
	typedef T Result;
};

template <typename T>
struct GArgumentTraits <T *>
{
	typedef T * Result;
};

template <typename T>
struct GArgumentTraits <T &>
{
	typedef T & Result;
};

#if G_SUPPORT_RVALUE_REFERENCE
template <typename T>
struct GArgumentTraits <T &&>
{
	typedef T && Result;
};
#endif

template <typename T, unsigned int N>
struct GArgumentTraits <T [N]>
{
	typedef const T * & Result;
};


} // namespace cpgf



#endif
