#ifndef __GMETAMETHOD_P_H
#define __GMETAMETHOD_P_H

#include "cpgf/private/gmetainvoke_p.h"
#include "cpgf/private/gmetadefaultparam_p.h"
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

struct GMetaMethodDataVirtual
{
	void (*deleteObject)(void * self);
	size_t (*getParamCount)(const void * self);
	bool (*hasResult)(const void * self);
	GMetaType (*getParamType)(const void * self, size_t index);
	GMetaType (*getResultType)(const void * self);
	bool (*isVariadic)(const void * self);
	bool (*isExplicitThis)(const void * self);
	GVariant (*invoke)(const void * self, void * instance, GVariant const * const * params, size_t paramCount);
	bool (*checkParam)(const void * self, const GVariant & param, size_t paramIndex);
	bool (*isParamTransferOwnership)(const void * self, size_t paramIndex);
	bool (*isResultTransferOwnership)(const void * self);
	GMetaConverter * (*createResultConverter)(const void * self);
};

class GMetaMethodDataBase
{
public:
	void deleteObject() {
		this->virtualFunctions->deleteObject(this);
	}

	size_t getParamCount() const {
		return this->virtualFunctions->getParamCount(this);
	}

	bool hasResult() const {
		return this->virtualFunctions->hasResult(this);
	}

	GMetaType getParamType(size_t index) const {
		return this->virtualFunctions->getParamType(this, index);
	}

	GMetaType getResultType() const {
		return this->virtualFunctions->getResultType(this);
	}

	bool isVariadic() const {
		return this->virtualFunctions->isVariadic(this);
	}

	bool isExplicitThis() const {
		return this->virtualFunctions->isExplicitThis(this);
	}

	GVariant invoke(void * instance, GVariant const * const * params, size_t paramCount) const {
		return this->virtualFunctions->invoke(this, instance, params, paramCount);
	}

	bool checkParam(const GVariant & param, size_t paramIndex) const {
		return this->virtualFunctions->checkParam(this, param, paramIndex);
	}

	bool isParamTransferOwnership(size_t paramIndex) const {
		return this->virtualFunctions->isParamTransferOwnership(this, paramIndex);
	}

	bool isResultTransferOwnership() const {
		return this->virtualFunctions->isResultTransferOwnership(this);
	}

	GMetaConverter * createResultConverter() const {
		return this->virtualFunctions->createResultConverter(this);
	}

	GMetaDefaultParamList * getDefaultParamList() const;
	bool hasDefaultParam() const;

protected:
	GMetaMethodDataVirtual * virtualFunctions;

private:
	mutable GScopedPointer<GMetaDefaultParamList> defaultParamList;
};

template<typename CallbackT, typename Policy>
class GMetaMethodData : public GMetaMethodDataBase
{
private:
    typedef GMetaMethodData<CallbackT, Policy> ThisType;
	typedef CallbackT CallbackType;
	typedef typename CallbackType::TraitsType TraitsType;
	typedef typename TraitsType::ArgTypeList ArgTypeList;

private:
	static size_t virtualGetParamCount(const void * self) {
		(void)self;

		return PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result ? TraitsType::Arity - 1 : TraitsType::Arity;
	}

	static bool virtualHasResult(const void * self) {
		(void)self;

		return ! IsSameType<typename TraitsType::ResultType, void>::Result;
	}

	static GMetaType virtualGetParamType(const void * self, size_t index) {
		(void)self;

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

	static GMetaType virtualGetResultType(const void * self) {
		(void)self;

		return createMetaType<typename CallbackT::TraitsType::ResultType>();
	}

	static bool virtualIsVariadic(const void * self) {
		(void)self;

		return IsVariadicFunction<TraitsType>::Result;
	}

	static bool virtualIsExplicitThis(const void * self) {
		(void)self;

		return PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result;
	}

	static GVariant virtualInvoke(const void * self, void * instance, GVariant const * const * params, size_t paramCount) {
		if(!(
				static_cast<const GMetaMethodData *>(self)->isVariadic()
				|| paramCount == static_cast<const GMetaMethodData *>(self)->getParamCount()
				|| (paramCount == static_cast<const GMetaMethodData *>(self)->getParamCount() - 1 && PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result)
			)
		) {
			raiseCoreException(Error_Meta_WrongArity, static_cast<const GMetaMethodData *>(self)->getParamCount(), paramCount);
		}

		static_cast<const GMetaMethodData *>(self)->callback.setObject(instance);
		return GMetaInvokeHelper<CallbackType,
			TraitsType,
			TraitsType::Arity,
			typename TraitsType::ResultType,
			Policy,
			IsVariadicFunction<TraitsType>::Result,
			PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result
		>::invoke(instance, static_cast<const GMetaMethodData *>(self)->callback, params, paramCount);
	}

	static bool virtualCheckParam(const void * self, const GVariant & param, size_t paramIndex) {
		if(static_cast<const GMetaMethodData *>(self)->isVariadic()) {
			return true;
		}

		if(paramIndex >= static_cast<const GMetaMethodData *>(self)->getParamCount()) {
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

	static bool virtualIsParamTransferOwnership(const void * self, size_t paramIndex) {
		(void)self;

		if(PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result) {
			++paramIndex;
		}
		return policyHasIndexedRule<Policy, GMetaRuleTransferOwnership>(static_cast<int>(paramIndex));
	}

	static bool virtualIsResultTransferOwnership(const void * self) {
		(void)self;

		return policyHasIndexedRule<Policy, GMetaRuleTransferOwnership>(metaPolicyResultIndex);
	}

	static GMetaConverter * virtualCreateResultConverter(const void * self) {
		(void)self;

		return GMetaConverterTraits<typename CallbackT::TraitsType::ResultType>::createConverter();
	}

public:
	GMetaMethodData(const CallbackType & cb, const Policy &) : callback(cb) {
		static GMetaMethodDataVirtual thisFunctions = {
			&virtualBaseMetaDeleter<GMetaMethodData>,
			&virtualGetParamCount,
			&virtualHasResult,
			&virtualGetParamType,
			&virtualGetResultType,
			&virtualIsVariadic,
			&virtualIsExplicitThis,
			&virtualInvoke,
			&virtualCheckParam,
			&virtualIsParamTransferOwnership,
			&virtualIsResultTransferOwnership,
			&virtualCreateResultConverter
		};

		this->virtualFunctions = &thisFunctions;
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
