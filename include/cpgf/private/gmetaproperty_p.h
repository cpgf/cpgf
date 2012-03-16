#ifndef __GMETAPROPERTY_P_H
#define __GMETAPROPERTY_P_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/gcallback.h"
#include "cpgf/gifelse.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
#endif

#ifdef G_COMPILER_CPPBUILDER
#pragma warn -8008 //Condition is always true
#pragma warn -8066 //Unreachable code
#endif


namespace cpgf {

namespace meta_internal {

template <typename Getter, typename Policy, typename Enabled = void>
class GMetaGetter
{
public:
	typedef int PropertyType;
	
	G_STATIC_CONSTANT(bool, HasGetter = false);
	G_STATIC_CONSTANT(bool, Readable = false);

public:
	GMetaGetter(const Getter & getter) {
		(void)getter;
	}

	GVariant get(void * instance) const {
		(void)instance;

		return GVariant();
	}

	void * getPropertyAddress(void * instance) const {
		(void)instance;

		return NULL;
	}
};


template <typename Getter, typename Policy>
class GMetaGetter <Getter, Policy, typename GEnableIfResult<
	GAndResult<
		GNotResult<IsFunction<Getter> >,
		GNotResult<MemberDataTrait<Getter> >,
		GNotResult<IsFundamental<Getter> >
	>
	>::Result
>
{
	GASSERT_STATIC(IsPointer<Getter>::Result);

public:
	G_STATIC_CONSTANT(bool, HasGetter = true);
	G_STATIC_CONSTANT(bool, Readable = (PolicyNotHasRule<Policy, GMetaRuleForbidRead>::Result));

public:
	typedef typename RemovePointer<Getter>::Result PropertyType;

public:
	GMetaGetter(const Getter & getter) : getter(getter) {
	}

	GMetaGetter(const GMetaGetter & other) : getter(other.getter) {
	}

	GMetaGetter & operator = (const GMetaGetter & other) {
		if(this != &other) {
			this->getter = other.getter;
		}
	}

	GVariant get(void * instance) const {
		return this->doGet<void>(instance);
	}
	
	void * getPropertyAddress(void * instance) const {
		(void)instance;

		return this->getter;
	}

private:	
	template <typename T>
	GVariant doGet(typename GEnableIf<Readable, T>::Result * instance) const {
		(void)instance;

		GVarTypeData data = GVarTypeData();
		deduceVariantType<PropertyType>(data, true);
		return GVariant(data, *(this->getter));
	}
	
	template <typename T>
	GVariant doGet(typename GDisableIf<Readable, T>::Result * instance) const {
		(void)instance;

		meta_internal::handleForbidAccessError(true);
		
		return GVariant();
	}
	

private:
	Getter getter;
};

template <typename Getter, typename Policy>
class GMetaGetter <Getter, Policy, typename GEnableIfResult<
	GAndResult<
		GNotResult<IsFunction<Getter> >,
		MemberDataTrait<Getter>
	>
	>::Result
>
{
public:
	G_STATIC_CONSTANT(bool, HasGetter = true);
	G_STATIC_CONSTANT(bool, Readable = (PolicyNotHasRule<Policy, GMetaRuleForbidRead>::Result));

public:
	typedef typename MemberDataTrait<Getter>::FieldType PropertyType;

public:
	GMetaGetter(const Getter & getter) : getter(getter) {
	}

	GMetaGetter(const GMetaGetter & other) : getter(other.getter) {
	}

	GMetaGetter & operator = (const GMetaGetter & other) {
		if(this != &other) {
			this->getter = other.getter;
		}
	}

	GVariant get(void * instance) const {
		return this->doGet<void>(instance);
	}
	
	void * getPropertyAddress(void * instance) const {
		return &(static_cast<typename MemberDataTrait<Getter>::ObjectType *>(instance)->*(this->getter));
	}

private:	
	template <typename T>
	GVariant doGet(typename GEnableIf<Readable, T>::Result * instance) const {
		GVarTypeData data = GVarTypeData();
		deduceVariantType<PropertyType>(data, true);
		return GVariant(data, static_cast<typename MemberDataTrait<Getter>::ObjectType *>(instance)->*(this->getter));
	}

	template <typename T>
	GVariant doGet(typename GDisableIf<Readable, T>::Result * instance) const {
		(void)instance;

		meta_internal::handleForbidAccessError(true);
		
		return GVariant();
	}
	
private:
	Getter getter;
};

template <typename Getter, typename Policy>
class GMetaGetter <Getter, Policy, typename GEnableIfResult<IsFunction<Getter> >::Result>
{
public:
	G_STATIC_CONSTANT(bool, HasGetter = true);
	G_STATIC_CONSTANT(bool, Readable = (PolicyNotHasRule<Policy, GMetaRuleForbidRead>::Result));
	G_STATIC_CONSTANT(bool, ExplicitThis = (PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result || PolicyHasRule<Policy, GMetaRuleGetterExplicitThis>::Result));
	G_STATIC_CONSTANT(bool, NotExplicitThis = !ExplicitThis);

public:
	typedef typename GFunctionTraits<Getter>::ResultType PropertyType;

public:
	GMetaGetter(const Getter & getter) : callback(makeCallback(getter)) {
	}

	GMetaGetter(const GMetaGetter & other) : callback(other.callback) {
	}

	GMetaGetter & operator = (const GMetaGetter & other) {
		if(this != &other) {
			this->callback = other.callback;
		}
	}

	GVariant get(void * instance) const {
		return this->doGet<void>(instance);
	}
	
	void * getPropertyAddress(void * instance) const {
		(void)instance;

		return NULL;
	}

private:	
	template <typename T>
	GVariant doGet(typename GEnableIf<Readable && NotExplicitThis, T>::Result * instance) const {
		this->callback.setObject(instance);
		
		GVarTypeData data = GVarTypeData();
		deduceVariantType<PropertyType>(data, true);
		return GVariant(data, this->callback());
	}

	template <typename T>
	GVariant doGet(typename GEnableIf<Readable && ExplicitThis, T>::Result * instance) const {
		this->callback.setObject(instance);
		
		GVarTypeData data = GVarTypeData();
		deduceVariantType<PropertyType>(data, true);
		return GVariant(data, this->callback((typename GFunctionTraits<Getter>::ArgList::Arg0)(instance)));
	}

	template <typename T>
	GVariant doGet(typename GDisableIf<Readable, T>::Result * instance) const {
		(void)instance;

		meta_internal::handleForbidAccessError(true);
		
		return GVariant();
	}
	
private:
	typename FunctionCallbackType<Getter>::Result callback;
};


template <typename Setter, typename Policy, typename Enabled = void>
class GMetaSetter
{
public:
	typedef int PropertyType;

	G_STATIC_CONSTANT(bool, HasSetter = false);
	G_STATIC_CONSTANT(bool, Writable = false);

public:
	GMetaSetter(const Setter & setter) {
		(void)setter;
	}

	void set(void * instance, const GVariant & value) const {
		(void)instance;
		(void)value;
	}

};


template <typename Setter, typename Policy>
class GMetaSetter <Setter, Policy, typename GEnableIfResult<
	GAndResult<
		GNotResult<IsFunction<Setter> >,
		GNotResult<MemberDataTrait<Setter> >,
		GNotResult<IsFundamental<Setter> >
	>
	>::Result
>
{
	GASSERT_STATIC(IsPointer<Setter>::Result);

public:
	G_STATIC_CONSTANT(bool, HasSetter = true);
	G_STATIC_CONSTANT(bool, Writable = (PolicyNotHasRule<Policy, GMetaRuleForbidWrite>::Result));

public:
	typedef typename RemovePointer<Setter>::Result PropertyType;

public:
	GMetaSetter(const Setter & setter) : setter(setter) {
	}

	GMetaSetter(const GMetaSetter & other) : setter(other.setter) {
	}

	GMetaSetter & operator = (const GMetaSetter & other) {
		if(this != &other) {
			this->setter = other.setter;
		}
	}

	void set(void * instance, const GVariant & value) const {
		this->doSet<void>(instance, value);
	}

private:	
	template <typename T>
	void doSet(typename GEnableIf<Writable, T>::Result * instance, const GVariant & value) const {
		(void)instance;

		*(this->setter) = fromVariant<PropertyType>(value);
	}

	template <typename T>
	void doSet(typename GDisableIf<Writable, T>::Result * instance, const GVariant & value) const {
		(void)instance;
		(void)value;

		meta_internal::handleForbidAccessError(false);
	}

private:
	Setter setter;
};

template <typename Setter, typename Policy>
class GMetaSetter <Setter, Policy, typename GEnableIfResult<
	GAndResult<
		GNotResult<IsFunction<Setter> >,
		MemberDataTrait<Setter>
	>
	>::Result
>
{
public:
	G_STATIC_CONSTANT(bool, HasSetter = true);
	G_STATIC_CONSTANT(bool, Writable = (PolicyNotHasRule<Policy, GMetaRuleForbidWrite>::Result));

public:
	typedef typename MemberDataTrait<Setter>::FieldType PropertyType;

public:
	GMetaSetter(const Setter & setter) : setter(setter) {
	}

	GMetaSetter(const GMetaSetter & other) : setter(other.setter) {
	}

	GMetaSetter & operator = (const GMetaSetter & other) {
		if(this != &other) {
			this->setter = other.setter;
		}
	}

	void set(void * instance, const GVariant & value) const {
		this->doSet<void>(instance, value);
	}

private:	
	template <typename T>
	void doSet(typename GEnableIf<Writable, T>::Result * instance, const GVariant & value) const {
		static_cast<typename MemberDataTrait<Setter>::ObjectType *>(instance)->*(this->setter) = fromVariant<PropertyType>(value);
	}

	template <typename T>
	void doSet(typename GDisableIf<Writable, T>::Result * instance, const GVariant & value) const {
		(void)instance;
		(void)value;

		meta_internal::handleForbidAccessError(false);
	}

private:
	Setter setter;
};

template <typename Setter, typename Policy>
class GMetaSetter <Setter, Policy, typename GEnableIfResult<IsFunction<Setter> >::Result>
{
public:
	G_STATIC_CONSTANT(bool, HasSetter = true);
	G_STATIC_CONSTANT(bool, Writable = (PolicyNotHasRule<Policy, GMetaRuleForbidWrite>::Result));
	G_STATIC_CONSTANT(bool, ExplicitThis = (PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result || PolicyHasRule<Policy, GMetaRuleSetterExplicitThis>::Result));
	G_STATIC_CONSTANT(bool, NotExplicitThis = !ExplicitThis);

public:
	typedef typename GFunctionTraits<Setter>::ArgList::Arg0 PropertyType;

public:
	GMetaSetter(const Setter & setter) : callback(makeCallback(setter)) {
	}

	GMetaSetter(const GMetaSetter & other) : callback(other.callback) {
	}

	GMetaSetter & operator = (const GMetaSetter & other) {
		if(this != &other) {
			this->callback = other.callback;
		}
	}

	void set(void * instance, const GVariant & value) const {
		this->doSet<void>(instance, value);
	}

private:	
	template <typename T>
	void doSet(typename GEnableIf<Writable && NotExplicitThis, T>::Result * instance, const GVariant & value) const {
		this->callback.setObject(instance);
		this->callback(fromVariant<PropertyType, typename GIfElseResult<PolicyHasRule<Policy, GMetaRuleCopyConstReference<0> >, VarantCastCopyConstRef, VarantCastKeepConstRef>::Result>(value));
	}

	template <typename T>
	void doSet(typename GEnableIf<Writable && ExplicitThis, T>::Result * instance, const GVariant & value) const {
		this->callback.setObject(instance);
		this->callback((typename GFunctionTraits<Setter>::ArgList::Arg0)(instance),
			fromVariant<typename GFunctionTraits<Setter>::ArgList::Arg1, typename GIfElseResult<PolicyHasRule<Policy, GMetaRuleCopyConstReference<0> >, VarantCastCopyConstRef, VarantCastKeepConstRef>::Result>(value));
	}

	template <typename T>
	void doSet(typename GDisableIf<Writable, T>::Result * instance, const GVariant & value) const {
		(void)instance;
		(void)value;

		meta_internal::handleForbidAccessError(false);
	}

private:
	typename FunctionCallbackType<Setter>::Result callback;
};


struct GMetaPropertyDataVirtual
{
	void (*deleteObject)(void * self);
	bool (*canGet)(const void * self);
	bool (*canSet)(const void * self);
	GVariant (*get)(const void * self, void * instance);
	void (*set)(const void * self, void * instance, const GVariant & v);
	size_t (*getPropertySize)(const void * self);
	void * (*getPropertyAddress)(const void * self, void * instance);
	GMetaConverter * (*createConverter)(const void * self);
	GMetaExtendType (*getItemExtendType)(const void * self, uint32_t flags);
};

class GMetaPropertyDataBase
{
public:
	void deleteObject();
	
