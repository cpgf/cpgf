#ifndef CPGF_GMETAMETHOD_H
#define CPGF_GMETAMETHOD_H

#include "cpgf/private/gmetamethod_p.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif


namespace cpgf {


GMAKE_FINAL(GMetaMethod)

class GMetaMethod : public GMetaCallable, GFINAL_BASE(GMetaMethod)
{
private:
	typedef GMetaCallable super;

public:
	template <typename OT, typename FT, typename Policy>
	static GMetaMethod * newMethod(const char * name, const FT & func, const Policy & policy) {
		GMetaMethod * method = new GMetaMethod(name, createMetaType<FT>(), meta_internal::GMetaMethodCallbackMaker<OT, FT>::make(func), policy);

		if(! PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result) {
			method->addModifier(meta_internal::GMetaMethodCallbackMaker<OT, FT>::modifiers);
		}

		return method;
	}

public:
	template <typename CT, typename Policy>
	GMetaMethod(const char * name, const GMetaType & itemType, const CT & callback, const Policy & policy)
		: super(name, itemType, mcatMethod), baseData(new meta_internal::GMetaMethodData<CT, Policy>(callback, policy)) {
	}

	virtual GMetaType getParamType(size_t index) const;
	virtual size_t getParamCount() const;
	virtual size_t getDefaultParamCount() const;
	virtual bool hasResult() const;
	virtual GMetaType getResultType() const;
	virtual GMetaExtendType getResultExtendType(uint32_t flags) const;
	virtual GMetaExtendType getParamExtendType(uint32_t flags, size_t index) const;
	virtual bool isVariadic() const;
	virtual bool isExplicitThis() const;

	virtual GVariant execute(void * instance, const GVariant * params, size_t paramCount) const;
	virtual GVariant executeByData(void * instance, const GVariantData * * params, size_t paramCount) const;

	virtual bool checkParam(const GVariant & param, size_t paramIndex) const;

	virtual bool isParamTransferOwnership(size_t paramIndex) const;
	virtual bool isResultTransferOwnership() const;
	
	virtual GMetaExtendType getItemExtendType(uint32_t flags) const;

	void addDefaultParam(const GVariant & v);

#define REF_INVOKE(N, unused) GVariant invoke(void * instance GPP_COMMA_IF(N) GPP_REPEAT(N, GPP_COMMA_PARAM, const GVariant & p)) const;

	GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE

protected:
	GScopedPointer<meta_internal::GMetaMethodDataBase,
		meta_internal::GScopedPointerDeleter_BaseMeta<meta_internal::GMetaMethodDataBase> >
		baseData;
};


GMAKE_FINAL(GMetaConstructor)

class GMetaConstructor : public GMetaCallable, GFINAL_BASE(GMetaConstructor)
{
private:
	typedef GMetaCallable super;

public:
	template <typename OT, typename Signature, typename Policy>
	static GMetaConstructor * newConstructor(const Policy & policy) {
		GMetaConstructor * metaConstructor = new GMetaConstructor(
				GCallback<Signature>(
					meta_internal::GMetaConstructorInvoker<GFunctionTraits<Signature>::Arity, OT, typename GFunctionTraits<Signature>::ArgList>()
				),
				policy
			);

		return metaConstructor;
	}

	template <typename OT, typename FT, typename Policy>
	static GMetaConstructor * newConstructor(const FT & func, const Policy & policy) {
		GMetaConstructor * metaConstructor = new GMetaConstructor(
				meta_internal::GMetaMethodCallbackMaker<OT, FT>::make(func),
				policy
			);

		return metaConstructor;
	}

public:
	template <typename CT, typename Policy>
	GMetaConstructor(const CT & callback, const Policy & policy)
		: super(meta_internal::arityToName(CT::TraitsType::Arity).c_str(), createMetaType<typename CT::TraitsType::FullType>(), mcatConstructor),
			baseData(new meta_internal::GMetaMethodData<CT, Policy>(callback, policy)) {
	}

	virtual GMetaType getParamType(size_t index) const;
	virtual size_t getParamCount() const;
	virtual size_t getDefaultParamCount() const;
	virtual bool hasResult() const;
	virtual GMetaType getResultType() const;
	virtual GMetaExtendType getResultExtendType(uint32_t flags) const;
	virtual GMetaExtendType getParamExtendType(uint32_t flags, size_t index) const;
	virtual bool isVariadic() const;
	virtual bool isExplicitThis() const;

	virtual GVariant execute(void * instance, const GVariant * params, size_t paramCount) const;
	virtual GVariant executeByData(void * instance, const GVariantData * * params, size_t paramCount) const;

	virtual bool checkParam(const GVariant & param, size_t paramIndex) const;

	virtual bool isParamTransferOwnership(size_t paramIndex) const;
	virtual bool isResultTransferOwnership() const;

	virtual GMetaExtendType getItemExtendType(uint32_t flags) const;

	void addDefaultParam(const GVariant & v);

#define REF_NEW_INSTANCE(N, unused) \
	void * invoke(GPP_REPEAT(N, GPP_COMMA_PARAM, const GVariant & p)) const;

	GPP_REPEAT_2(REF_MAX_ARITY, REF_NEW_INSTANCE, GPP_EMPTY)

#undef REF_NEW_INSTANCE

protected:
	GScopedPointer<meta_internal::GMetaMethodDataBase,
		meta_internal::GScopedPointerDeleter_BaseMeta<meta_internal::GMetaMethodDataBase> >
		baseData;
};


} // namespace cpgf



#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif
