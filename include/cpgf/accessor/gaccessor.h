#ifndef CPGF_GACCESSOR_H
#define CPGF_GACCESSOR_H

#include "cpgf/accessor/ggetter.h"
#include "cpgf/accessor/gsetter.h"
#include "cpgf/gifelse.h"

#include <type_traits>

namespace cpgf {

template <typename RawGetter, typename RawSetter, typename Policy = GMetaPolicyDefault>
class GInstanceAccessor
{
public:
	typedef GInstanceGetter<RawGetter, Policy> GetterType;
	typedef GInstanceSetter<RawSetter, Policy> SetterType;

	typedef typename std::conditional<
		GetterType::HasGetter,
		typename GetterType::ValueType,
		typename SetterType::ValueType
	>::type ValueType;
	
public:
	GInstanceAccessor(RawGetter rawGetter, RawSetter rawSetter) : getter(GetterType(rawGetter)), setter(SetterType(rawSetter)) {
	}
	
	GInstanceAccessor(const GInstanceAccessor & other) : getter(other.getter), setter(other.setter) {
	}
	
	GInstanceAccessor & operator = (const GInstanceAccessor & other) {
		this->getter = other.getter;
		this->setter = other.setter;
		
		return *this;
	}

	typename GetterType::PassType get(void * instance) const {
		return this->getter.get(instance);
	}
	
	void set(void * instance, const typename SetterType::PassType & value) const {
		this->setter.set(instance, value);
	}
	
	void * getAddress(void * instance) const {
		if(GetterType::HasGetter) {
			return this->getter.getAddress(instance);
		}
		else {
			return this->setter.getAddress(instance);
		}
	}
private:
	GetterType getter;
	SetterType setter;
};


template <typename RawGetter, typename RawSetter, typename Policy = GMetaPolicyDefault>
class GAccessor
{
public:
	typedef GInstanceGetter<RawGetter, Policy> GetterType;
	typedef GInstanceSetter<RawSetter, Policy> SetterType;

	typedef typename std::conditional<
		GetterType::HasGetter,
		typename GetterType::ValueType,
		typename SetterType::ValueType
	>::type ValueType;
	
public:
	GAccessor(const void * instance, RawGetter rawGetter, RawSetter rawSetter)
		: instance(instance), getter(GetterType(rawGetter)), setter(SetterType(rawSetter)) {
	}

	GAccessor(const GAccessor & other) : instance(other.instance), getter(other.getter), setter(other.setter) {
	}
	
	GAccessor & operator = (const GAccessor & other) {
		this->getter = other.getter;
		this->setter = other.setter;
		this->instance = other.instance;
		
		return *this;
	}
	
	bool canRead() const {
		return GetterType::Readable;
	}

	bool canWrite() const {
		return SetterType::Writable;
	}

	typename GetterType::PassType get() const {
		return this->getter.get(this->instance);
	}
	
	typename GetterType::PassType operator() () const {
		return this->get();
	}
	
	operator typename GetterType::PassType () const {
		return this->get();
	}
	
	void set(typename SetterType::PassType value) const {
		this->setter.set(const_cast<void *>(this->instance), value);
	}
	
	void operator() (typename SetterType::PassType value) const {
		this->set(value);
	}
	
	GAccessor & operator = (typename SetterType::PassType value) {
		this->set(value);
		
		return *this;
	}

	void * getAddress() const {
		if(GetterType::HasGetter) {
			return this->getter.getAddress(this->instance);
		}
		else {
			return this->setter.getAddress(const_cast<void *>(this->instance));
		}
	}

	const void * getInstance() const {
		return this->instance;
	}

	void setInstance(const void * newInstance) {
		this->instance = newInstance;
	}
	
private:
	const void * instance;
	GetterType getter;
	SetterType setter;
};


template <typename RawGetter, typename RawSetter, typename Policy>
GInstanceAccessor<RawGetter, RawSetter, Policy> createInstanceAccessor(RawGetter rawGetter, RawSetter rawSetter)
{
	return GInstanceAccessor<RawGetter, RawSetter, Policy>(rawGetter, rawSetter);
}

template <typename RawGetter, typename RawSetter>
GInstanceAccessor<RawGetter, RawSetter, GMetaPolicyDefault> createInstanceAccessor(RawGetter rawGetter, RawSetter rawSetter)
{
	return GInstanceAccessor<RawGetter, RawSetter, GMetaPolicyDefault>(rawGetter, rawSetter);
}

template <typename RawGetter, typename RawSetter, typename Policy>
GAccessor<RawGetter, RawSetter, Policy> createAccessor(const void * instance, RawGetter rawGetter, RawSetter rawSetter)
{
	return GAccessor<RawGetter, RawSetter, Policy>(instance, rawGetter, rawSetter);
}

template <typename RawGetter, typename RawSetter>
GAccessor<RawGetter, RawSetter, GMetaPolicyDefault> createAccessor(const void * instance, RawGetter rawGetter, RawSetter rawSetter)
{
	return GAccessor<RawGetter, RawSetter, GMetaPolicyDefault>(instance, rawGetter, rawSetter);
}


} // namespace cpgf



#endif
