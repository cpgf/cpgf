#ifndef CPGF_GMETAMETHOD_P_H
#define CPGF_GMETAMETHOD_P_H

#include "cpgf/private/gmetainvoke_p.h"
#include "cpgf/private/gmetadefaultparam_p.h"
#include "cpgf/gmetacommon.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/gtypeutil.h"
#include "cpgf/gpp.h"
#include "cpgf/gcallback.h"
#include "cpgf/gexception.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif


namespace cpgf {

extern int Error_Meta_ParamOutOfIndex;

namespace meta_internal {

std::string arityToName(int arity);

struct GMetaMethodDataVirtual
{
	void (*deleteSelf)(void * self);
	size_t (*getParamCount)();
	bool (*hasResult)();
	GMetaType (*getParamType)(size_t index);
	GMetaType (*getResultType)();
	GMetaExtendType (*getResultExtendType)(uint32_t flags);
	GMetaExtendType (*getParamExtendType)(uint32_t flags, size_t index);
	bool (*isVariadic)();
	bool (*isExplicitThis)();
	GVariant (*invoke)(const void * self, void * instance, GVariant const * const * params, size_t paramCount);
	GVariant (*invokeByData)(const void * self, void * instance, GVariantData const * const * params, size_t paramCount);
	bool (*checkParam)(const GVariant & param, size_t paramIndex);
	bool (*isParamTransferOwnership)(size_t paramIndex);
	bool (*isResultTransferOwnership)();
	GMetaExtendType (*getItemExtendType)(uint32_t flags, const GMetaItem * metaItem);
};

class GMetaMethodDataBase
{
public:
	void deleteSelf();

	size_t getParamCount() const;
	bool hasResult() const;
	GMetaType getParamType(size_t index) const;
	GMetaType getResultType() const;
	
	GMetaExtendType getResultExtendType(uint32_t flags) const {
		return this->virtualFunctions->getResultExtendType(flags);
	}
	
	GMetaExtendType getParamExtendType(uint32_t flags, size_t index) const {
		return this->virtualFunctions->getParamExtendType(flags, index);
	}

	bool isVariadic() const;
	bool isExplicitThis() const;

	GVariant invoke(void * instance, GVariant const * const * params, size_t paramCount) const;
	GVariant invokeByData(void * instance, GVariantData const * const * params, size_t paramCount) const;

	bool checkParam(const GVariant & param, size_t paramIndex) const;

	bool isParamTransferOwnership(size_t paramIndex) const;
	bool isResultTransferOwnership() const;

