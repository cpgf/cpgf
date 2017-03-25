#ifndef CPGF_GTYPETRAITS_H
#define CPGF_GTYPETRAITS_H

#include "cpgf/gcompiler.h"
#include "cpgf/genableif.h"
#include "cpgf/gtypelist.h"
#include "cpgf/gfunctiontraits.h"

#ifdef _MSC_VER
#include <wchar.h>
#endif

#ifdef G_COMPILER_CPPBUILDER
#include <type_traits>
#endif

// suppress all warnings
#ifdef __GNUC__
#pragma GCC system_header
#endif


namespace cpgf {

template <typename T>
struct PointerDimension
{
	G_STATIC_CONSTANT(int, Result = 0);
};

template <typename T> struct PointerDimension <T *>
{
	G_STATIC_CONSTANT(int, Result = 1 + PointerDimension<T>::Result);
};

template <typename T> struct PointerDimension <T const>
{
	G_STATIC_CONSTANT(int, Result = PointerDimension<T>::Result);
};
template <typename T> struct PointerDimension <T volatile>
{
	G_STATIC_CONSTANT(int, Result = PointerDimension<T>::Result);
};

template <typename T> struct PointerDimension <T const volatile>
{
	G_STATIC_CONSTANT(int, Result = PointerDimension<T>::Result);
};


template <typename T>
struct IsPointer
{
	G_STATIC_CONSTANT(bool, Result = PointerDimension<T>::Result > 0);
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
	G_STATIC_CONSTANT(bool, Result = false);
};

template <typename T>
struct IsReference <T &>
{
	G_STATIC_CONSTANT(bool, Result = true);
};

template <typename T>
struct IsReference <T &&>
{
	G_STATIC_CONSTANT(bool, Result = true);
};


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

template <typename T>
struct RemoveReference <T &&>
{
	typedef T Result;
};


template <typename T>
struct IsLValueReference
{
	G_STATIC_CONSTANT(bool, Result = false);
};

template <typename T>
struct IsLValueReference <T &>
{
	G_STATIC_CONSTANT(bool, Result = true);
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
	G_STATIC_CONSTANT(bool, Result = false);
};

template <typename T>
struct IsRValueReference <T &&>
{
	G_STATIC_CONSTANT(bool, Result = true);
};


template <typename T>
struct RemoveRValueReference
{
	typedef T Result;
};

template <typename T>
struct RemoveRValueReference <T &&>
{
	typedef T Result;
};


template <typename T>
struct IsArray
{
	G_STATIC_CONSTANT(bool, Result = false);
};

template <typename T> struct IsArray <T []> { G_STATIC_CONSTANT(bool, Result = true); };
template <typename T> struct IsArray <T const []> { G_STATIC_CONSTANT(bool, Result = true); };
template <typename T> struct IsArray <T volatile []> { G_STATIC_CONSTANT(bool, Result = true); };
template <typename T> struct IsArray <T const volatile []> { G_STATIC_CONSTANT(bool, Result = true); };
#ifndef G_COMPILER_CPPBUILDER
template <typename T, unsigned int N> struct IsArray <T [N]> { G_STATIC_CONSTANT(bool, Result = true); };
template <typename T, unsigned int N> struct IsArray <T const [N]> { G_STATIC_CONSTANT(bool, Result = true); };
template <typename T, unsigned int N> struct IsArray <T volatile [N]> { G_STATIC_CONSTANT(bool, Result = true); };
template <typename T, unsigned int N> struct IsArray <T const volatile [N]> { G_STATIC_CONSTANT(bool, Result = true); };
#endif


template <typename T, typename Enable = void>
struct ArraySize
{
	enum { Result = 0 };
};

template <typename T>
struct ArraySize <T, typename GEnableIfResult<IsArray<T> >::Result>
{
private:
	static T arrayData;

public:
	enum { Result = (sizeof(arrayData) / sizeof(arrayData[0])) };
};


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

template <>
struct AddReference <void>
{
	typedef void Result;
};



template <typename T>
struct IsConst
{
	G_STATIC_CONSTANT(bool, Result = false);
};

template <typename T>
struct IsConst <T const>
{
	G_STATIC_CONSTANT(bool, Result = true);
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
	G_STATIC_CONSTANT(bool, Result = false);
};

template <typename T>
struct IsVolatile <T volatile>
{
	G_STATIC_CONSTANT(bool, Result = true);
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
	G_STATIC_CONSTANT(bool, Result = false);
};

template <typename T>
struct IsConstVolatile <T const volatile>
{
	G_STATIC_CONSTANT(bool, Result = true);
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
template <typename T> struct ExtractRawType <T &&> { typedef typename ExtractRawType<T>::Result Result; };



template <typename T1, typename T2>
struct IsSameType
{
	G_STATIC_CONSTANT(bool, Result = false);
};

template <typename T>
struct IsSameType <T, T>
{
	G_STATIC_CONSTANT(bool, Result = true);
};


template <typename T>
struct IsVoid
{
	G_STATIC_CONSTANT(bool, Result = (TypeList_IndexOf< TypeList_Make<void, void const, void volatile, void const volatile>::Result, T>::Result >= 0));
};


typedef TypeList_Make<float, double, long double>::Result FloatTypes;

template <typename T>
struct IsFloat
{
	G_STATIC_CONSTANT(bool, Result = (TypeList_IndexOf<FloatTypes, T>::Result >= 0));
};


typedef TypeList_Make<signed char, short, int, long, long long>::Result SignedTypes;
typedef TypeList_Make<unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long>::Result UnsignedTypes;
typedef TypeList_Make<bool, char, wchar_t>::Result UnknownSignTypes;


template <typename T>
struct IsSigned
{
	G_STATIC_CONSTANT(bool, Result = (TypeList_IndexOf<SignedTypes, T>::Result >= 0));
};


template <typename T>
struct IsUnsigned
{
	G_STATIC_CONSTANT(bool, Result = (TypeList_IndexOf<UnsignedTypes, T>::Result >= 0));
};


template <typename T>
struct IsUnknownSign
{
	G_STATIC_CONSTANT(bool, Result = (TypeList_IndexOf<UnknownSignTypes, T>::Result >= 0));
};


template <typename T>
struct IsInteger
{
	G_STATIC_CONSTANT(bool, Result = (GOrResult<IsSigned<T>, IsUnsigned<T>, IsUnknownSign<T> >::Result));
};


#ifndef G_COMPILER_CPPBUILDER
template <typename T>
struct IsFundamental
{
	G_STATIC_CONSTANT(bool, Result = (GOrResult<IsInteger<T>, IsFloat<T> >::Result));
};
#else
template <class __T>
struct IsFundamental
{
//	G_STATIC_CONSTANT(bool, Result = __is_fundamental(T));
	static const bool Result=__is_fundamental(__T);
};
#endif


namespace typetraits_internal {
	typedef char YesType;
	struct NoType { char a[8]; };
} // namespace typetraits_internal


#ifndef G_COMPILER_CPPBUILDER
template <typename T>
struct IsClass
{
public:
	template <typename C>
	static typetraits_internal::YesType test(int C::*);
	template <typename C>
	static typetraits_internal::NoType test(...);

	G_STATIC_CONSTANT(bool, Result = (sizeof(test<T>(0)) == sizeof(typetraits_internal::YesType)));
};
#else
template <typename T>
struct IsClass
{
	G_STATIC_CONSTANT(bool, Result = __is_class(T));
};
#endif


#ifndef G_COMPILER_CPPBUILDER
template <typename T, typename Enable = void>
struct IsAbstractClass
{
	G_STATIC_CONSTANT(bool, Result = false);
};

template <typename T>
struct IsAbstractClass <T, typename GEnableIfResult<IsClass<T> >::Result>
{
private:
	template <typename U>
	static typetraits_internal::NoType check(U (*)[1]);
	template <typename U>
	static typetraits_internal::YesType check(...);

public:
	G_STATIC_CONSTANT(bool, Result = (sizeof(T) != 0 && sizeof(check<T>(0)) == sizeof(typetraits_internal::YesType)));
};
#else
template <typename T>
struct IsAbstractClass
{
	G_STATIC_CONSTANT(bool, Result = __is_abstract(T));
};
#endif


template <typename T>
struct IsFunction
{
	G_STATIC_CONSTANT(bool, Result = GFunctionTraits<T>::IsFunction);
};


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4244)
#pragma warning(disable:4800) // warning C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable:4267) // weird warning when reflecting std::vector in VC, disable it.
#endif

#ifndef G_COMPILER_CPPBUILDER
template <typename From, typename To, typename Enabled = void>
struct IsConvertible
{
private:

	static typetraits_internal::NoType check(...);
	static typetraits_internal::YesType check(To);

	static From from;

public:
	G_STATIC_CONSTANT(bool, Result = (IsSameType<From, To>::Result || (sizeof(check(from)) == sizeof(typetraits_internal::YesType))));
};

template <typename From, typename To>
struct IsConvertible <From, To, typename GEnableIfResult<
	GOrResult<
		IsVoid<From>,
		IsVoid<To>,
		IsAbstractClass<From>,
		IsAbstractClass<To>,
		IsArray<From>,
		IsArray<To>,
		IsFunction<From>,
		IsFunction<To>
	>
	>::Result>
{
	G_STATIC_CONSTANT(bool, Result = false);
};
#else
template <typename From, typename To>
struct IsConvertible
{
	G_STATIC_CONSTANT(bool, Result = (std::is_convertible<From, To>::value));
};
#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#if defined(_MSC_VER)
#pragma warning(push)
// 'class1' : inherits 'class2::member' via dominance
#pragma warning(disable:4250)
#endif

// may cause compiler error "no unique final overrider for" on GCC
template <typename Derived, typename Base, typename Enabled = void>
struct IsVirtualBase
{
	G_STATIC_CONSTANT(bool, Result = false);
};

template <typename Derived, typename Base>
struct IsVirtualBase <Derived, Base, typename GEnableIfResult<GAndResult<IsClass<Derived>, IsClass<Base> > >::Result>
{
private:
	struct A : Derived, virtual Base {
		A();
		A(const A &);
		A & operator = (const A &);
		~A();
	};

	struct B : Derived {
		B();
		B(const B &);
		B & operator = (const B &);
		~B();
	};

public:
	G_STATIC_CONSTANT(bool, Result = (!IsSameType<Derived, Base>::Result
		&& IsConvertible<const volatile Derived *, const volatile Base *>::Result
		&& IsConvertible<const volatile Derived &, const volatile Base &>::Result
		&& sizeof(A) == sizeof(B)));
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif


template <typename T, typename Enabled = void>
struct IsPolymorphic
{
	G_STATIC_CONSTANT(bool, Result = false);
};

template <typename T>
struct IsPolymorphic <T, typename GEnableIfResult<IsClass<T> >::Result>
{
private:
	struct A : public T {
		A();
		A(const A &);
		A & operator = (const A &);
		virtual ~A();
		char n[256];
#if !defined(_MSC_VER) && !defined(__ICL)
		// has problem in VC when has virtual base class
		struct V {};
		virtual void x(const V *);
#endif
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
	G_STATIC_CONSTANT(bool, Result = (sizeof(A) == sizeof(B)));
};

template <typename T>
struct IsEnum
{
	G_STATIC_CONSTANT(bool, Result = (
		IsConvertible<T, int>::Result
		&& !IsFundamental<T>::Result
		&& !IsClass<T>::Result
		&& !IsReference<T>::Result
		&& !IsPointer<T>::Result
		&& !IsFunction<T>::Result
		)
	);
};

template <typename T>
struct MemberDataTrait
{
	G_STATIC_CONSTANT(bool, Result = false);
	typedef void ObjectType;
	typedef void FieldType;
	typedef void DataType;
};

template <typename OT, typename FT>
struct MemberDataTrait <FT OT::*>
{
	G_STATIC_CONSTANT(bool, Result = true);
	typedef OT ObjectType;
	typedef FT FieldType;
	typedef FT OT::* DataType;
};

template <typename OT, typename FT>
struct MemberDataTrait <FT OT::* const>
{
	G_STATIC_CONSTANT(bool, Result = true);
	typedef OT ObjectType;
	typedef FT FieldType;
	typedef FT OT::* const DataType;
};

template <typename OT, typename FT>
struct MemberDataTrait <FT OT::* volatile>
{
	G_STATIC_CONSTANT(bool, Result = true);
	typedef OT ObjectType;
	typedef FT FieldType;
	typedef FT OT::* volatile DataType;
};

template <typename OT, typename FT>
struct MemberDataTrait <FT OT::* const volatile>
{
	G_STATIC_CONSTANT(bool, Result = true);
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
struct GArgumentTraits <T, typename GEnableIfResult<IsFundamental<T> >::Result>
{
	typedef T Result;
};

template <typename T>
struct GArgumentTraits <T &>
{
	typedef T & Result;
};

template <typename T>
struct GArgumentTraits <T &&>
{
	typedef T && Result;
};

#ifndef G_COMPILER_CPPBUILDER
template <typename T, unsigned int N>
struct GArgumentTraits <T [N]>
{
	typedef const T * & Result;
};
#endif


// http://stackoverflow.com/questions/5100015/c-metafunction-to-determine-whether-a-type-is-callable
template<typename F, typename...Args>
struct IsCallable
{
	template<typename U> static auto test(U * p) -> decltype((*p)(std::declval<Args>()...), void(), std::true_type());
	template<typename U> static auto test(...) -> decltype(std::false_type());

	static constexpr bool Result = decltype(test<typename std::remove_reference<F>::type>(0))::value;
};


} // namespace cpgf



#endif

