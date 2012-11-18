#ifndef __GMETAINVOKE_P_H
#define __GMETAINVOKE_P_H

#include "cpgf/gpp.h"
#include "cpgf/gassert.h"
#include "cpgf/gifelse.h"
#include "cpgf/gvariant.h"
#include "cpgf/gmetacommon.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/gcallback.h"
#include "cpgf/gexception.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4267) // weird warning when reflecting std::vector in VC, disable it.
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif


namespace cpgf {

extern size_t abstractParameterIndexBase;

namespace meta_internal {

template <typename Policy, int N>
struct SelectFromVariantPolicy
{
	typedef typename GIfElseResult<PolicyHasRule<Policy, GMetaRuleCopyConstReference<N> >, VarantCastCopyConstRef, VarantCastKeepConstRef>::Result Result;
};

template <typename Policy>
struct PolicyIsCopyable
{
	G_STATIC_CONSTANT(bool, Result = (! PolicyHasRule<Policy, GMetaRuleParamNoncopyable<metaPolicyResultIndex> >::Result));
};


template <typename CT, typename FT, unsigned int N, typename RT, typename Policy, bool IsVariadic, bool ExplicitThis>
struct GMetaInvokeHelper;

#define REF_CALL_HELPER_CAST(N, unused) \
	GPP_COMMA_IF(N) fromVariant<typename FT::ArgList::Arg ## N, typename SelectFromVariantPolicy<Policy, N>::Result >(*params[N])

#define REF_CALL_HELPER_CAST_EXPLICIT_THIS_HELPER(N) \
	GPP_COMMA() fromVariant<typename FT::ArgList::Arg ## N, typename SelectFromVariantPolicy<Policy, N>::Result >(*params[N - 1])
#define REF_CALL_HELPER_CAST_EXPLICIT_THIS_EMPTY(N)
#define REF_CALL_HELPER_CAST_EXPLICIT_THIS(N, unused) GPP_IF(N, REF_CALL_HELPER_CAST_EXPLICIT_THIS_HELPER, REF_CALL_HELPER_CAST_EXPLICIT_THIS_EMPTY)(N)

#define REF_CALL_HELPER(N, unused) \
	template <typename CT, typename FT, typename RT, typename Policy> \
	struct GMetaInvokeHelper<CT, FT, N, RT, Policy, false, false> { \
		static GVariant invoke(void * /*instance*/, const CT & callback, GVariant const * const * params, size_t /*paramCount*/) { \
			(void)params; /*unused when N == 0*/ \
			return createVariant<PolicyIsCopyable<Policy>::Result, RT>(callback(GPP_REPEAT(N, REF_CALL_HELPER_CAST, GPP_EMPTY)), PolicyIsCopyable<Policy>::Result); \
		} \
		static GVariant invoke(void * /*instance*/, CT & callback, GVariant const * const * params, size_t /*paramCount*/) { \
			(void)params; /*unused when N == 0*/ \
			return createVariant<PolicyIsCopyable<Policy>::Result, RT>(callback(GPP_REPEAT(N, REF_CALL_HELPER_CAST, GPP_EMPTY)), PolicyIsCopyable<Policy>::Result); \
		} \
	}; \
	template <typename CT, typename FT, typename Policy> \
	struct GMetaInvokeHelper<CT, FT, N, void, Policy, false, false> { \
		static GVariant invoke(void * /*instance*/, const CT & callback, GVariant const * const * params, size_t /*paramCount*/) { \
			(void)params; /*unused when N == 0*/ \
			callback(GPP_REPEAT(N, REF_CALL_HELPER_CAST, GPP_EMPTY)); \
			return GVariant(); \
		} \
		static GVariant invoke(void * /*instance*/, CT & callback, GVariant const * const * params, size_t /*paramCount*/) { \
			(void)params; /*unused when N == 0*/ \
			callback(GPP_REPEAT(N, REF_CALL_HELPER_CAST, GPP_EMPTY)); \
			return GVariant(); \
		} \
	}; \
	template <typename CT, typename FT, typename RT, typename Policy> \
	struct GMetaInvokeHelper<CT, FT, N, RT, Policy, false, true> { \
		static GVariant invoke(void * instance, const CT & callback, GVariant const * const * params, size_t /*paramCount*/) { \
			(void)params; /*unused when N == 0*/ \
			return createVariant<PolicyIsCopyable<Policy>::Result, RT>(callback((typename CT::TraitsType::ArgList::Arg0)(instance) GPP_REPEAT(N, REF_CALL_HELPER_CAST_EXPLICIT_THIS, GPP_EMPTY)), PolicyIsCopyable<Policy>::Result); \
		} \
	}; \
	template <typename CT, typename FT, typename Policy> \
	struct GMetaInvokeHelper<CT, FT, N, void, Policy, false, true> { \
		static GVariant invoke(void * instance, const CT & callback, GVariant const * const * params, size_t /*paramCount*/) { \
			(void)params; /*unused when N == 0*/ \
			callback((typename CT::TraitsType::ArgList::Arg0)(instance) GPP_REPEAT(N, REF_CALL_HELPER_CAST_EXPLICIT_THIS, GPP_EMPTY)); \
			return GVariant(); \
		} \
		static GVariant invoke(void * instance, CT & callback, GVariant const * const * params, size_t /*paramCount*/) { \
			(void)params; /*unused when N == 0*/ \
			callback((typename CT::TraitsType::ArgList::Arg0)(instance) GPP_REPEAT(N, REF_CALL_HELPER_CAST_EXPLICIT_THIS, GPP_EMPTY)); \
			return GVariant(); \
		} \
	};

#define REF_CALL_HELPER_CAST_VARIADIC_HELPER(N) \
	fromVariant<typename FT::ArgList::Arg ## N, typename SelectFromVariantPolicy<Policy, N>::Result >(*params[N]) GPP_COMMA()
#define REF_CALL_HELPER_CAST_VARIADIC_EMPTY(N)
//#define REF_CALL_HELPER_CAST_VARIADIC(N, unused) GPP_IF(N, REF_CALL_HELPER_CAST_VARIADIC_HELPER, REF_CALL_HELPER_CAST_VARIADIC_EMPTY)(N)
#define REF_CALL_HELPER_CAST_VARIADIC(N, unused) \
	fromVariant<typename FT::ArgList::Arg ## N, typename SelectFromVariantPolicy<Policy, N>::Result >(*params[N]) GPP_COMMA()

#define I_REF_CALL_VARIADIC_HELPER(N, M) \
	template <typename CT, typename FT, typename RT, typename Policy> \
	struct GMetaInvokeHelper <CT, FT, N, RT, Policy, true, false> { \
		static GVariant invoke(void * /*instance*/, const CT & callback, GVariant const * const * params, size_t paramCount) { \
			GMetaVariadicParam variadicParams; \
			variadicParams.params = &params[N -1]; \
			variadicParams.paramCount = paramCount - (N - 1); \
			return createVariant<PolicyIsCopyable<Policy>::Result, RT>(callback(GPP_REPEAT(M, REF_CALL_HELPER_CAST_VARIADIC, GPP_EMPTY) &variadicParams), PolicyIsCopyable<Policy>::Result); \
		} \
		static GVariant invoke(void * /*instance*/, CT & callback, GVariant const * const * params, size_t paramCount) { \
			GMetaVariadicParam variadicParams; \
			variadicParams.params = &params[N -1]; \
			variadicParams.paramCount = paramCount - (N - 1); \
			return createVariant<PolicyIsCopyable<Policy>::Result, RT>(callback(GPP_REPEAT(M, REF_CALL_HELPER_CAST_VARIADIC, GPP_EMPTY) &variadicParams), PolicyIsCopyable<Policy>::Result); \
		} \
	}; \
	template <typename CT, typename FT, typename Policy> \
	struct GMetaInvokeHelper <CT, FT, N, void, Policy, true, false> { \
		static GVariant invoke(void * /*instance*/, const CT & callback, GVariant const * const * params, size_t paramCount) { \
			GMetaVariadicParam variadicParams; \
			variadicParams.params = &params[N -1]; \
			variadicParams.paramCount = paramCount - (N - 1); \
			callback(GPP_REPEAT(M, REF_CALL_HELPER_CAST_VARIADIC, GPP_EMPTY) &variadicParams); \
			return GVariant(); \
		} \
		static GVariant invoke(void * /*instance*/, CT & callback, GVariant const * const * params, size_t paramCount) { \
			GMetaVariadicParam variadicParams; \
			variadicParams.params = &params[N -1]; \
			variadicParams.paramCount = paramCount - (N - 1); \
			callback(GPP_REPEAT(M, REF_CALL_HELPER_CAST_VARIADIC, GPP_EMPTY) &variadicParams); \
			return GVariant(); \
		} \
	};
#define REF_CALL_VARIADIC_HELPER_EMPTY(N, M)
#define REF_CALL_VARIADIC_HELPER(N, unused) GPP_IF(N, I_REF_CALL_VARIADIC_HELPER, REF_CALL_VARIADIC_HELPER_EMPTY)(N, GPP_DEC(N))

#define REF_CALL_HELPER_CAST_VARIADIC_EXPLICIT_THIS_HELPER(N) \
	fromVariant<typename FT::ArgList::Arg ## N, typename SelectFromVariantPolicy<Policy, N>::Result >(*params[N - 1]) GPP_COMMA()
#define REF_CALL_HELPER_CAST_VARIADIC_EXPLICIT_THIS_EMPTY(N)
#define REF_CALL_HELPER_CAST_VARIADIC_EXPLICIT_THIS(N, unused) GPP_IF(N, REF_CALL_HELPER_CAST_VARIADIC_EXPLICIT_THIS_HELPER, REF_CALL_HELPER_CAST_VARIADIC_EXPLICIT_THIS_EMPTY)(N)

#define I_REF_CALL_VARIADIC_EXPLICIT_THIS_HELPER(N, M) \
	template <typename CT, typename FT, typename RT, typename Policy> \
	struct GMetaInvokeHelper <CT, FT, N, RT, Policy, true, true> { \
		static GVariant invoke(void * instance, const CT & callback, GVariant const * const * params, size_t paramCount) { \
			GMetaVariadicParam variadicParams; \
			variadicParams.params = &params[N - 2]; \
			variadicParams.paramCount = paramCount - (M - 1); \
			return createVariant<PolicyIsCopyable<Policy>::Result, RT>(callback((typename CT::TraitsType::ArgList::Arg0)(instance), GPP_REPEAT(M, REF_CALL_HELPER_CAST_VARIADIC_EXPLICIT_THIS, GPP_EMPTY) &variadicParams), PolicyIsCopyable<Policy>::Result); \
		} \
		static GVariant invoke(void * instance, CT & callback, GVariant const * const * params, size_t paramCount) { \
			GMetaVariadicParam variadicParams; \
			variadicParams.params = &params[N - 2]; \
			variadicParams.paramCount = paramCount - (M - 1); \
			return createVariant<PolicyIsCopyable<Policy>::Result, RT>(callback((typename CT::TraitsType::ArgList::Arg0)(instance), GPP_REPEAT(M, REF_CALL_HELPER_CAST_VARIADIC_EXPLICIT_THIS, GPP_EMPTY) &variadicParams), PolicyIsCopyable<Policy>::Result); \
		} \
	}; \
	template <typename CT, typename FT, typename Policy> \
	struct GMetaInvokeHelper <CT, FT, N, void, Policy, true, true> { \
		static GVariant invoke(void * instance, const CT & callback, GVariant const * const * params, size_t paramCount) { \
			GMetaVariadicParam variadicParams; \
			variadicParams.params = &params[N - 2]; \
			variadicParams.paramCount = paramCount - (M - 1); \
			callback((typename CT::TraitsType::ArgList::Arg0)(instance), GPP_REPEAT(M, REF_CALL_HELPER_CAST_VARIADIC_EXPLICIT_THIS, GPP_EMPTY) &variadicParams); \
			return GVariant(); \
		} \
		static GVariant invoke(void * instance, CT & callback, GVariant const * const * params, size_t paramCount) { \
			GMetaVariadicParam variadicParams; \
			variadicParams.params = &params[N - 2]; \
			variadicParams.paramCount = paramCount - (M - 1); \
			callback((typename CT::TraitsType::ArgList::Arg0)(instance), GPP_REPEAT(M, REF_CALL_HELPER_CAST_VARIADIC_EXPLICIT_THIS, GPP_EMPTY) &variadicParams); \
			return GVariant(); \
		} \
	};
#define REF_CALL_VARIADIC_EXPLICIT_THIS_HELPER_EMPTY(N, unused)
#define REF_CALL_VARIADIC_EXPLICIT_THIS_HELPER(N, unused) GPP_IF(N, I_REF_CALL_VARIADIC_EXPLICIT_THIS_HELPER, REF_CALL_VARIADIC_EXPLICIT_THIS_HELPER_EMPTY)(N, GPP_DEC(N))

GPP_REPEAT_2(REF_MAX_ARITY, REF_CALL_HELPER, GPP_EMPTY)
GPP_REPEAT_2(REF_MAX_ARITY, REF_CALL_VARIADIC_HELPER, GPP_EMPTY)
GPP_REPEAT_2(REF_MAX_ARITY, REF_CALL_VARIADIC_EXPLICIT_THIS_HELPER, GPP_EMPTY)

template <typename FunctionTraits>
struct CheckVariadicArity
{
	G_STATIC_CONSTANT(bool, Result = (FunctionTraits::Arity >= 1));
};

template <typename FunctionTraits, typename Enabled = void>
struct IsVariadicFunction
{
	typedef FunctionTraits TraitsType;

	G_STATIC_CONSTANT(bool, Result = false);
};

template <typename FunctionTraits>
struct IsVariadicFunction <FunctionTraits, typename GEnableIfResult<CheckVariadicArity<FunctionTraits> >::Result>
{
	typedef FunctionTraits TraitsType;
	typedef typename TypeList_Get<typename TraitsType::ArgTypeList, TraitsType::Arity - 1>::Result VariadicArg;

	G_STATIC_CONSTANT(bool, Result = (IsSameType<VariadicArg, GMetaVariadicParam *>::Result
		|| IsSameType<VariadicArg, const GMetaVariadicParam *>::Result)
	);
};

void checkInvokingArity(size_t invokingParamCount, size_t prototypeParamCount, bool isVariadic);

template <typename Policy>
void adjustParamIndex(size_t & index)
{
	if(index >= abstractParameterIndexBase) {
		index -= abstractParameterIndexBase;
	}
	else {
		if(PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result) {
			++index;
		}
	}
}


} // namespace meta_internal


} // namespace cpgf


#undef REF_CALL_HELPER
#undef REF_CALL_VARIADIC_HELPER
#undef REF_CALL_HELPER_CAST
#undef REF_CALL_HELPER_CAST_EXPLICIT_THIS
#undef REF_CALL_HELPER_CAST_EXPLICIT_THIS_HELPER
#undef REF_CALL_HELPER_CAST_EXPLICIT_THIS_EMPTY
#undef REF_CALL_HELPER_CAST_VARIADIC_HELPER
#undef REF_CALL_HELPER_CAST_VARIADIC_EMPTY
#undef REF_CALL_HELPER_CAST_VARIADIC
#undef I_REF_CALL_VARIADIC_EXPLICIT_THIS_HELPER
#undef REF_CALL_VARIADIC_EXPLICIT_THIS_HELPER_EMPTY
#undef REF_CALL_VARIADIC_EXPLICIT_THIS_HELPER
#undef I_REF_CALL_VARIADIC_HELPER
#undef REF_CALL_HELPER_CAST_VARIADIC_EXPLICIT_THIS
#undef REF_CALL_HELPER_CAST_VARIADIC_EXPLICIT_THIS_EMPTY
#undef REF_CALL_HELPER_CAST_VARIADIC_EXPLICIT_THIS_HELPER
#undef REF_CALL_VARIADIC_HELPER_EMPTY

#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif
