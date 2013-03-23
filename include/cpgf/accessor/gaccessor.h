#ifndef __GACCESSOR_H
#define __GACCESSOR_H

#include "cpgf/accessor/ggetter.h"
#include "cpgf/accessor/gsetter.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
#endif

#ifdef G_COMPILER_CPPBUILDER
#pragma warn -8008 //Condition is always true
#pragma warn -8066 //Unreachable code
#endif


namespace cpgf {


template <typename RawGetter, typename RawSetter, typename Policy = GMetaPolicyDefault>
class GAccessor
{
public:
	typedef GGetter<RawGetter, Policy> GetterType;
	typedef GSetter<RawSetter, Policy> SetterType;
	
public:
	GAccessor(RawGetter rawGetter, RawSetter rawSetter) : getter(GetterType(rawGetter)), setter(SetterType(rawSetter)) {
	}
	
	GAccessor(const GAccessor & other) : getter(other.getter), setter(other.setter) {
	}
	
	GAccessor & operator = (const GAccessor & other) {
		this->getter = other.getter;
		this->setter = other.setter;
		
		return *this;
	}

	typename GetterType::PassType get(void * instance) const {
		return this->getter.get(instance);
	}
	
	void set(void * instance, typename SetterType::PassType value) const {
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
class GInstanceAccessor
{
public:
	typedef GGetter<RawGetter, Policy> GetterType;
	typedef GSetter<RawSetter, Policy> SetterType;
	
public:
	GInstanceAccessor(RawGetter rawGetter, RawSetter rawSetter, const void * instance) : getter(GetterType(rawGetter)), setter(SetterType(rawSetter)), instance(instance) {
	}

	GInstanceAccessor(const GInstanceAccessor & other) : getter(other.getter), setter(other.setter), instance(other.instance) {
	}
	
	GInstanceAccessor & operator = (const GInstanceAccessor & other) {
		this->getter = other.getter;
		this->setter = other.setter;
		this->instance = other.instance;
		
		return *this;
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
	
	GInstanceAccessor & operator = (typename SetterType::PassType value) {
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
	GetterType getter;
	SetterType setter;
	const void * instance;
};


template <typename RawGetter, typename RawSetter, typename Policy>
GAccessor<RawGetter, RawSetter, Policy> createAccessor(RawGetter rawGetter, RawSetter rawSetter)
{
	return GAccessor<RawGetter, RawSetter, Policy>(rawGetter, rawSetter);
}

template <typename RawGetter, typename RawSetter>
GAccessor<RawGetter, RawSetter, GMetaPolicyDefault> createAccessor(RawGetter rawGetter, RawSetter rawSetter)
{
	return GAccessor<RawGetter, RawSetter, GMetaPolicyDefault>(rawGetter, rawSetter);
}

template <typename RawGetter, typename RawSetter, typename Policy>
GInstanceAccessor<RawGetter, RawSetter, Policy> createInstanceAccessor(RawGetter rawGetter, RawSetter rawSetter, const void * instance)
{
	return GInstanceAccessor<RawGetter, RawSetter, Policy>(rawGetter, rawSetter, instance);
}

template <typename RawGetter, typename RawSetter>
GInstanceAccessor<RawGetter, RawSetter, GMetaPolicyDefault> createInstanceAccessor(RawGetter rawGetter, RawSetter rawSetter, const void * instance)
{
	return GInstanceAccessor<RawGetter, RawSetter, GMetaPolicyDefault>(rawGetter, rawSetter, instance);
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
