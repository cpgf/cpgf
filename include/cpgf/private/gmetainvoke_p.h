#ifndef CPGF_GMETAINVOKE_P_H
#define CPGF_GMETAINVOKE_P_H

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

template <typename CT, typename RT, typename Policy, typename... Parameters>
typename std::enable_if<! std::is_void<RT>::value, GVariant>::type
doInvokeMetaCallable(const CT & callback, Parameters && ... parameters)
{
	return createVariant<RT>(callback(std::forward<Parameters>(parameters)...), PolicyIsCopyable<Policy>::Result);
}

template <typename CT, typename RT, typename Policy, typename... Parameters>
typename std::enable_if<std::is_void<RT>::value, GVariant>::type
doInvokeMetaCallable(const CT & callback, Parameters && ... parameters)
{
	callback(std::forward<Parameters>(parameters)...);
	return GVariant();
}

template <typename CT, typename FT, unsigned int N, typename RT, typename Policy, bool IsVariadic, bool ExplicitThis>
struct GMetaInvokeHelper;

#define REF_CALL_HELPER_CAST(N, unused) GPP_COMMA() fromVariant<typename FT::ArgList::Arg ## N, typename SelectFromVariantPolicy<Policy, N>::Result>(*params[N])
#define REF_CALL_HELPER_CAST_BY_DATA(N, unused) GPP_COMMA() fromVariantData<typename FT::ArgList::Arg ## N, typename SelectFromVariantPolicy<Policy, N>::Result>(*params[N])

#define REF_CALL_HELPER_CAST_EXPLICIT_THIS_HELPER(N) GPP_COMMA() fromVariant<typename FT::ArgList::Arg ## N, typename SelectFromVariantPolicy<Policy, N>::Result>(*params[N - 1])
#define REF_CALL_HELPER_CAST_EXPLICIT_THIS(N, unused) GPP_IF(N, REF_CALL_HELPER_CAST_EXPLICIT_THIS_HELPER, GPP_EMPTY_N)(N)

#define REF_CALL_HELPER_CAST_BY_DATA_EXPLICIT_THIS_HELPER(N) GPP_COMMA() fromVariantData<typename FT::ArgList::Arg ## N, typename SelectFromVariantPolicy<Policy, N>::Result>(*params[N - 1])
#define REF_CALL_HELPER_CAST_BY_DATA_EXPLICIT_THIS(N, unused) GPP_IF(N, REF_CALL_HELPER_CAST_BY_DATA_EXPLICIT_THIS_HELPER, GPP_EMPTY_N)(N)

#define REF_CALL_HELPER(N, unused) \
	template <typename CT, typename FT, typename RT, typename Policy> \
	struct GMetaInvokeHelper<CT, FT, N, RT, Policy, false, false> { \
		static GVariant invoke(void * /*instance*/, const CT & callback, GVariant const * const * params, size_t /*paramCount*/) { \
			(void)params; /*unused when N == 0*/ \
			return doInvokeMetaCallable<CT, RT, Policy>(callback GPP_REPEAT(N, REF_CALL_HELPER_CAST, GPP_EMPTY)); \
		} \
		static GVariant invokeByData(void * /*instance*/, const CT & callback, GVariantData const * const * params, size_t /*paramCount*/) { \
			(void)params; /*unused when N == 0*/ \
			return doInvokeMetaCallable<CT, RT, Policy>(callback GPP_REPEAT(N, REF_CALL_HELPER_CAST_BY_DATA, GPP_EMPTY)); \
		} \
	}; \
	template <typename CT, typename FT, typename RT, typename Policy> \
	struct GMetaInvokeHelper<CT, FT, N, RT, Policy, false, true> { \
		static GVariant invoke(void * instance, const CT & callback, GVariant const * const * params, size_t /*paramCount*/) { \
			(void)params; /*unused when N == 0*/ \
			return doInvokeMetaCallable<CT, RT, Policy>(callback, (typename CT::TraitsType::ArgList::Arg0)(instance) GPP_REPEAT(N, REF_CALL_HELPER_CAST_EXPLICIT_THIS, GPP_EMPTY)); \
		} \
		static GVariant invokeByData(void * instance, const CT & callback, GVariantData const * const * params, size_t /*paramCount*/) { \
			(void)params; /*unused when N == 0*/ \
			return doInvokeMetaCallable<CT, RT, Policy>(callback, (typename CT::TraitsType::ArgList::Arg0)(instance) GPP_REPEAT(N, REF_CALL_HELPER_CAST_BY_DATA_EXPLICIT_THIS, GPP_EMPTY)); \
		} \
	};


#define REF_CALL_HELPER_CAST_VARIADIC(N, unused) fromVariant<typename FT::ArgList::Arg ## N, typename SelectFromVariantPolicy<Policy, N>::Result>(*params[N]) GPP_COMMA()
#define REF_CALL_HELPER_CAST_BY_DATA_VARIADIC(N, unused) fromVariantData<typename FT::ArgList::Arg ## N, typename SelectFromVariantPolicy<Policy, N>::Result>(*params[N]) GPP_COMMA()

#define I_REF_CALL_VARIADIC_HELPER(N, M) \
	template <typename CT, typename FT, typename RT, typename Policy> \
	struct GMetaInvokeHelper <CT, FT, N, RT, Policy, true, false> { \
		static GVariant invoke(void * /*instance*/, const CT & callback, GVariant const * const * params, size_t paramCount) { \
			GMetaVariadicParam variadicParams; \
			variadicParams.params = &params[N -1]; \
			variadicParams.paramCount = paramCount - (N - 1); \
			return doInvokeMetaCallable<CT, RT, Policy>(callback, GPP_REPEAT(M, REF_CALL_HELPER_CAST_VARIADIC, GPP_EMPTY) &variadicParams); \
		} \
		static GVariant invokeByData(void * /*instance*/, const CT & callback, GVariantData const * const * params, size_t paramCount) { \
			GMetaVariadicParam variadicParams; \
			GVariant buffer[REF_MAX_ARITY]; \
			GVariant * pointerBuffer[REF_MAX_ARITY]; \
			for(size_t i = N - 1; i < paramCount; ++i) { buffer[i - (N - 1)] = createVariantFromData(*params[i]); pointerBuffer[i - (N - 1)] = &buffer[i - (N - 1)]; } \
			variadicParams.params = pointerBuffer; \
			variadicParams.paramCount = paramCount - (N - 1); \
			return doInvokeMetaCallable<CT, RT, Policy>(callback, GPP_REPEAT(M, REF_CALL_HELPER_CAST_BY_DATA_VARIADIC, GPP_EMPTY) &variadicParams); \
		} \
	};
#define REF_CALL_VARIADIC_HELPER(N, unused) GPP_IF(N, I_REF_CALL_VARIADIC_HELPER, GPP_EMPTY_N)(N, GPP_DEC(N))


#define REF_CALL_HELPER_CAST_VARIADIC_EXPLICIT_THIS_HELPER(N) fromVariant<typename FT::ArgList::Arg ## N, typename SelectFromVariantPolicy<Policy, N>::Result>(*params[N - 1]) GPP_COMMA()
#define REF_CALL_HELPER_CAST_VARIADIC_EXPLICIT_THIS(N, unused) GPP_IF(N, REF_CALL_HELPER_CAST_VARIADIC_EXPLICIT_THIS_HELPER, GPP_EMPTY_N)(N)

#define REF_CALL_HELPER_CAST_BY_DATA_VARIADIC_EXPLICIT_THIS_HELPER(N) fromVariantData<typename FT::ArgList::Arg ## N, typename SelectFromVariantPolicy<Policy, N>::Result>(*params[N - 1]) GPP_COMMA()
#define REF_CALL_HELPER_CAST_BY_DATA_VARIADIC_EXPLICIT_THIS(N, unused) GPP_IF(N, REF_CALL_HELPER_CAST_BY_DATA_VARIADIC_EXPLICIT_THIS_HELPER, GPP_EMPTY_N)(N)

#define I_REF_CALL_VARIADIC_EXPLICIT_THIS_HELPER(N, M) \
	template <typename CT, typename FT, typename RT, typename Policy> \
	struct GMetaInvokeHelper <CT, FT, N, RT, Policy, true, true> { \
		static GVariant invoke(void * instance, const CT & callback, GVariant const * const * params, size_t paramCount) { \
			GMetaVariadicParam variadicParams; \
			variadicParams.params = &params[N - 2]; \
			variadicParams.paramCount = paramCount - (M - 1); \
			return doInvokeMetaCallable<CT, RT, Policy>(callback, (typename CT::TraitsType::ArgList::Arg0)(instance), GPP_REPEAT(M, REF_CALL_HELPER_CAST_VARIADIC_EXPLICIT_THIS, GPP_EMPTY) &variadicParams); \
		} \
		static GVariant invokeByData(void * instance, const CT & callback, GVariantData const * const * params, size_t paramCount) { \
			GMetaVariadicParam variadicParams; \
			GVariant buffer[REF_MAX_ARITY]; \
			GVariant * pointerBuffer[REF_MAX_ARITY]; \
			for(size_t i = N - 2; i < paramCount; ++i) { buffer[i - (N - 2)] = createVariantFromData(*params[i]); pointerBuffer[i - (N - 2)] = &buffer[i - (N - 2)]; } \
			variadicParams.params = pointerBuffer; \
			variadicParams.paramCount = paramCount - (M - 1); \
			return doInvokeMetaCallable<CT, RT, Policy>(callback, (typename CT::TraitsType::ArgList::Arg0)(instance), GPP_REPEAT(M, REF_CALL_HELPER_CAST_BY_DATA_VARIADIC_EXPLICIT_THIS, GPP_EMPTY) &variadicParams); \
		} \
	};
#define REF_CALL_VARIADIC_EXPLICIT_THIS_HELPER(N, unused) GPP_IF(N, I_REF_CALL_VARIADIC_EXPLICIT_THIS_HELPER, GPP_EMPTY_N)(N, GPP_DEC(N))

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
void adjustParamIndex(size_t & index, bool isExplicitThis);


} // namespace meta_internal


} // namespace cpgf


#undef REF_CALL_HELPER
#undef REF_CALL_VARIADIC_HELPER
#undef REF_CALL_HELPER_CAST
#undef REF_CALL_HELPER_CAST_EXPLICIT_THIS
#undef REF_CALL_HELPER_CAST_EXPLICIT_THIS_HELPER
#undef REF_CALL_HELPER_CAST_VARIADIC
#undef I_REF_CALL_VARIADIC_EXPLICIT_THIS_HELPER
#undef REF_CALL_VARIADIC_EXPLICIT_THIS_HELPER
#undef I_REF_CALL_VARIADIC_HELPER
#undef REF_CALL_HELPER_CAST_VARIADIC_EXPLICIT_THIS
#undef REF_CALL_HELPER_CAST_VARIADIC_EXPLICIT_THIS_HELPER

#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif
