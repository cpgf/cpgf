#ifndef __GMETAMETHOD_P_H
#define __GMETAMETHOD_P_H

#include "cpgf/private/gmetainvoke_p.h"
#include "cpgf/gmetacommon.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/gmetaconverter.h"
#include "cpgf/gpp.h"
#include "cpgf/gcallback.h"
#include "cpgf/gexception.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif


namespace cpgf {

namespace meta_internal {


#define REF_GETPARAM_HELPER(N, unused) \
	case N: return createMetaType<typename TypeList_GetWithDefault<typename CallbackT::TraitsType::ArgTypeList, N>::Result>();

std::string arityToName(int arity);

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
	virtual bool isExplicitThis() const = 0;
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
		return PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result ? TraitsType::Arity - 1 : TraitsType::Arity;
	}

	virtual bool hasResult() const {
		return ! IsSameType<typename TraitsType::ResultType, void>::Result;
	}

	virtual GMetaType getParamType(size_t index) const {
		if(PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result) {
			++index;
		}
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

	virtual bool isExplicitThis() const {
		return PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result;
	}

	virtual GVariant invoke(void * instance, GVariant const * const * params, size_t paramCount) const {
		if(!(
				this->isVariadic()
				|| paramCount == this->getParamCount()
				|| (paramCount == this->getParamCount() - 1 && PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result)
			)
		) {
			raiseCoreException(Error_Meta_WrongArity, this->getParamCount(), paramCount);
		}

		this->callback.setObject(instance);
		return GMetaInvokeHelper<CallbackType,
			TraitsType,
			TraitsType::Arity,
			typename TraitsType::ResultType,
			Policy,
			IsVariadicFunction<TraitsType>::Result,
			PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result
		>::invoke(instance, this->callback, params, paramCount);
	}

	virtual bool checkParam(const GVariant & param, size_t paramIndex) const {
		if(this->isVariadic()) {
			return true;
		}

		if(paramIndex >= this->getParamCount()) {
			return false;
		}

		if(PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result) {
			++paramIndex;
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
		if(PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result) {
			++paramIndex;
		}
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

template <typename OT, typename FT, typename Enabled = void>
struct CheckIsStatic
{
	G_STATIC_CONSTANT(bool, Result = true);
};

template <typename OT, typename FT>
struct CheckIsStatic <OT, FT, typename GEnableIfResult<IsFunction<FT> >::Result>
{
	G_STATIC_CONSTANT(bool, Result = (IsSameType<OT, void>::Result || IsSameType<typename GFunctionTraits<FT>::ObjectType, void>::Result));
};

template <typename OT, typename FT, typename Enabled = void>
struct CheckIsFunction
{
	G_STATIC_CONSTANT(bool, Result = false);
};

template <typename OT, typename FT>
struct CheckIsFunction <OT, FT, typename GEnableIfResult<IsFunction<FT> >::Result>
{
	G_STATIC_CONSTANT(bool, Result = true);
};

template <typename OT, typename FT, typename Enabled = void>
struct CheckIsFunctor
{
	G_STATIC_CONSTANT(bool, IsFunctor = true);
};

template <typename OT, typename FT>
struct CheckIsFunctor <OT, FT, typename GEnableIfResult<IsFunction<FT> >::Result>
{
	G_STATIC_CONSTANT(bool, IsFunctor = false);
};

template <typename OT, typename FT, typename Enabled = void>
struct GMetaMethodCallbackMaker {
};

template <typename OT, typename FT>
struct GMetaMethodCallbackMaker <OT, FT,
	typename GEnableIfResult<
		GAndResult<
			CheckIsFunction<OT, FT>,
			GNotResult<CheckIsStatic<OT, FT> >
		>
	>::Result>
{
	enum { modifiers = 0 };

	template <typename F>
	static typename FunctionCallbackType<FT>::Result make(const F & func) {
		return makeCallback(static_cast<OT *>(NULL), func);
	}
};

template <typename OT, typename FT>
struct GMetaMethodCallbackMaker <OT, FT,
	typename GEnableIfResult<
		GAndResult<
			CheckIsFunction<OT, FT>,
			CheckIsStatic<OT, FT>
		>
	>::Result>
{
	enum { modifiers = metaModifierStatic };

	template <typename F>
	static typename FunctionCallbackType<FT>::Result make(const F & func) {
		return makeCallback(func);
	}
};

template <typename OT, typename FT>
struct GMetaMethodCallbackMaker <OT, GCallback<FT>,
	typename GEnableIfResult<
		GNotResult<CheckIsFunction<OT, GCallback<FT> > >
	>::Result>
{
	enum { modifiers = metaModifierStatic };

	static GCallback<FT> make(const GCallback<FT> & func) {
		return func;
	}
};


#define REF_PARAM_TYPEVALUE(N, P)		GPP_COMMA_IF(N) P ## N p ## N

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



#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif
