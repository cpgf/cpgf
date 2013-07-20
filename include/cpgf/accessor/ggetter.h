#ifndef __GGETTER_H
#define __GGETTER_H

#include "cpgf/gmetapolicy.h"
#include "cpgf/gcallback.h"
#include "cpgf/gifelse.h"
#include "cpgf/gtypetraits.h"
#include "cpgf/gassert.h"
#include "cpgf/gexception.h"
#include "cpgf/ggetobjectaddress.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
#endif

#ifdef G_COMPILER_CPPBUILDER
#pragma warn -8008 //Condition is always true
#pragma warn -8066 //Unreachable code
#endif


namespace cpgf {


extern int Error_Meta_ReadDenied;

template <typename RawGetter, typename Policy = GMetaPolicyDefault, typename Enabled = void>
class GInstanceGetter
{
public:
	typedef int ValueType;
	typedef ValueType PassType;
	
	G_STATIC_CONSTANT(bool, HasGetter = false);
	G_STATIC_CONSTANT(bool, Readable = false);

public:
	GInstanceGetter(const RawGetter & /*getter*/) {
	}

	PassType get(const void * /*instance*/) const {
		raiseCoreException(Error_Meta_ReadDenied);

		return ValueType();
	}

	void * getAddress(const void * /*instance*/) const {
		raiseCoreException(Error_Meta_ReadDenied);

		return NULL;
	}
};


template <typename RawGetter, typename Policy>
class GInstanceGetter <RawGetter, Policy, typename GEnableIfResult<
	GAndResult<
		GNotResult<IsFunction<RawGetter> >,
		GNotResult<MemberDataTrait<RawGetter> >,
		GNotResult<IsFundamental<RawGetter> >
	>
	>::Result
>
{
	GASSERT_STATIC(IsPointer<RawGetter>::Result);
	
protected:
	typedef const RawGetter & RawGetterPassType;	

public:
	G_STATIC_CONSTANT(bool, HasGetter = true);
	G_STATIC_CONSTANT(bool, Readable = (PolicyNotHasRule<Policy, GMetaRuleForbidRead>::Result));

public:
	typedef typename RemovePointer<RawGetter>::Result ValueType;
	typedef ValueType & PassType;

public:
	GInstanceGetter(RawGetterPassType getter) : getter(getter) {
	}

	GInstanceGetter(const GInstanceGetter & other) : getter(other.getter) {
	}

	GInstanceGetter & operator = (const GInstanceGetter & other) {
		if(this != &other) {
			this->getter = other.getter;
		}
		
		return *this;
	}

	PassType get(const void * instance) const {
		return this->doGet<void>(instance);
	}
	
	void * getAddress(const void * /*instance*/) const {
		return *(void **)(getObjectAddress(this->getter));
	}

private:	
	template <typename T>
	PassType doGet(typename GEnableIf<Readable, T>::Result const * /*instance*/) const {
		return *(this->getter);
	}
	
	template <typename T>
	PassType doGet(typename GDisableIf<Readable, T>::Result const * /*instance*/) const {
		raiseCoreException(Error_Meta_ReadDenied);
		
		return *(typename RemoveReference<ValueType>::Result *)(0);
	}
	

private:
	RawGetter getter;
};

template <typename RawGetter, typename Policy>
class GInstanceGetter <RawGetter, Policy, typename GEnableIfResult<
	GAndResult<
		GNotResult<IsFunction<RawGetter> >,
		MemberDataTrait<RawGetter>
	>
	>::Result
>
{
protected:
	typedef const RawGetter & RawGetterPassType;	

public:
	G_STATIC_CONSTANT(bool, HasGetter = true);
	G_STATIC_CONSTANT(bool, Readable = (PolicyNotHasRule<Policy, GMetaRuleForbidRead>::Result));

public:
	typedef typename MemberDataTrait<RawGetter>::FieldType ValueType;
	typedef ValueType PassType;

public:
	GInstanceGetter(RawGetterPassType getter) : getter(getter) {
	}

	GInstanceGetter(const GInstanceGetter & other) : getter(other.getter) {
	}

	GInstanceGetter & operator = (const GInstanceGetter & other) {
		if(this != &other) {
			this->getter = other.getter;
		}
		
		return *this;
	}

	PassType get(const void * instance) const {
		return this->doGet<void>(instance);
	}
	
	void * getAddress(const void * instance) const {
		return &(static_cast<typename MemberDataTrait<RawGetter>::ObjectType *>(const_cast<void *>(instance))->*(this->getter));
	}

private:	
	template <typename T>
	PassType doGet(typename GEnableIf<Readable, T>::Result const * instance) const {
		return static_cast<typename MemberDataTrait<RawGetter>::ObjectType const *>(instance)->*(this->getter);
	}

	template <typename T>
	PassType doGet(typename GDisableIf<Readable, T>::Result const * /*instance*/) const {
		raiseCoreException(Error_Meta_ReadDenied);
		
		return *(typename RemoveReference<ValueType>::Result *)(0);
	}
	
private:
	RawGetter getter;
};

template <typename RawGetter, typename Policy>
class GInstanceGetter <RawGetter, Policy, typename GEnableIfResult<IsFunction<RawGetter> >::Result>
{
protected:
	typedef RawGetter RawGetterPassType;	

public:
	G_STATIC_CONSTANT(bool, HasGetter = true);
	G_STATIC_CONSTANT(bool, Readable = (PolicyNotHasRule<Policy, GMetaRuleForbidRead>::Result));
	G_STATIC_CONSTANT(bool, ExplicitThis = (PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result || PolicyHasRule<Policy, GMetaRuleGetterExplicitThis>::Result));
	G_STATIC_CONSTANT(bool, NotExplicitThis = !ExplicitThis);

public:
	typedef typename GFunctionTraits<RawGetter>::ResultType ValueType;
	typedef ValueType PassType;

public:
	GInstanceGetter(RawGetterPassType getter) : callback(makeCallback(getter)) {
	}

