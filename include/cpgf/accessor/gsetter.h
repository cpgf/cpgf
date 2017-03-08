#ifndef CPGF_GSETTER_H
#define CPGF_GSETTER_H

#include "cpgf/gfunctiontraits.h"
#include "cpgf/gtypetraits.h"
#include "cpgf/gcallback.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/gexception.h"
#include "cpgf/gerrorcode.h"

#include <type_traits>

namespace cpgf {

namespace setter_internal {

template <typename T, typename Policy, typename EnableIf = void>
class GInstanceSetterImplement
{
public:
	typedef int DataType;
	typedef int ValueType;
	typedef ValueType PassType;
	
	static constexpr bool HasSetter = false;

public:
	static PassType set(DataType & /*data*/, const void * /*instance*/) {
		raiseCoreException(Error_Meta_WriteDenied);

		return ValueType();
	}

	static void * getAddress(DataType & /*data*/, const void * /*instance*/) {
		raiseCoreException(Error_Meta_WriteDenied);

		return nullptr;
	}
};

// setter via data address
template <typename T, typename Policy>
class GInstanceSetterImplement <T, Policy, typename std::enable_if<
		(std::is_pointer<T>::value || std::is_member_pointer<T>::value)
		&& ! GFunctionTraits<T>::IsFunction
	>::type>
{
private:
	typedef MemberDataTrait<T> MemberTrait;
	static constexpr bool IsMember = MemberTrait::Result;

public:
	typedef T DataType;

	typedef typename std::conditional<
		IsMember,
		typename MemberTrait::FieldType,
		typename std::remove_pointer<T>::type
	>::type ValueType;

	typedef const ValueType & PassType;
	
	static constexpr bool HasSetter = true;

public:
	static void set(DataType & data, const void * instance, const PassType & value){
		return doSet<DataType &>(data, instance, value);
	}
	
	static void * getAddress(DataType & data, const void * instance) {
		return doGetAddress<DataType &>(data, instance);
	}

private:
	template <typename U>
	static void doSet(typename std::enable_if<IsMember, U>::type data, const void * instance, const PassType & value) {
		(typename MemberTrait::ObjectType *)(instance)->*data = value;
	}

	template <typename U>
	static void doSet(typename std::enable_if<! IsMember, U>::type data, const void * /*instance*/, const PassType & value) {
		*data = value;
	}

	template <typename U>
	static void * doGetAddress(typename std::enable_if<IsMember, U>::type data, const void * instance) {
		return (void *)&(static_cast<const typename MemberTrait::ObjectType *>(instance)->*data);
	}

	template <typename U>
	static void * doGetAddress(typename std::enable_if<! IsMember, U>::type data, const void * /*instance*/) {
		return (void *)&*data;
	}
};

// setter via functor/function
template <typename T, typename Policy>
class GInstanceSetterImplement <T, Policy, typename std::enable_if<
		(GFunctionTraits<T>::IsFunction)
		|| (! std::is_pointer<T>::value && ! std::is_member_pointer<T>::value && ! std::is_fundamental<T>::value)
	>::type>
{
private:
	typedef decltype(makeCallback(std::declval<T>())) CallbackType;

	static constexpr bool ExplicitThis = (
		PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result
		|| PolicyHasRule<Policy, GMetaRuleGetterExplicitThis>::Result
	);

public:
	typedef CallbackType DataType;

	typedef typename std::conditional<
		ExplicitThis,
		typename TypeList_Get<typename CallbackType::TraitsType::ArgTypeList, 1>::Result,
		typename TypeList_Get<typename CallbackType::TraitsType::ArgTypeList, 0>::Result
	>::type ValueType;
	typedef ValueType PassType;
	
	static constexpr bool HasSetter = true;

public:
	static void set(DataType & data, const void * instance, const PassType & value) {
		doSet<DataType &>(data, instance, value);
	}
	
	static void * getAddress(DataType & /*data*/, const void * /*instance*/) {
		return nullptr;
	}

private:
	template <typename U>
	static void doSet(typename std::enable_if<ExplicitThis, U>::type data, const void * instance, const PassType & value) {
		data.setObject((void *)instance);
		data((typename TypeList_Get<typename CallbackType::TraitsType::ArgTypeList, 0>::Result)(instance), value);
	}

	template <typename U>
	static void doSet(typename std::enable_if<! ExplicitThis, U>::type data, const void * instance, const PassType & value) {
		data.setObject((void *)instance);
		data(value);
	}

};


} //namespace setter_internal

template <typename T, typename Policy = GMetaPolicyDefault>
class GInstanceSetter
{
private:
	typedef setter_internal::GInstanceSetterImplement<T, Policy> ImplmentType;

public:
	typedef typename ImplmentType::ValueType ValueType;
	typedef typename ImplmentType::PassType PassType;
	
	static constexpr bool HasSetter = ImplmentType::HasSetter;
	static constexpr bool Writable = HasSetter && ! PolicyHasRule<Policy, GMetaRuleForbidWrite>::Result;
	

public:
	GInstanceSetter() : setter() {
	}

	explicit GInstanceSetter(const T & setter) : setter(setter) {
	}
	
	GInstanceSetter(const GInstanceSetter & other) : setter(other.setter) {
	}
	
	GInstanceSetter & operator = (const GInstanceSetter & other) {
		this->setter = other.setter;
		return *this;
	}
	
	void set(const void * instance, const PassType & value) const {
		ImplmentType::set(this->setter, instance, value);
	}
	
	void * getAddress(const void * instance) const {
		return ImplmentType::getAddress(this->setter, instance);
	}

private:
	mutable typename ImplmentType::DataType setter;
};


template <typename T, typename Policy = GMetaPolicyDefault>
class GSetter : public GInstanceSetter<T, Policy>
{
private:
	typedef GInstanceSetter<T, Policy> super;

public:
	GSetter(const void * instance, const T & setter)
		: super(setter), instance(instance)
	{
	}
	
	GSetter(const GSetter & other)
		: super(other), instance(other.instance)
	{
	}
	
	GSetter & operator = (const GSetter & other) {
		super::operator = (other);
		this->instance = other.instance;
		
		return *this;
	}

	void set(const typename super::PassType & value) const {
		super::set(this->instance, value);
	}
	
	void operator() (const typename super::PassType & value) const {
		this->set(value);
	}
	
	GSetter & operator = (const typename super::PassType & value) {
		this->set(value);
		
		return *this;
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


} //namespace cpgf


#endif
