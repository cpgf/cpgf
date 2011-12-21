#ifndef __GMETAINVOKE_P_H
#define __GMETAINVOKE_P_H

#include "cpgf/gpp.h"
#include "cpgf/gassert.h"
#include "cpgf/gvariant.h"
#include "cpgf/gmetacommon.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/gmetaconverter.h"
#include "cpgf/gcallback.h"
#include "cpgf/gexception.h"


namespace cpgf {

namespace meta_internal {


#define REF_CALL_HELPER_CAST(N, unused) \
	GPP_COMMA_IF(N) fromVariant<typename FT::ArgList::Arg ## N, PolicyHasRule<Policy, GMetaRuleCopyConstReference<N> >::Result ? VarantCastCopyConstRef : VarantCastKeepConstRef>(*params[N])

#define REF_CALL_HELPER_CAST_EXPLICIT_THIS_HELPER(N) \
	GPP_COMMA_IF(N) fromVariant<typename FT::ArgList::Arg ## N, PolicyHasRule<Policy, GMetaRuleCopyConstReference<N> >::Result ? VarantCastCopyConstRef : VarantCastKeepConstRef>(*params[N - 1])

#define REF_CALL_HELPER_CAST_EXPLICIT_THIS_EMPTY(N)

#define REF_CALL_HELPER_CAST_EXPLICIT_THIS(N, unused) GPP_IF(N, REF_CALL_HELPER_CAST_EXPLICIT_THIS_HELPER, REF_CALL_HELPER_CAST_EXPLICIT_THIS_EMPTY)(N)

#define REF_CALL_HELPER(N, unused) \
	template <typename CT, typename FT, typename RT, typename Policy> \
	struct GMetaInvokeHelper<CT, FT, N, RT, Policy, false, false> { \
		static GVariant invoke(void * instance, const CT & callback, GVariant const * const * params, size_t paramCount) { \
			(void)params; \
			(void)paramCount; \
			(void)instance; \
			GVarTypeData typeData = GVarTypeData(); \
			deduceVariantType<RT>(typeData, ! PolicyHasRule<Policy, GMetaRuleParamNoncopyable<metaPolicyResultIndex> >::Result); \
			GVariant v; \
			variant_internal::InitVariant<! PolicyHasRule<Policy, GMetaRuleParamNoncopyable<metaPolicyResultIndex> >::Result>(v, typeData, static_cast<typename variant_internal::DeducePassType<RT>::PassType>(callback(GPP_REPEAT(N, REF_CALL_HELPER_CAST, GPP_EMPTY)))); \
			return v; \
		} \
	}; \
	template <typename CT, typename FT, typename Policy> \
	struct GMetaInvokeHelper<CT, FT, N, void, Policy, false, false> { \
		static GVariant invoke(void * instance, const CT & callback, GVariant const * const * params, size_t paramCount) { \
			(void)params; \
			(void)paramCount; \
			(void)instance; \
			callback(GPP_REPEAT(N, REF_CALL_HELPER_CAST, GPP_EMPTY)); \
			return GVariant(); \
		} \
	}; \
	template <typename CT, typename FT, typename RT, typename Policy> \
	struct GMetaInvokeHelper<CT, FT, N, RT, Policy, false, true> { \
		static GVariant invoke(void * instance, const CT & callback, GVariant const * const * params, size_t paramCount) { \
			(void)params; \
			(void)paramCount; \
			GVarTypeData typeData = GVarTypeData(); \
			deduceVariantType<RT>(typeData, ! PolicyHasRule<Policy, GMetaRuleParamNoncopyable<metaPolicyResultIndex> >::Result); \
			GVariant v; \
			variant_internal::InitVariant<! PolicyHasRule<Policy, GMetaRuleParamNoncopyable<metaPolicyResultIndex> >::Result>(v, typeData, static_cast<typename variant_internal::DeducePassType<RT>::PassType>(callback((typename CT::TraitsType::ArgList::Arg0)(instance) GPP_REPEAT(N, REF_CALL_HELPER_CAST_EXPLICIT_THIS, GPP_EMPTY)))); \
			return v; \
		} \
	}; \
	template <typename CT, typename FT, typename Policy> \
	struct GMetaInvokeHelper<CT, FT, N, void, Policy, false, true> { \
		static GVariant invoke(void * instance, const CT & callback, GVariant const * const * params, size_t paramCount) { \
			(void)params; \
			(void)paramCount; \
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

	static GVariant invoke(void * instance, const CT & callback, GVariant const * const * params, size_t paramCount) {
		(void)instance;

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

	static GVariant invoke(void * instance, const CT & callback, GVariant const * const * params, size_t paramCount) {
		(void)instance;

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

template <typename FunctionTraits, typename Enabled = void>
struct IsVariadicFunction
{
	typedef FunctionTraits TraitsType;

	enum { Result = false };
};

template <typename FunctionTraits>
struct IsVariadicFunction <FunctionTraits, typename GEnableIf<(FunctionTraits::Arity == 1)>::Result>
{
	typedef FunctionTraits TraitsType;

	enum { Result = (IsSameType<typename TraitsType::ArgList::Arg0, GMetaVariadicParam *>::Result
		|| IsSameType<typename TraitsType::ArgList::Arg0, const GMetaVariadicParam *>::Result)
	};
};



} // namespace meta_internal


} // namespace cpgf


#undef REF_CALL_HELPER
#undef REF_CALL_HELPER_CAST
#undef REF_CALL_HELPER_CAST_EXPLICIT_THIS
#undef REF_CALL_HELPER_CAST_EXPLICIT_THIS_HELPER
#undef REF_CALL_HELPER_CAST_EXPLICIT_THIS_EMPTY


#endif
