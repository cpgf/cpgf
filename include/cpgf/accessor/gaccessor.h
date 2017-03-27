#ifndef CPGF_GACCESSOR_H
#define CPGF_GACCESSOR_H

#include "cpgf/accessor/ggetter.h"
#include "cpgf/accessor/gsetter.h"
#include "cpgf/gifelse.h"

#include <type_traits>

namespace cpgf {

template <typename GetterType, typename SetterType>
class GInstanceAccessor
{
public:
	typedef typename std::conditional<
		GetterType::HasGetter,
		typename GetterType::ValueType,
		typename SetterType::ValueType
	>::type ValueType;
	
public:
	GInstanceAccessor(const GetterType & getter, const SetterType & setter)
		: getter(getter), setter(setter) {
	}

	GInstanceAccessor(const GInstanceAccessor & other)
		: getter(other.getter), setter(other.setter) {
	}
	
	GInstanceAccessor & operator = (const GInstanceAccessor & other) {
		if(this != &other) {
			this->getter = other.getter;
			this->setter = other.setter;
		}
		
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


template <typename GetterType, typename SetterType>
class GAccessor
{
public:
	typedef typename std::conditional<
		GetterType::HasGetter,
		typename GetterType::ValueType,
		typename SetterType::ValueType
	>::type ValueType;
	
public:
	GAccessor(const GetterType & getter, const SetterType & setter)
		: getter(getter), setter(setter) {
	}

	GAccessor(const GAccessor & other)
		: getter(other.getter), setter(other.setter) {
	}
	
	GAccessor & operator = (const GAccessor & other) {
		if(this != &other) {
			this->getter = other.getter;
			this->setter = other.setter;
		}
		
		return *this;
	}
	
	bool canRead() const {
		return GetterType::Readable;
	}

	bool canWrite() const {
		return SetterType::Writable;
	}

	typename GetterType::PassType get() const {
		return this->getter.get();
	}
	
	typename GetterType::PassType operator() () const {
		return this->get();
	}
	
	operator typename GetterType::PassType () const {
		return this->get();
	}
	
	void set(const typename SetterType::PassType & value) const {
		this->setter.set(value);
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
			return this->getter.getAddress();
		}
		else {
			return this->setter.getAddress();
		}
	}

	const void * getInstance() const {
		if(GetterType::HasGetter) {
			return this->getter.getInstance();
		}
		else {
			return this->setter.getInstance();
		}
	}

	void setInstance(const void * newInstance) {
		if(GetterType::HasGetter) {
			this->getter.setInstance(newInstance);
		}
		if(SetterType::HasSetter) {
			this->setter.setInstance(newInstance);
		}
	}
	
private:
	GetterType getter;
	SetterType setter;
};


template <typename RawGetter, typename RawSetter, typename Policy>
GInstanceAccessor<GInstanceGetter<RawGetter, Policy>, GInstanceSetter<RawSetter, Policy> >
createInstanceAccessor(
		const RawGetter & rawGetter,
		const RawSetter & rawSetter,
		Policy
	)
{
	return { rawGetter, rawSetter };
}

template <typename RawGetter, typename RawSetter>
GInstanceAccessor<GInstanceGetter<RawGetter, GMetaPolicyDefault>, GInstanceSetter<RawSetter, GMetaPolicyDefault> >
createInstanceAccessor(
		const RawGetter & rawGetter,
		const RawSetter & rawSetter
	)
{
	return { rawGetter, rawSetter };
}

template <typename RawGetter, typename RawSetter, typename Policy>
GInstanceAccessor<GInstanceGetter<RawGetter, Policy>, GInstanceSetter<RawSetter, Policy> >
createInstanceAccessor(
		const GInstanceGetter<RawGetter, Policy> & getter,
		const GInstanceSetter<RawSetter, Policy> & setter
	)
{
	return { getter, setter };
}


template <typename RawGetter, typename RawSetter, typename Policy>
GAccessor<GGetter<RawGetter, Policy>, GSetter<RawSetter, Policy> >
createAccessor(
		void * instance,
		const RawGetter & rawGetter,
		const RawSetter & rawSetter,
		Policy policy
	)
{
	return { createGetter(instance, rawGetter, policy), createSetter(instance, rawSetter, policy) };
}

template <typename RawGetter, typename RawSetter>
GAccessor<GGetter<RawGetter, GMetaPolicyDefault>, GSetter<RawSetter, GMetaPolicyDefault> >
createAccessor(
		void * instance,
		const RawGetter & rawGetter,
		const RawSetter & rawSetter
	)
{
	return { createGetter(instance, rawGetter), createSetter(instance, rawSetter) };
}

template <typename RawGetter, typename RawSetter, typename Policy>
GAccessor<GGetter<RawGetter, Policy>, GSetter<RawSetter, Policy> >
createAccessor(
		const GGetter<RawGetter, Policy> & getter,
		const GSetter<RawSetter, Policy> & setter
	)
{
	return { getter, setter };
}


} // namespace cpgf



#endif