	bool canGet() const;
	bool canSet() const;

	GVariant get(void * instance) const;
	void set(void * instance, const GVariant & v) const;

	size_t getPropertySize() const;
	void * getPropertyAddress(void * instance) const;

	GMetaConverter * createConverter() const;
	GMetaExtendType getItemExtendType(uint32_t flags) const;

protected:
	GMetaPropertyDataVirtual * virtualFunctions;
};

template <typename Getter, typename Setter, typename Policy>
class GMetaPropertyData : public GMetaPropertyDataBase
{
private:
	typedef GMetaGetter<Getter, Policy> GetterType;
	typedef GMetaSetter<Setter, Policy> SetterType;
	
private:
	static bool virtualCanGet(const void * self) {
		(void)self;

		return GetterType::Readable;
	}

	static bool virtualCanSet(const void * self) {
		(void)self;
		
		return SetterType::Writable;
	}

	static GVariant virtualGet(const void * self, void * instance) {
		return static_cast<const GMetaPropertyData *>(self)->metaGetter.get(instance);
	}

	static void virtualSet(const void * self, void * instance, const GVariant & value) {
		static_cast<const GMetaPropertyData *>(self)->metaSetter.set(instance, value);
	}

	static void * virtualGetPropertyAddress(const void * self, void * instance) {
		return static_cast<const GMetaPropertyData *>(self)->metaGetter.getPropertyAddress(instance);
	}

	static size_t virtualGetPropertySize(const void * self) {
		(void)self;

		if(GetterType::HasGetter) {
			return sizeof(typename GetterType::PropertyType);
		}

		if(SetterType::HasSetter) {
			return sizeof(typename SetterType::PropertyType);
		}

		meta_internal::handleForbidAccessError(true);
		
		return 0;
	}

	static GMetaConverter * virtualCreateConverter(const void * self) {
		(void)self;

		if(GetterType::HasGetter) {
			return GMetaConverterTraits<typename GetterType::PropertyType>::createConverter();
		}

		if(SetterType::HasSetter) {
			return GMetaConverterTraits<typename SetterType::PropertyType>::createConverter();
		}

		return NULL;
	}

	static GMetaExtendType virtualGetItemExtendType(const void * self, uint32_t flags)
	{
		(void)self;
		
		if(GetterType::HasGetter) {
			return createMetaExtendType<typename GetterType::PropertyType>(flags);
		}

		if(SetterType::HasSetter) {
			return createMetaExtendType<typename SetterType::PropertyType>(flags);
		}

		return GMetaExtendType();
	}

public:
	GMetaPropertyData(const Getter & getter, const Setter & setter) : metaGetter(getter), metaSetter(setter) {
		static GMetaPropertyDataVirtual thisFunctions = {
			&virtualBaseMetaDeleter<GMetaPropertyData>,
			&virtualCanGet,
			&virtualCanSet,
			&virtualGet,
			&virtualSet,
			&virtualGetPropertySize,
			&virtualGetPropertyAddress,
			&virtualCreateConverter,
			virtualGetItemExtendType
		};
		this->virtualFunctions = &thisFunctions;
	}

private:
	GMetaGetter<Getter, Policy> metaGetter;
	GMetaSetter<Setter, Policy> metaSetter;
};


template <typename Getter, typename Setter, typename Policy>
GMetaType createPropertyType() {
	if(GMetaGetter<Getter, Policy>::HasGetter) {
		return createMetaType<typename GMetaGetter<Getter, Policy>::PropertyType>();
	}

	if(GMetaSetter<Setter, Policy>::HasSetter) {
		return createMetaType<typename GMetaSetter<Setter, Policy>::PropertyType>();
	}

	return GMetaType();
}

} // namespace meta_internal




} // namespace cpgf


#ifdef G_COMPILER_CPPBUILDER
#pragma warn .8008 //Condition is always true
#pragma warn .8066 //Unreachable code
#endif


#if defined(_MSC_VER)
#pragma warning(pop)
#endif



#endif