	GInstanceGetter(const GInstanceGetter & other) : callback(other.callback) {
	}

	GInstanceGetter & operator = (const GInstanceGetter & other) {
		if(this != &other) {
			this->callback = other.callback;
		}
		
		return *this;
	}

	PassType get(const void * instance) const {
		return this->doGet<void>(instance);
	}
	
	void * getAddress(const void * /*instance*/) const {
		return NULL;
	}

private:	
	template <typename T>
	PassType doGet(typename GEnableIf<Readable && NotExplicitThis, T>::Result const * instance) const {
		this->callback.setObject(const_cast<void *>(instance));
		return this->callback();
	}

	template <typename T>
	PassType doGet(typename GEnableIf<Readable && ExplicitThis, T>::Result const * instance) const {
		this->callback.setObject(const_cast<void *>(instance));
		return this->callback((typename GFunctionTraits<RawGetter>::ArgList::Arg0)(instance));
	}

	template <typename T>
	PassType doGet(typename GDisableIf<Readable, T>::Result const * /*instance*/) const {
		raiseCoreException(Error_Meta_ReadDenied);
		
		return *(typename RemoveReference<ValueType>::Result *)(0);
	}
	
private:
	typename FunctionCallbackType<RawGetter>::Result callback;
};


template <typename RawGetter, typename Policy = GMetaPolicyDefault>
class GGetter : public GInstanceGetter<RawGetter, Policy>
{
private:
	typedef GInstanceGetter<RawGetter, Policy> super;

public:
	GGetter(const void * instance, typename super::RawGetterPassType getter) : super(getter), instance(instance) {
	}
	
	GGetter(const GGetter & other) : super(other), instance(other.instance) {
	}
	
	GGetter & operator = (const GGetter & other) {
		super::operator = (other);
		this->instance = other.instance;
		
		return *this;
	}

	typename super::PassType get() const {
		return super::get(this->instance);
	}
	
	typename super::PassType operator() () const {
		return this->get();
	}
	
	operator typename super::PassType () const {
		return this->get();
	}
	
	void * getAddress() const {
		return super::getAddress(this->instance);
	}
	
	const void * getInstance() const {
		return this->instance;
	}

	void setInstance(const void * newInstance) {
		this->instance = newInstance;
	}
	
private:
	const void * instance;
};


template <typename ValueType>
class GConstantGetter
{
private:
	typedef const ValueType & PassType;

public:
	GConstantGetter(ValueType value) : value(value) {
	}
	
	GConstantGetter(const GConstantGetter & other) : value(other.value) {
	}
	
	GConstantGetter & operator = (const GConstantGetter & other) {
		this->value = other.value;
		
		return *this;
	}

	PassType get() const {
		return this->value;
	}
	
	PassType operator() () const {
		return this->get();
	}
	
	operator PassType () const {
		return this->get();
	}
	
	void * getAddress() const {
		return NULL;
	}
	
	const void * getInstance() const {
		return NULL;
	}

	void setInstance(const void * /*newInstance*/) {
	}
	
private:
	ValueType value;
};


template <typename RawGetter, typename Policy>
GInstanceGetter<RawGetter, Policy> createInstanceGetter(const RawGetter & getter, Policy /*policy*/)
{
	return GInstanceGetter<RawGetter, Policy>(getter);
}

template <typename RawGetter>
GInstanceGetter<RawGetter, GMetaPolicyDefault> createInstanceGetter(const RawGetter & getter)
{
	return GInstanceGetter<RawGetter, GMetaPolicyDefault>(getter);
}

template <typename RawGetter, typename Policy>
GGetter<RawGetter, Policy> createGetter(const void * instance, const RawGetter & getter, Policy /*policy*/)
{
	return GGetter<RawGetter, Policy>(instance, getter);
}

template <typename RawGetter>
GGetter<RawGetter, GMetaPolicyDefault> createGetter(const void * instance, const RawGetter & getter)
{
	return GGetter<RawGetter, GMetaPolicyDefault>(instance, getter);
}

template <typename ValueType>
GConstantGetter<ValueType> createConstantGetter(const ValueType & value)
{
	return GConstantGetter<ValueType>(value);
}


} // namespace cpgf


#ifdef G_COMPILER_CPPBUILDER
#pragma warn .8008 //Condition is always true
#pragma warn .8066 //Unreachable code
#endif


#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif
