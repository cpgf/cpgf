#ifndef CPGF_GMETAMETHOD_H
#define CPGF_GMETAMETHOD_H

#include "cpgf/private/gmetamethod_p.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif


namespace cpgf {

class GMetaMethod final : public GMetaCallable
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

	virtual GMetaType getParamType(size_t index) const override;
	virtual size_t getParamCount() const override;
	virtual size_t getDefaultParamCount() const override;
	virtual bool hasResult() const override;
	virtual GMetaType getResultType() const override;
	virtual GMetaExtendType getResultExtendType(uint32_t flags) const override;
	virtual GMetaExtendType getParamExtendType(uint32_t flags, size_t index) const override;
	virtual bool isVariadic() const override;
	virtual bool isExplicitThis() const override;

	virtual GVariant execute(void * instance, const GVariant * params, size_t paramCount) const override;
	virtual GVariant executeByData(void * instance, const GVariantData * * params, size_t paramCount) const override;

	virtual bool checkParam(const GVariant & param, size_t paramIndex) const override;

	virtual bool isParamTransferOwnership(size_t paramIndex) const override;
	virtual bool isResultTransferOwnership() const override;
	
	virtual GMetaExtendType getItemExtendType(uint32_t flags) const override;

	void addDefaultParam(const GVariant & v);

#define REF_INVOKE(N, unused) GVariant invoke(void * instance GPP_COMMA_IF(N) GPP_REPEAT(N, GPP_COMMA_PARAM, const GVariant & p)) const;

	GPP_REPEAT_2(REF_MAX_ARITY, REF_INVOKE, GPP_EMPTY)

#undef REF_INVOKE

protected:
	std::unique_ptr<meta_internal::GMetaMethodDataBase,
		meta_internal::GScopedPointerDeleter_BaseMeta<meta_internal::GMetaMethodDataBase> >
		baseData;
};


class GMetaConstructor final : public GMetaCallable
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

	virtual GMetaType getParamType(size_t index) const override;
	virtual size_t getParamCount() const override;
	virtual size_t getDefaultParamCount() const override;
	virtual bool hasResult() const override;
	virtual GMetaType getResultType() const override;
	virtual GMetaExtendType getResultExtendType(uint32_t flags) const override;
	virtual GMetaExtendType getParamExtendType(uint32_t flags, size_t index) const override;
	virtual bool isVariadic() const override;
	virtual bool isExplicitThis() const override;

	virtual GVariant execute(void * instance, const GVariant * params, size_t paramCount) const override;
	virtual GVariant executeByData(void * instance, const GVariantData * * params, size_t paramCount) const override;

	virtual bool checkParam(const GVariant & param, size_t paramIndex) const override;

	virtual bool isParamTransferOwnership(size_t paramIndex) const override;
	virtual bool isResultTransferOwnership() const override;

	virtual GMetaExtendType getItemExtendType(uint32_t flags) const override;

	void addDefaultParam(const GVariant & v);

#define REF_NEW_INSTANCE(N, unused) \
	void * invoke(GPP_REPEAT(N, GPP_COMMA_PARAM, const GVariant & p)) const;

	GPP_REPEAT_2(REF_MAX_ARITY, REF_NEW_INSTANCE, GPP_EMPTY)

#undef REF_NEW_INSTANCE

protected:
	std::unique_ptr<meta_internal::GMetaMethodDataBase,
		meta_internal::GScopedPointerDeleter_BaseMeta<meta_internal::GMetaMethodDataBase> >
		baseData;
};


} // namespace cpgf



#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif
