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
#endif


namespace cpgf {

namespace meta_internal {

template <typename Policy, int N>
struct SelectFromVariantPolicy
{
	typedef typename GIfElseResult<PolicyHasRule<Policy, GMetaRuleCopyConstReference<N> >, VarantCastCopyConstRef, VarantCastKeepConstRef>::Result Result;
};


#define REF_CALL_HELPER_CAST(N, unused) \
	GPP_COMMA_IF(N) fromVariant<typename FT::ArgList::Arg ## N, typename SelectFromVariantPolicy<Policy, N>::Result >(*params[N])

#define REF_CALL_HELPER_CAST_EXPLICIT_THIS_HELPER(N) \
	GPP_COMMA_IF(N) fromVariant<typename FT::ArgList::Arg ## N, typename SelectFromVariantPolicy<Policy, N>::Result >(*params[N - 1])

#define REF_CALL_HELPER_CAST_EXPLICIT_THIS_EMPTY(N)

#define REF_CALL_HELPER_CAST_EXPLICIT_THIS(N, unused) GPP_IF(N, REF_CALL_HELPER_CAST_EXPLICIT_THIS_HELPER, REF_CALL_HELPER_CAST_EXPLICIT_THIS_EMPTY)(N)

#define REF_CALL_HELPER(N, unused) \
	template <typename CT, typename FT, typename RT, typename Policy> \
	struct GMetaInvokeHelper<CT, FT, N, RT, Policy, false, false> { \
		static GVariant invoke(void * /*instance*/, const CT & callback, GVariant const * const * params, size_t /*paramCount*/) { \
			(void)params; /*unused when N == 0*/ \
			GVarTypeData typeData = GVarTypeData(); \
			deduceVariantType<RT>(typeData, ! PolicyHasRule<Policy, GMetaRuleParamNoncopyable<metaPolicyResultIndex> >::Result); \
			GVariant v; \
			variant_internal::InitVariant<! PolicyHasRule<Policy, GMetaRuleParamNoncopyable<metaPolicyResultIndex> >::Result, RT>(v, typeData, callback(GPP_REPEAT(N, REF_CALL_HELPER_CAST, GPP_EMPTY))); \
			return v; \
		} \
	}; \
	template <typename CT, typename FT, typename Policy> \
	struct GMetaInvokeHelper<CT, FT, N, void, Policy, false, false> { \
		static GVariant invoke(void * /*instance*/, const CT & callback, GVariant const * const * params, size_t /*paramCount*/) { \
			(void)params; /*unused when N == 0*/ \
			callback(GPP_REPEAT(N, REF_CALL_HELPER_CAST, GPP_EMPTY)); \
			return GVariant(); \
		} \
	}; \
	template <typename CT, typename FT, typename RT, typename Policy> \
	struct GMetaInvokeHelper<CT, FT, N, RT, Policy, false, true> { \
		static GVariant invoke(void * instance, const CT & callback, GVariant const * const * params, size_t /*paramCount*/) { \
			(void)params; /*unused when N == 0*/ \
			GVarTypeData typeData = GVarTypeData(); \
			deduceVariantType<RT>(typeData, ! PolicyHasRule<Policy, GMetaRuleParamNoncopyable<metaPolicyResultIndex> >::Result); \
			GVariant v; \
			variant_internal::InitVariant<! PolicyHasRule<Policy, GMetaRuleParamNoncopyable<metaPolicyResultIndex> >::Result, RT>(v, typeData, callback((typename CT::TraitsType::ArgList::Arg0)(instance) GPP_REPEAT(N, REF_CALL_HELPER_CAST_EXPLICIT_THIS, GPP_EMPTY))); \
			return v; \
		} \
	}; \
	template <typename CT, typename FT, typename Policy> \
	struct GMetaInvokeHelper<CT, FT, N, void, Policy, false, true> { \
		static GVariant invoke(void * instance, const CT & callback, GVariant const * const * params, size_t /*paramCount*/) { \
			(void)params; /*unused when N == 0*/ \
			callback((typename CT::TraitsType::ArgList::Arg0)(instance) GPP_REPEAT(N, REF_CALL_HELPER_CAST_EXPLICIT_THIS, GPP_EMPTY)); \
			return GVariant(); \
		} \
	};

template <typename CT, typename FT, unsigned int N, typename RT, typename Policy, bool IsVariadic, bool ExplicitThis>
struct GMetaInvokeHelper;

template <typename CT, typename FT, unsigned int N, typename RT, typename Policy>
struct GMetaInvokeHelper <CT, FT, N, RT, Policy, true, false>
{
	GASSERT_STATIC(N == 1);

	static GVariant invoke(void * /*instance*/, const CT & callback, GVariant const * const * params, size_t paramCount) {
		GMetaVariadicParam variadicParams;
		variadicParams.params = params;
		variadicParams.paramCount = paramCount;
		
		return createVariant<true, RT>(callback(&variadicParams), true);
	}
};

template <typename CT, typename FT, unsigned int N, typename Policy>
struct GMetaInvokeHelper <CT, FT, N, void, Policy, true, false>
{
	GASSERT_STATIC(N == 1);

	static GVariant invoke(void * /*instance*/, const CT & callback, GVariant const * const * params, size_t paramCount) {
		GMetaVariadicParam variadicParams;
		variadicParams.params = params;
		variadicParams.paramCount = paramCount;
		callback(&variadicParams);
		return GVariant();
	}
};

template <typename CT, typename FT, unsigned int N, typename RT, typename Policy>
struct GMetaInvokeHelper <CT, FT, N, RT, Policy, true, true>
{
	GASSERT_STATIC(N == 1);

	static GVariant invoke(void * instance, const CT & callback, GVariant const * const * params, size_t paramCount) {
		GVariant self(instance);
		GScopedArray<const GVariant *> tempParams(new const GVariant *[paramCount + 1]);
		
		tempParams[0] = &self;
		for(size_t i = 0; i < paramCount; ++i) {
			tempParams[i + 1] = params[i];
		}
		
		GMetaVariadicParam variadicParams;
		variadicParams.params = tempParams;
		variadicParams.paramCount = paramCount + 1;
		
		return createVariant<true, RT>(callback(&variadicParams), true);
	}
};

template <typename CT, typename FT, unsigned int N, typename Policy>
struct GMetaInvokeHelper <CT, FT, N, void, Policy, true, true>
{
	GASSERT_STATIC(N == 1);

	static GVariant invoke(void * instance, const CT & callback, GVariant const * const * params, size_t paramCount) {
		GVariant self(instance);
		GScopedArray<const GVariant *> tempParams(new const GVariant *[paramCount + 1]);
		
		tempParams[0] = &self;
		for(size_t i = 0; i < paramCount; ++i) {
			tempParams[i + 1] = params[i];
		}
		
		GMetaVariadicParam variadicParams;
		variadicParams.params = tempParams;
		variadicParams.paramCount = paramCount + 1;
		
		callback(&variadicParams);
		return GVariant();
	}
};

GPP_REPEAT_2(REF_MAX_ARITY, REF_CALL_HELPER, GPP_EMPTY)

template <typename FunctionTraits>
struct CheckSingleArity
{
	G_STATIC_CONSTANT(bool, Result = (FunctionTraits::Arity == 1));
};

template <typename FunctionTraits, typename Enabled = void>
struct IsVariadicFunction
{
	typedef FunctionTraits TraitsType;

	G_STATIC_CONSTANT(bool, Result = false);
};

template <typename FunctionTraits>
struct IsVariadicFunction <FunctionTraits, typename GEnableIfResult<CheckSingleArity<FunctionTraits> >::Result>
{
	typedef FunctionTraits TraitsType;

	G_STATIC_CONSTANT(bool, Result = (IsSameType<typename TraitsType::ArgList::Arg0, GMetaVariadicParam *>::Result
		|| IsSameType<typename TraitsType::ArgList::Arg0, const GMetaVariadicParam *>::Result)
	);
};



} // namespace meta_internal


} // namespace cpgf


#undef REF_CALL_HELPER
#undef REF_CALL_HELPER_CAST
#undef REF_CALL_HELPER_CAST_EXPLICIT_THIS
#undef REF_CALL_HELPER_CAST_EXPLICIT_THIS_HELPER
#undef REF_CALL_HELPER_CAST_EXPLICIT_THIS_EMPTY


#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif
