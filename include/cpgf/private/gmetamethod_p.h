#ifndef __GMETAMETHOD_P_H
#define __GMETAMETHOD_P_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/gmetaconverter.h"
#include "cpgf/gpp.h"
#include "cpgf/gcallback.h"
#include "cpgf/gexception.h"


namespace cpgf {

namespace meta_internal {


#define REF_GETPARAM_HELPER(N, unused) \
	case N: return createMetaType<typename TypeList_GetWithDefault<typename CallbackT::TraitsType::ArgTypeList, N>::Result>();

#define REF_CALL_HELPER_CAST(N, unused) \
	GPP_COMMA_IF(N) fromVariant<typename FT::ArgList::Arg ## N, PolicyHasRule<Policy, GMetaRuleCopyConstReference<N> >::Result ? VarantCastCopyConstRef : VarantCastKeepConstRef>(*params[N])

#define REF_CALL_HELPER(N, unused) \
	template <typename CT, typename FT, typename RT, typename Policy> \
	struct GMetaMethodCallHelper<CT, FT, N, RT, Policy, false> { \
		static GVariant invoke(const CT & callback, GVariant const * const * params, size_t paramCount) { \
			(void)params; \
			(void)paramCount; \
			GVarTypeData typeData = GVarTypeData(); \
			deduceVariantType<RT>(typeData, ! PolicyHasRule<Policy, GMetaRuleParamNoncopyable<metaPolicyResultIndex> >::Result); \
			GVariant v; \
			variant_internal::InitVariant<! PolicyHasRule<Policy, GMetaRuleParamNoncopyable<metaPolicyResultIndex> >::Result>(v, typeData, static_cast<typename variant_internal::DeducePassType<RT>::PassType>(callback(GPP_REPEAT(N, REF_CALL_HELPER_CAST, GPP_EMPTY)))); \
			return v; \
		} \
	}; \
	template <typename CT, typename FT, typename Policy> \
	struct GMetaMethodCallHelper<CT, FT, N, void, Policy, false> { \
		static GVariant invoke(const CT & callback, GVariant const * const * params, size_t paramCount) { \
			(void)params; \
			(void)paramCount; \
			callback(GPP_REPEAT(N, REF_CALL_HELPER_CAST, GPP_EMPTY)); \
			return GVariant(); \
		} \
	};

std::string arityToName(int arity);

template <typename CT, typename FT, unsigned int N, typename RT, typename Policy, bool IsVariadic>
struct GMetaMethodCallHelper;

template <typename CT, typename FT, unsigned int N, typename RT, typename Policy>
struct GMetaMethodCallHelper <CT, FT, N, RT, Policy, true>
{
	GASSERT_STATIC(N == 1);

	static GVariant invoke(const CT & callback, GVariant const * const * params, size_t paramCount) {
		GMetaVariadicParam variadicParams;
		variadicParams.params = params;
		variadicParams.paramCount = paramCount;
		
		return createVariant<true, RT>(callback(&variadicParams), true);
	}
};

template <typename CT, typename FT, unsigned int N, typename Policy>
struct GMetaMethodCallHelper <CT, FT, N, void, Policy, true>
{
	GASSERT_STATIC(N == 1);

	static GVariant invoke(const CT & callback, GVariant const * const * params, size_t paramCount) {
		GMetaVariadicParam variadicParams;
		variadicParams.params = params;
		variadicParams.paramCount = paramCount;
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

class GMetaMethodDataBase
{
public:
	virtual ~GMetaMethodDataBase() {
	}

	virtual size_t getParamCount() const = 0;
	virtual bool hasResult() const = 0;
	virtual GMetaType getParamType(size_t index) const = 0;
	virtual GMetaType getResultType() const = 0;
	virtual bool isVariadic() const = 0;
	virtual GVariant invoke(void * instance, GVariant const * const * params, size_t paramCount) const = 0;
	virtual bool checkParam(const GVariant & param, size_t paramIndex) const = 0;
	virtual bool isParamTransferOwnership(size_t paramIndex) const = 0;
	virtual bool isResultTransferOwnership() const = 0;
	virtual GMetaConverter * createResultConverter() const = 0;
};

template<typename CallbackT, typename Policy>
class GMetaMethodData : public GMetaMethodDataBase
{
private:
    typedef GMetaMethodData<CallbackT, Policy> ThisType;
	typedef CallbackT CallbackType;
	typedef typename CallbackType::TraitsType TraitsType;
	typedef typename TraitsType::ArgTypeList ArgTypeList;

public:
	explicit GMetaMethodData(const CallbackType & cb, const Policy &) : callback(cb) {
	}

	virtual size_t getParamCount() const {
		return TraitsType::Arity;
	}

	virtual bool hasResult() const {
		return ! IsSameType<typename TraitsType::ResultType, void>::Result;
	}

	virtual GMetaType getParamType(size_t index) const {
		switch(index) {
			GPP_REPEAT(REF_MAX_ARITY, REF_GETPARAM_HELPER, GPP_EMPTY)

			default:
				raiseCoreException(Error_Meta_ParamOutOfIndex);
				return GMetaType();
		}
	}

	virtual GMetaType getResultType() const {
		return createMetaType<typename CallbackT::TraitsType::ResultType>();
	}
	
	virtual bool isVariadic() const {
		return IsVariadicFunction<TraitsType>::Result;
	}

	virtual GVariant invoke(void * instance, GVariant const * const * params, size_t paramCount) const {
		if(!this->isVariadic() && paramCount != this->getParamCount()) {
			raiseCoreException(Error_Meta_WrongArity, this->getParamCount(), paramCount);
		}

		this->callback.setObject(instance);
		return GMetaMethodCallHelper<CallbackType, TraitsType, TraitsType::Arity, typename TraitsType::ResultType, Policy, IsVariadicFunction<TraitsType>::Result>::invoke(this->callback, params, paramCount);
	}

	virtual bool checkParam(const GVariant & param, size_t paramIndex) const {
		if(this->isVariadic()) {
			return true;
		}

		if(paramIndex >= this->getParamCount()) {
			return false;
		}

#define REF_CHECKPARAM_HELPER(N, unused) \
	case N: return canFromVariant<typename TypeList_GetWithDefault<typename CallbackT::TraitsType::ArgTypeList, N>::Result>(param);

		switch(paramIndex) {
			GPP_REPEAT(REF_MAX_ARITY, REF_CHECKPARAM_HELPER, GPP_EMPTY)

			default:
				raiseCoreException(Error_Meta_ParamOutOfIndex);
				return false;
		}

#undef REF_CHECKPARAM_HELPER

	}

	virtual bool isParamTransferOwnership(size_t paramIndex) const {
		return policyHasIndexedRule<Policy, GMetaRuleTransferOwnership>(static_cast<int>(paramIndex));
	}

	virtual bool isResultTransferOwnership() const {
		return policyHasIndexedRule<Policy, GMetaRuleTransferOwnership>(metaPolicyResultIndex);
	}

	virtual GMetaConverter * createResultConverter() const {
		return GMetaConverterTraits<typename CallbackT::TraitsType::ResultType>::createConverter();
	}

public:
	CallbackType callback;
};


#define REF_PARAM_TYPEVALUE(N, P)		GPP_COMMA_IF(N) P ## N p ## N

template <typename OT, typename FuncOT, typename Enabled = void>
struct GMetaMethodCallbackMaker {
	enum { modifiers = 0 };

	template <typename FT>
	static typename FunctionCallbackType<FT>::Result make(const FT & func) {
		return makeCallback(static_cast<OT *>(NULL), func);
	}
};

template <typename OT, typename FuncOT>
struct GMetaMethodCallbackMaker <OT, FuncOT, typename GEnableIf<IsSameType<OT, void>::Result || IsSameType<FuncOT, void>::Result>::Result> {
	enum { modifiers = metaModifierStatic };

	template <typename FT>
	static typename FunctionCallbackType<FT>::Result make(const FT & func) {
		return makeCallback(func);
	}
};


#define REF_CONSTRUCTOR_INVOKE(N, unused) \
    template<typename OT, typename ArgsList> struct GMetaConstructorInvoker <N, OT, ArgsList> { \
        void * operator ()(GPP_REPEAT(N, REF_PARAM_TYPEVALUE, typename ArgsList::Arg)) const { \
            return new OT(GPP_REPEAT_PARAMS(N, p)); \
        } \
    };

template <int arity, typename OT, typename ArgsListType>
struct GMetaConstructorInvoker;

GPP_REPEAT_2(REF_MAX_ARITY, REF_CONSTRUCTOR_INVOKE, GPP_EMPTY)


} // namespace meta_internal


} // namespace cpgf


#undef REF_CALL_HELPER
#undef REF_CALL_HELPER_CAST

#undef REF_CALL_LOAD_PARAM
#undef REF_CONSTRUCTOR_INVOKE
#undef REF_GETPARAM_HELPER
#undef REF_PARAM_TYPEVALUE


#endif
