#ifndef __GMETAMETHOD_H
#define __GMETAMETHOD_H

#include "cpgf/private/gmetamethod_p.h"


#define GMETA_METHOD(method) \
	reflectMethod(cpgf::normalizeReflectName(GPP_STRINGIZE(method)).c_str(), &MetaCurrentClassType::method)

#define GMETA_QUALIFIED_METHOD(method) \
	using namespace cpgf; \
	reflectMethod(cpgf::normalizeReflectName(GPP_STRINGIZE(method)).c_str(), &method)


namespace cpgf {


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



#endif
