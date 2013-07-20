#ifndef __GSETTER_H
#define __GSETTER_H

#include "cpgf/gmetapolicy.h"
#include "cpgf/gcallback.h"
#include "cpgf/gifelse.h"
#include "cpgf/gtypetraits.h"
#include "cpgf/gassert.h"
#include "cpgf/gexception.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
#endif

#ifdef G_COMPILER_CPPBUILDER
#pragma warn -8008 //Condition is always true
#pragma warn -8066 //Unreachable code
#endif


namespace cpgf {


extern int Error_Meta_WriteDenied;

template <typename RawSetter, typename Policy = GMetaPolicyDefault, typename Enabled = void>
class GInstanceSetter
{
public:
	typedef int ValueType;
	typedef const ValueType & PassType;

	G_STATIC_CONSTANT(bool, HasSetter = false);
	G_STATIC_CONSTANT(bool, Writable = false);

public:
	GInstanceSetter(const RawSetter & /*setter*/) {
	}

	void set(void * /*instance*/, PassType /*value*/) const {
		raiseCoreException(Error_Meta_WriteDenied);
	}

	void * getAddress(void * /*instance*/) const {
		raiseCoreException(Error_Meta_WriteDenied);
		return NULL;
	}
};


template <typename RawSetter, typename Policy>
class GInstanceSetter <RawSetter, Policy, typename GEnableIfResult<
	GAndResult<
		GNotResult<IsFunction<RawSetter> >,
		GNotResult<MemberDataTrait<RawSetter> >,
		GNotResult<IsFundamental<RawSetter> >
	>
	>::Result
>
{
	GASSERT_STATIC(IsPointer<RawSetter>::Result);

protected:
	typedef const RawSetter & RawSetterPassType;	

public:
	G_STATIC_CONSTANT(bool, HasSetter = true);
	G_STATIC_CONSTANT(bool, Writable = (PolicyNotHasRule<Policy, GMetaRuleForbidWrite>::Result));

public:
	typedef typename RemovePointer<RawSetter>::Result ValueType;
	typedef const ValueType & PassType;

public:
	GInstanceSetter(RawSetterPassType setter) : setter(setter) {
	}

	GInstanceSetter(const GInstanceSetter & other) : setter(other.setter) {
	}

	GInstanceSetter & operator = (const GInstanceSetter & other) {
		if(this != &other) {
			this->setter = other.setter;
		}
	}

	void set(void * instance, PassType value) const {
		this->doSet<void>(instance, value);
	}

	void * getAddress(const void * /*instance*/) const {
		return this->setter;
	}

private:	
	template <typename T>
	void doSet(typename GEnableIf<Writable, T>::Result * /*instance*/, PassType value) const {
		*(this->setter) = value;
	}

	template <typename T>
	void doSet(typename GDisableIf<Writable, T>::Result * /*instance*/, PassType /*value*/) const {
		raiseCoreException(Error_Meta_WriteDenied);
	}

private:
	RawSetter setter;
};

template <typename RawSetter, typename Policy>
class GInstanceSetter <RawSetter, Policy, typename GEnableIfResult<
	GAndResult<
		GNotResult<IsFunction<RawSetter> >,
		MemberDataTrait<RawSetter>
	>
	>::Result
>
{
protected:
	typedef const RawSetter & RawSetterPassType;	

public:
	G_STATIC_CONSTANT(bool, HasSetter = true);
	G_STATIC_CONSTANT(bool, Writable = (PolicyNotHasRule<Policy, GMetaRuleForbidWrite>::Result));

public:
	typedef typename MemberDataTrait<RawSetter>::FieldType ValueType;
	typedef const ValueType & PassType;

public:
	GInstanceSetter(RawSetterPassType setter) : setter(setter) {
	}

	GInstanceSetter(const GInstanceSetter & other) : setter(other.setter) {
	}

	GInstanceSetter & operator = (const GInstanceSetter & other) {
		if(this != &other) {
			this->setter = other.setter;
		}
	}

	void set(void * instance, PassType value) const {
		this->doSet<void>(instance, value);
	}

	void * getAddress(const void * instance) const {
		return &(static_cast<typename MemberDataTrait<RawSetter>::ObjectType *>(const_cast<void *>(instance))->*(this->setter));
	}

private:	
	template <typename T>
	void doSet(typename GEnableIf<Writable, T>::Result * instance, PassType value) const {
		static_cast<typename MemberDataTrait<RawSetter>::ObjectType *>(instance)->*(this->setter) = value;
	}

	template <typename T>
	void doSet(typename GDisableIf<Writable, T>::Result * /*instance*/, PassType /*value*/) const {
		raiseCoreException(Error_Meta_WriteDenied);
	}

private:
	RawSetter setter;
};

template <typename RawSetter, typename Policy>
class GInstanceSetter <RawSetter, Policy, typename GEnableIfResult<IsFunction<RawSetter> >::Result>
{
protected:
	typedef RawSetter RawSetterPassType;	

public:
	G_STATIC_CONSTANT(bool, HasSetter = true);
	G_STATIC_CONSTANT(bool, Writable = (PolicyNotHasRule<Policy, GMetaRuleForbidWrite>::Result));
	G_STATIC_CONSTANT(bool, ExplicitThis = (PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result || PolicyHasRule<Policy, GMetaRuleSetterExplicitThis>::Result));
	G_STATIC_CONSTANT(bool, NotExplicitThis = !ExplicitThis);

private:
	template <typename F, bool>
	struct SelectValueType { typedef typename GFunctionTraits<F>::ArgList::Arg1 Result; };
	template <typename F>
	struct SelectValueType <F, false> { typedef typename GFunctionTraits<F>::ArgList::Arg0 Result; };

public:
	typedef typename SelectValueType<RawSetter, ExplicitThis>::Result ValueType;
	typedef ValueType PassType;

public:
	GInstanceSetter(RawSetterPassType setter) : callback(makeCallback(setter)) {
	}

	GInstanceSetter(const GInstanceSetter & other) : callback(other.callback) {
	}

	GInstanceSetter & operator = (const GInstanceSetter & other) {
		if(this != &other) {
			this->callback = other.callback;
		}
	}

	void set(void * instance, PassType value) const {
		this->doSet<void>(instance, value);
	}

	void * getAddress(const void * /*instance*/) const {
		return NULL;
	}

private:	
	template <typename T>
	void doSet(typename GEnableIf<Writable && NotExplicitThis, T>::Result * instance, PassType value) const {
		this->callback.setObject(instance);
		this->callback(value);
	}

	template <typename T>
	void doSet(typename GEnableIf<Writable && ExplicitThis, T>::Result * instance, PassType value) const {
		this->callback.setObject(instance);
		this->callback((typename GFunctionTraits<RawSetter>::ArgList::Arg0)(instance), value);
	}

	template <typename T>
	void doSet(typename GDisableIf<Writable, T>::Result * /*instance*/, PassType /*value*/) const {
		raiseCoreException(Error_Meta_WriteDenied);
	}

private:
	typename FunctionCallbackType<RawSetter>::Result callback;
};


template <typename RawSetter, typename Policy = GMetaPolicyDefault>
class GSetter : public GInstanceSetter<RawSetter, Policy>
{
private:
	typedef GInstanceSetter<RawSetter, Policy> super;

public:
	GSetter(void * instance, typename super::RawSetterPassType setter) : super(setter), instance(instance) {
	}
	
	GSetter(const GSetter & other) : super(other), instance(other.instance) {
	}
	
	GSetter & operator = (const GSetter & other) {
		super::operator = (other);
		this->instance = other.instance;
		
		return *this;
	}

	void set(typename super::PassType value) const {
		super::set(this->instance, value);
	}
	
	void operator() (typename super::PassType value) const {
		this->set(value);
	}
	
	GSetter & operator = (typename super::PassType value) {
		this->set(value);
		
		return *this;
	}

	void * getAddress() const {
		return super::getAddress(this->instance);
	}
	
	void * getInstance() const {
		return this->instance;
	}

	void setInstance(void * newInstance) {
		this->instance = newInstance;
	}
	
private:
	void * instance;
};


template <typename RawSetter, typename Policy>
GInstanceSetter<RawSetter, Policy> createInstanceSetter(const RawSetter & setter, Policy /*policy*/)
{
	return GInstanceSetter<RawSetter, Policy>(setter);
}

template <typename RawSetter>
GInstanceSetter<RawSetter, GMetaPolicyDefault> createInstanceSetter(const RawSetter & setter)
{
	return GInstanceSetter<RawSetter, GMetaPolicyDefault>(setter);
}

template <typename RawSetter, typename Policy>
GSetter<RawSetter, Policy> createSetter(void * instance, const RawSetter & setter, Policy /*policy*/)
{
	return GSetter<RawSetter, Policy>(instance, setter);
}

template <typename RawSetter>
GSetter<RawSetter, GMetaPolicyDefault> createSetter(void * instance, const RawSetter & setter)
{
	return GSetter<RawSetter, GMetaPolicyDefault>(instance, setter);
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
