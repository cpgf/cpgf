#ifndef __GMETAMETHOD_H
#define __GMETAMETHOD_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/gmetaconverter.h"
#include "cpgf/gpp.h"
#include "cpgf/gcallback.h"

#define GMETA_METHOD(method) \
	reflectMethod(cpgf::normalizeReflectName(GPP_STRINGIZE(method)).c_str(), &MetaCurrentClassType::method)

#define GMETA_QUALIFIED_METHOD(method) \
	using namespace cpgf; \
	reflectMethod(cpgf::normalizeReflectName(GPP_STRINGIZE(method)).c_str(), &method)


namespace cpgf {

template <typename FT>
void reflectMethod(const char * name, FT func);


namespace meta_internal {


#define REF_GETPARAM_HELPER(N, unused) \
	case N: return createMetaType<typename TypeList_GetWithDefault<typename CallbackT::TraitsType::ArgTypeList, N>::Result>();

#define REF_CALL_HELPER_CAST(N, unused) \
	GPP_COMMA_IF(N) fromVariant<typename FT::ArgList::Arg ## N, HasMetaPolicyItem<Policy, GMetaPolicyItemKeepConstReference<N> >::Result>(*params[N])

#define REF_CALL_HELPER(N, unused) \
	template <typename CT, typename FT, typename RT, typename Policy> \
	struct GMetaMethodCallHelper<CT, FT, N, RT, Policy, false> { \
		static GVariant invoke(const CT & callback, GVariant const * const * params, size_t paramCount) { \
			(void)params; \
			(void)paramCount; \
			GVarTypeData typeData; \
			deduceVariantType<RT>(typeData, true); \
			GVariant v; \
			variant_internal::InitVariant<!HasMetaPolicyItem<Policy, GMetaPolicyItemKeepConstReference<N> >::Result>(v, typeData, static_cast<typename variant_internal::DeducePassType<RT>::PassType>(callback(GPP_REPEAT(N, REF_CALL_HELPER_CAST, GPP_EMPTY)))); \
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
				handleError(metaError_ParamOutOfIndex, "Parameter out of index");
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
			meta_internal::handleError(metaError_WrongArity, meta_internal::formatString("Wrong argument count. Expect: %d, but get: %d.", this->getParamCount(), paramCount));
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
				handleError(metaError_ParamOutOfIndex, "Parameter out of index");
				return false;
		}

#undef REF_CHECKPARAM_HELPER

	}

	virtual bool isParamTransferOwnership(size_t paramIndex) const {
		return hasIndexedPolicy<Policy, GMetaPolicyItemTransferOwnership>(static_cast<int>(paramIndex));
	}

	virtual bool isResultTransferOwnership() const {
		return hasIndexedPolicy<Policy, GMetaPolicyItemTransferOwnership>(-1);
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


class GMetaMethod : public GMetaCallable
{
private:
	typedef GMetaCallable super;

public:
	template <typename OT, typename FT, typename Policy>
	static GMetaMethod * newMethod(const char * name, const FT & func, const Policy & policy) {
		GASSERT_STATIC(IsFunction<FT>::Result);

		GMetaMethod * method = new GMetaMethod(name, createMetaType<FT>(), meta_internal::GMetaMethodCallbackMaker<OT, typename GFunctionTraits<FT>::ObjectType>::make(func), policy);

		method->addModifier(meta_internal::GMetaMethodCallbackMaker<OT, typename GFunctionTraits<FT>::ObjectType>::modifiers);

		return method;
	}

public:
	template <typename CT, typename Policy>
	GMetaMethod(const char * name, const GMetaType & itemType, const CT & callback, const Policy & policy)
		: super(name, itemType, mcatMethod), baseData(new meta_internal::GMetaMethodData<CT, Policy>(callback, policy)) {
	}

	virtual GMetaType getParamType(size_t index) const;
	virtual size_t getParamCount() const;
	virtual bool hasResult() const;
	virtual GMetaType getResultType() const;
	virtual bool isVariadic() const;


	virtual GVariant execute(void * instance, const GVariant * params, size_t paramCount) const;

	virtual bool checkParam(const GVariant & param, size_t paramIndex) const;

	virtual bool isParamTransferOwnership(size_t paramIndex) const;
	virtual bool isResultTransferOwnership() const;
	
	virtual GMetaConverter * createResultConverter() const;

#define REF_INVOKE(N, unused) GVariant invoke(void * instance GPP_COMMA_IF(N) GPP_REPEAT(N, GPP_COMMA_PARAM, const GVariant & p)) const;

	GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE

protected:
	GScopedPointer<meta_internal::GMetaMethodDataBase> baseData;
};

class GMetaConstructor : public GMetaCallable
{
private:
	typedef GMetaCallable super;

public:
	template <typename CT, typename Policy>
	GMetaConstructor(const CT & callback, const Policy & policy)
		: super(meta_internal::arityToName(CT::TraitsType::Arity).c_str(), createMetaType<typename CT::TraitsType::FullType>(), mcatConstructor),
			baseData(new meta_internal::GMetaMethodData<CT, Policy>(callback, policy)) {
	}

	virtual GMetaType getParamType(size_t index) const;
	virtual size_t getParamCount() const;
	virtual bool hasResult() const;
	virtual GMetaType getResultType() const;
	virtual bool isVariadic() const;

	virtual GVariant execute(void * instance, const GVariant * params, size_t paramCount) const;

	virtual bool checkParam(const GVariant & param, size_t paramIndex) const;

	virtual bool isParamTransferOwnership(size_t paramIndex) const;
	virtual bool isResultTransferOwnership() const;

	virtual GMetaConverter * createResultConverter() const;

#define REF_NEW_INSTANCE(N, unused) \
	void * invoke(GPP_REPEAT(N, GPP_COMMA_PARAM, const GVariant & p)) const;

	GPP_REPEAT_2(REF_MAX_ARITY, REF_NEW_INSTANCE, GPP_EMPTY)

#undef REF_NEW_INSTANCE

protected:
	GScopedPointer<meta_internal::GMetaMethodDataBase> baseData;
};


void globalAddMethod(GMetaMethod * method);

template <typename FT>
void reflectMethod(const char * name, FT func)
{
	GASSERT_STATIC(GFunctionTraits<FT>::IsFunction && !GFunctionTraits<FT>::IsMember);

	globalAddMethod(cpgf::GMetaMethod::newMethod<void>(name, func, GMetaPolicyDefault()));
}

template <typename FT, typename Policy>
void reflectMethod(const char * name, FT func, const Policy & policy)
{
	GASSERT_STATIC(GFunctionTraits<FT>::IsFunction && !GFunctionTraits<FT>::IsMember);

	globalAddMethod(cpgf::GMetaMethod::newMethod<void>(name, func, policy));
}


} // namespace cpgf


#undef REF_CALL_HELPER
#undef REF_CALL_HELPER_CAST
#undef REF_CALL_LOAD_PARAM
#undef REF_CONSTRUCTOR_INVOKE
#undef REF_GETPARAM_HELPER
#undef REF_PARAM_TYPEVALUE


#endif
