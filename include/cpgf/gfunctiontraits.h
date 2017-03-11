#ifndef CPGF_GFUNCTIONTRAITS_H
#define CPGF_GFUNCTIONTRAITS_H

#include "cpgf/gcompiler.h"
#include "cpgf/gpp.h"
#include "cpgf/gtypelist.h"
#include "cpgf/gassert.h"

#include <type_traits>

namespace cpgf {

typedef void GFunctionTraitNullType;

namespace functiontraits_internal {

constexpr int ftfConst = (1 << 0);
constexpr int ftfVolatile = (1 << 1);
constexpr int ftfConstVolatile = (1 << 2);
constexpr int ftfStdCall = (1 << 3);
constexpr int ftfFastCall = (1 << 4);
constexpr int ftfNoReturn = (1 << 5);

// this struct is here to be compatible with the old function traits.
// this will be removed after all other parts are refactored.
template <typename... Parameters>
struct GFunctionTraitsArgList
{
	static constexpr unsigned int Arity = sizeof...(Parameters);
	typedef typename cpgf::TypeList_Make<Parameters...>::Result ArgTypeList;

#define FT(N, P) typedef typename std::conditional<(Arity > N), typename cpgf::TypeList_Get<ArgTypeList, N>::Result, GFunctionTraitNullType>::type Arg ## N;
	
	GPP_REPEAT(60, FT, GPP_EMPTY())

#undef FT
};


template <int flags, typename AT, typename OT, typename RT, typename... Parameters>
struct GFunctionTraitsBase
{
	typedef AT FullType;
	typedef RT FunctionType (Parameters...);
	typedef typename std::conditional<std::is_pointer<FullType>::value, FullType, FullType *>:: type FunctionPointer;
	typedef typename cpgf::TypeList_Make<Parameters...>::Result ArgTypeList;
	typedef GFunctionTraitsArgList<Parameters...> ArgList;

	typedef OT ObjectType;
	typedef RT ResultType;

	static constexpr bool IsFunction = true;

	static constexpr int Arity = sizeof...(Parameters);
	static constexpr bool HasResult = ! std::is_void<RT>::value;

	static constexpr bool IsGlobal = std::is_same<OT, GFunctionTraitNullType>::value;
	static constexpr bool IsMember = ! IsGlobal;

	static constexpr bool IsConst = ((flags & ftfConst) != 0);
	static constexpr bool IsVolatile = ((flags & ftfVolatile) != 0);
	static constexpr bool IsConstVolatile = ((flags & ftfConstVolatile) != 0);
};


} //namespace namespace functiontraits_internal

template <typename Signature>
struct GFunctionTraits
{
	typedef void FullType;
	typedef void FunctionType;
	typedef void FunctionPointer;
	typedef void ArgTypeList;
	typedef void ArgList;

	typedef void ObjectType;
	typedef void ResultType;

	static constexpr bool IsFunction = false;

	static constexpr int Arity = -1;
	static constexpr bool HasResult = false;

	static constexpr bool IsGlobal = false;
	static constexpr bool IsMember = false;

	static constexpr bool IsConst = false;
	static constexpr bool IsVolatile = false;
	static constexpr bool IsConstVolatile = false;
};

#define G_FT_DEFINE_TRAITS(CC, CCFlag) \
	template <typename RT, typename ...Parameters> \
	class GFunctionTraits <RT CC (Parameters...)> : public functiontraits_internal::GFunctionTraitsBase< \
			CCFlag, RT CC (Parameters...), GFunctionTraitNullType, RT, Parameters... \
		> \
	{}; \
	template <typename RT, typename ...Parameters> \
	class GFunctionTraits <RT (CC *) (Parameters...)> : public functiontraits_internal::GFunctionTraitsBase< \
			CCFlag, RT (CC *) (Parameters...), GFunctionTraitNullType, RT, Parameters... \
		> \
	{}; \
	template <typename OT, typename RT, typename ...Parameters> \
	class GFunctionTraits <RT (CC OT::*)(Parameters...)> : public functiontraits_internal::GFunctionTraitsBase< \
			CCFlag, RT (CC OT::*)(Parameters...), OT, RT, Parameters... \
		> \
	{}; \
	template <typename OT, typename RT, typename ...Parameters> \
	class GFunctionTraits <RT (CC OT::*)(Parameters...) const> : public functiontraits_internal::GFunctionTraitsBase< \
			CCFlag | functiontraits_internal::ftfConst, RT (CC OT::*)(Parameters...) const, OT, RT, Parameters... \
		> \
	{}; \
	template <typename OT, typename RT, typename ...Parameters> \
	class GFunctionTraits <RT (CC OT::*)(Parameters...) volatile> : public functiontraits_internal::GFunctionTraitsBase< \
			CCFlag | functiontraits_internal::ftfVolatile, RT (CC OT::*)(Parameters...) volatile, OT, RT, Parameters... \
		> \
	{}; \
	template <typename OT, typename RT, typename ...Parameters> \
	class GFunctionTraits <RT (CC OT::*)(Parameters...) const volatile> : public functiontraits_internal::GFunctionTraitsBase< \
			CCFlag | functiontraits_internal::ftfConstVolatile, RT (CC OT::*)(Parameters...) const volatile, OT, RT, Parameters... \
		> \
	{};


G_FT_DEFINE_TRAITS(GPP_EMPTY(), 0)	
#ifdef G_SUPPORT_STDCALL
	G_FT_DEFINE_TRAITS(__stdcall, functiontraits_internal::ftfStdCall)
#endif
#ifdef G_SUPPORT_FASTCALL	
	G_FT_DEFINE_TRAITS(__fastcall, functiontraits_internal::ftfFastCall)
#endif
#ifdef G_SUPPORT_NORETURN_ATTRIBUTE
    G_FT_DEFINE_TRAITS(__attribute__((noreturn)), functiontraits_internal::ftfNoReturn)
#endif


#undef G_FT_DEFINE_TRAITS

} //namespace cpgf


#endif

