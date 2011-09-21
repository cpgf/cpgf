#ifndef __GFUNCTIONTRAITS_H
#define __GFUNCTIONTRAITS_H


#include "cpgf/gcompiler.h"
#include "cpgf/gconfig.h"
#include "cpgf/gpp.h"
#include "cpgf/gtypelist.h"


#ifndef FT_MAX_ARITY
	#define FT_MAX_ARITY G_MAX_ARITY
#endif

#define FT_PARAM_TYPEVALUE(N, P)		GPP_COMMA_IF(N) P ## N  p ## N
#define FT_ARG_TYPEDEF(N, P)			typedef P ## N Arg ## N;

#define FT_DEF_PARAM_HELPER(N) \
	template <GPP_REPEAT(N, GPP_COMMA_PARAM, typename P) > \
	struct GFunctionArgs ## N { \
		GPP_REPEAT(N, FT_ARG_TYPEDEF, P) \
	};

#define FT_DEF_PARAM_HELPER_EMPTY(N)

#define FT_DEF_PARAM(N, CC) GPP_IF(N, FT_DEF_PARAM_HELPER, FT_DEF_PARAM_HELPER_EMPTY)(N)

#define FT_DEF_ARGTYPE(N) typedef GFunctionArgs ## N<GPP_REPEAT_PARAMS(N, P) > ArgList;
#define FT_DEF_ARGTYPE0(N) typedef GFunctionTraitNullType ArgList;

#define FT_DEF_COMMON(N) \
		typedef RT FunctionType(GPP_REPEAT(N, FT_PARAM_TYPEVALUE, P)); \
		typedef FunctionType * FunctionPointer; \
		enum { Arity = N }; \
		enum { IsFunction = true }; \
		enum { HasResult = GFunctionHasResult<RT>::Result }; \
		typedef RT ResultType; \
		GPP_IF(N, FT_DEF_ARGTYPE, FT_DEF_ARGTYPE0)(N) \
		typedef typename TypeList_Make<GPP_REPEAT_PARAMS(N, P) >::Result ArgTypeList;

#define FT_DEF_GLOBAL_HELPER(N, CC) \
	template <typename RT GPP_COMMA_IF(N) GPP_REPEAT(N, GPP_COMMA_PARAM, typename P) > \
	struct GFunctionTraitsBase <RT (CC *) (GPP_REPEAT_PARAMS(N, P)) > { \
		typedef RT (CC * FullType) (GPP_REPEAT_PARAMS(N, P)); \
		typedef GFunctionTraitNullType ObjectType; \
		enum { IsMember = false, IsConst = false, IsVolatile = false, IsConstVolatile = false }; \
		FT_DEF_COMMON(N) \
	}; \
	template <typename RT GPP_COMMA_IF(N) GPP_REPEAT(N, GPP_COMMA_PARAM, typename P) > \
	struct GFunctionTraitsBase <RT CC (GPP_REPEAT_PARAMS(N, P)) > { \
		typedef RT (CC * FullType) (GPP_REPEAT_PARAMS(N, P)); \
		typedef GFunctionTraitNullType ObjectType; \
		enum { IsMember = false, IsConst = false, IsVolatile = false, IsConstVolatile = false }; \
		FT_DEF_COMMON(N) \
	};

#define FT_DEF_GLOBAL(N, CC) \
	GPP_REPEAT_2(FT_MAX_ARITY, FT_DEF_GLOBAL_HELPER, CC)

#define FT_DEF_MEMBER_HELPER_CV(N, CC, CV, isC, isV, isCV) \
	template <typename OT, typename RT GPP_COMMA_IF(N) GPP_REPEAT(N, GPP_COMMA_PARAM, typename P) > \
	struct GFunctionTraitsBase <RT (CC OT::*) (GPP_REPEAT_PARAMS(N, P)) CV> { \
		typedef RT (CC OT::*FullType) (GPP_REPEAT_PARAMS(N, P)) CV; \
		typedef OT ObjectType; \
		enum { IsMember = true, IsConst = isC, IsVolatile = isV, IsConstVolatile = isCV }; \
		FT_DEF_COMMON(N) \
	};

#define FT_DEF_MEMBER_HELPER(N, CC) \
	FT_DEF_MEMBER_HELPER_CV(N, CC, GPP_EMPTY(), false, false, false) \
	FT_DEF_MEMBER_HELPER_CV(N, CC, const, true, false, false) \
	FT_DEF_MEMBER_HELPER_CV(N, CC, volatile, false, true, false) \
	FT_DEF_MEMBER_HELPER_CV(N, CC, const volatile, false, false, true) \

#define FT_DEF_MEMBER(N, CC) \
	GPP_REPEAT_2(FT_MAX_ARITY, FT_DEF_MEMBER_HELPER, CC)


namespace cpgf {

typedef void GFunctionTraitNullType;

namespace _internal {

template <typename T>
struct GFunctionHasResult
{
	enum { Result = true };
};

template <>
struct GFunctionHasResult <void>
{
	enum { Result = false };
};

GPP_REPEAT_2(FT_MAX_ARITY, FT_DEF_PARAM, GPP_EMPTY)

template <typename FT>
struct GFunctionTraitsBase
{
	enum { Arity = -1 };
	enum { IsFunction = false };
	enum { IsMember = false, IsConst = false, IsVolatile = false, IsConstVolatile = false };
};

FT_DEF_GLOBAL(FT_MAX_ARITY, GPP_EMPTY())

#ifdef G_SUPPORT_STDCALL
	FT_DEF_GLOBAL(FT_MAX_ARITY, __stdcall)
#endif
#ifdef G_SUPPORT_FASTCALL	
	FT_DEF_GLOBAL(FT_MAX_ARITY, __fastcall)
#endif

FT_DEF_MEMBER(FT_MAX_ARITY, GPP_EMPTY())

#ifdef G_SUPPORT_STDCALL
	FT_DEF_MEMBER(FT_MAX_ARITY, __stdcall)
#endif
#ifdef G_SUPPORT_FASTCALL	
	FT_DEF_MEMBER(FT_MAX_ARITY, __fastcall)
#endif

} // namespace _internal

template <typename Signature>
struct GFunctionTraits : public _internal::GFunctionTraitsBase<Signature>
{
};

template <typename FuncTrait, unsigned int N>
struct GArgumentType
{
	typedef typename TypeList_Get<typename FuncTrait::ArgTypeList, N>::Result Result;
};


} // namespace cpgf


#undef FT_MAX_ARITY
#undef FT_ARG_TYPEDEF
#undef FT_DEF_GLOBAL
#undef FT_DEF_COMMON
#undef FT_DEF_GLOBAL_HELPER
#undef FT_DEF_MEMBER
#undef FT_DEF_MEMBER_HELPER
#undef FT_DEF_MEMBER_HELPER_CV


#endif