	// must be defined in header to make template function overloading happy.
	GMetaExtendType getItemExtendType(uint32_t flags, const GMetaItem * metaItem) const {
		return this->virtualFunctions->getItemExtendType(flags, metaItem);
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
	static size_t virtualGetParamCount() {
		size_t paramCount = TraitsType::Arity;
		if(paramCount > 0 && PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result) {
			--paramCount;
		}
		if(paramCount > 0 && virtualIsVariadic()) {
			--paramCount;
		}
		return paramCount;
	}

	static bool virtualHasResult() {
		return ! IsSameType<typename TraitsType::ResultType, void>::Result;
	}

	template <unsigned int N>
	struct GetParamTypeSelector
	{
		template <typename TypeList>
		GMetaType operator()(const TypeList & /*typeList*/)
		{
			return createMetaType<typename TypeList_GetWithDefault<TypeList, N>::Result>();
		}
	};

	static GMetaType virtualGetParamType(size_t index) {
		meta_internal::adjustParamIndex(index, PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result);
		return GTypeSelector<CallbackT::TraitsType::Arity>::template select<GMetaType, GetParamTypeSelector>(
			index,
			typename CallbackT::TraitsType::ArgTypeList()
		);
	}

	static GMetaType virtualGetResultType() {
		return createMetaType<typename CallbackT::TraitsType::ResultType>();
	}

	static GMetaExtendType virtualGetResultExtendType(uint32_t flags) {
		return createMetaExtendType<typename CallbackT::TraitsType::ResultType>(flags);
	}
	
	template <typename TypeList>
	struct GetParamExtendTypeSelectorParam
	{
		uint32_t flags;
	};
	
	template <unsigned int N>
	struct GetParamExtendTypeSelector
	{
		template <typename TypeList>
		GMetaExtendType operator()(const GetParamExtendTypeSelectorParam<TypeList> & param)
		{
			return createMetaExtendType<typename TypeList_GetWithDefault<TypeList, N>::Result>(param.flags);
		}
	};

	static GMetaExtendType virtualGetParamExtendType(uint32_t flags, size_t index) {
		meta_internal::adjustParamIndex(index, PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result);
		return GTypeSelector<CallbackT::TraitsType::Arity>::template select<GMetaExtendType, GetParamExtendTypeSelector>(
			index,
			GetParamExtendTypeSelectorParam<typename CallbackT::TraitsType::ArgTypeList>{ flags }
		);
	}
	
	static bool virtualIsVariadic() {
		return IsVariadicFunction<TraitsType>::Result;
	}

	static bool virtualIsExplicitThis() {
		return PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result;
	}

	static GVariant virtualInvoke(const void * self, void * instance, GVariant const * const * params, size_t paramCount) {
		checkInvokingArity(paramCount, virtualGetParamCount(), virtualIsVariadic());

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

	static GVariant virtualInvokeByData(const void * self, void * instance, GVariantData const * const * params, size_t paramCount) {
		checkInvokingArity(paramCount, virtualGetParamCount(), virtualIsVariadic());

		static_cast<const GMetaMethodData *>(self)->callback.setObject(instance);
		return GMetaInvokeHelper<CallbackType,
			TraitsType,
			TraitsType::Arity,
			typename TraitsType::ResultType,
			Policy,
			IsVariadicFunction<TraitsType>::Result,
			PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result
		>::invokeByData(instance, static_cast<const GMetaMethodData *>(self)->callback, params, paramCount);
	}

	template <typename TypeList>
	struct CheckParamSelectorParam
	{
		const GVariant & param;
	};
	
	template <unsigned int N>
	struct CheckParamSelector
	{
		template <typename TypeList>
		bool operator()(const CheckParamSelectorParam<TypeList> & param)
		{
			return canFromVariant<typename TypeList_GetWithDefault<TypeList, N>::Result>(param.param);
		}
	};

	static bool virtualCheckParam(const GVariant & param, size_t paramIndex) {
		if(virtualIsVariadic() && paramIndex >= virtualGetParamCount()) {
			return true;
		}
		else {
			if(paramIndex >= virtualGetParamCount()) {
				return false;
			}
		}

		if(PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result) {
			++paramIndex;
		}

		return GTypeSelector<CallbackT::TraitsType::Arity>::template select<bool, CheckParamSelector>(
			paramIndex,
			CheckParamSelectorParam<typename CallbackT::TraitsType::ArgTypeList>{ param }
		);

	}

	static bool virtualIsParamTransferOwnership(size_t paramIndex) {
		if(PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result) {
			++paramIndex;
		}
		return policyHasIndexedRule<Policy, GMetaRuleTransferOwnership>(static_cast<int>(paramIndex));
	}

	static bool virtualIsResultTransferOwnership() {
		return policyHasIndexedRule<Policy, GMetaRuleTransferOwnership>(metaPolicyResultIndex);
	}

	static GMetaExtendType virtualGetItemExtendType(uint32_t flags, const GMetaItem * metaItem)
	{
		return createMetaExtendType<typename TraitsType::FunctionType>(flags, metaItem);
	}

public:
	GMetaMethodData(const CallbackType & cb, const Policy &) : callback(cb) {
		static GMetaMethodDataVirtual thisFunctions = {
			&virtualBaseMetaDeleter<GMetaMethodData>,
			&virtualGetParamCount,
			&virtualHasResult,
			&virtualGetParamType,
			&virtualGetResultType,
			&virtualGetResultExtendType,
			&virtualGetParamExtendType,
			&virtualIsVariadic,
			&virtualIsExplicitThis,
			&virtualInvoke,
			&virtualInvokeByData,
			&virtualCheckParam,
			&virtualIsParamTransferOwnership,
			&virtualIsResultTransferOwnership,
			&virtualGetItemExtendType
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
		return makeCallback(static_cast<OT *>(nullptr), func);
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


template <int arity, typename OT, typename ArgsListType>
struct GMetaConstructorInvoker
{
	template <typename... Parameters>
	void * operator()(Parameters && ... parameters)
	{
		return new OT(std::forward<Parameters>(parameters)...);
	}
};


} // namespace meta_internal


} // namespace cpgf


#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif
