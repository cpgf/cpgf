#ifndef CPGF_GGETTER_H
#define CPGF_GGETTER_H

#include "cpgf/gfunctiontraits.h"
#include "cpgf/gtypetraits.h"
#include "cpgf/gcallback.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/gexception.h"
#include "cpgf/gerrorcode.h"

#include <type_traits>

namespace cpgf {

namespace getter_internal {

template <typename T, typename Policy, typename EnableIf = void>
class GInstanceGetterImplement
{
public:
	typedef int DataType;
	typedef int ValueType;
	typedef ValueType PassType;
	
	static constexpr bool HasGetter = false;

public:
	static PassType get(DataType & /*data*/, const void * /*instance*/) {
		raiseCoreException(Error_Meta_ReadDenied);

		return ValueType();
	}

	static void * getAddress(DataType & /*data*/, const void * /*instance*/) {
		raiseCoreException(Error_Meta_ReadDenied);

		return nullptr;
	}
};

// getter via data address
template <typename T, typename Policy>
class GInstanceGetterImplement <T, Policy, typename std::enable_if<
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

	typedef ValueType & PassType;
	
	static constexpr bool HasGetter = true;

public:
	static PassType get(DataType & data, const void * instance){
		return doGet<DataType &>(data, instance);
	}
	
	static void * getAddress(DataType & data, const void * instance) {
		return doGetAddress<DataType &>(data, instance);
	}

private:
	template <typename U>
	static PassType doGet(typename std::enable_if<IsMember, U>::type data, const void * instance) {
		return (typename MemberTrait::ObjectType *)(instance)->*data;
	}

	template <typename U>
	static PassType doGet(typename std::enable_if<! IsMember, U>::type data, const void * /*instance*/) {
		return *data;
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

// getter via functor/function
template <typename T, typename Policy>
class GInstanceGetterImplement <T, Policy, typename std::enable_if<
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

	typedef typename CallbackType::TraitsType::ResultType ValueType;
	typedef ValueType PassType;
	
	static constexpr bool HasGetter = true;

public:
	static PassType get(DataType & data, const void * instance) {
		return doGet<DataType &>(data, instance);
	}
	
	static void * getAddress(DataType & /*data*/, const void * /*instance*/) {
		return nullptr;
	}

private:
	template <typename U>
	static PassType doGet(typename std::enable_if<ExplicitThis, U>::type data, const void * instance) {
		data.setObject((void *)instance);
		return data((typename TypeList_Get<typename CallbackType::TraitsType::ArgTypeList, 0>::Result)(instance));
	}

	template <typename U>
	static PassType doGet(typename std::enable_if<! ExplicitThis, U>::type data, const void * instance) {
		data.setObject((void *)instance);
		return data();
	}

};


} //namespace getter_internal

template <typename T, typename Policy = GMetaPolicyDefault>
class GInstanceGetter
{
private:
	typedef getter_internal::GInstanceGetterImplement<T, Policy> ImplmentType;

public:
	typedef typename ImplmentType::ValueType ValueType;
	typedef typename ImplmentType::PassType PassType;
	
	static constexpr bool HasGetter = ImplmentType::HasGetter;
	static constexpr bool Readable = HasGetter && ! PolicyHasRule<Policy, GMetaRuleForbidRead>::Result;
	
public:
	GInstanceGetter() : getter() {
	}

	explicit GInstanceGetter(const T & getter) : getter(getter) {
	}
	
	GInstanceGetter(const GInstanceGetter & other) : getter(other.getter) {
	}
	
	GInstanceGetter & operator = (const GInstanceGetter & other) {
		this->getter = other.getter;
		return *this;
	}
	
	PassType get(const void * instance) const {
		return ImplmentType::get(this->getter, instance);
	}
	
	void * getAddress(const void * instance) const {
		return ImplmentType::getAddress(this->getter, instance);
	}

private:
	mutable typename ImplmentType::DataType getter;
};


template <typename T, typename Policy = GMetaPolicyDefault>
class GGetter : public GInstanceGetter<T, Policy>
{
private:
	typedef GInstanceGetter<T, Policy> super;

public:
	GGetter(const void * instance, const T & getter)
		: super(getter), instance(instance)
	{
	}
	
	GGetter(const GGetter & other)
		: super(other), instance(other.instance)
	{
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


} //namespace cpgf


#endif
