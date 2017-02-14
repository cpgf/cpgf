#ifndef CPGF_GFLAGS_H
#define CPGF_GFLAGS_H

#include "cpgf/genableif.h"
#include "cpgf/gtypetraits.h"

namespace cpgf {

namespace gflags_internal {
	template <unsigned int Size>
	struct GFlagsValueType
	{
		typedef unsigned int ValueType;
	};

	template <>
	struct GFlagsValueType <4>
	{
		typedef unsigned int ValueType;
	};

	template <>
	struct GFlagsValueType <8>
	{
		typedef unsigned long long ValueType;
	};
}


template <typename T>
class GFlags
{
private:
	typedef typename gflags_internal::GFlagsValueType<sizeof(T)>::ValueType ValueType;

public:
	GFlags() : value((ValueType)T()) {
	}
	
	GFlags(T flags) : value((ValueType)flags) {
	}

	template <typename U>
	GFlags(U flags, typename GEnableIf<IsInteger<U>::Result && !IsSameType<U, T>::Result >::Result * = 0) : value((ValueType)flags) {
	}

	GFlags(const GFlags & other) : value(other.value) {
	}
	
	GFlags & operator = (const GFlags & other) {
		this->value = other.value;

		return *this;
	}
	
	operator T () const {
		return (T)this->value;
	}

	operator unsigned int () const {
		return static_cast<unsigned int>(this->value);
	}
	
	bool operator ! () const {
		return this->value == (ValueType)T(0);
	}
	
	void set(T flags) {
		this->value |= (ValueType)flags;
	}
	
	void clear(T flags) {
		this->value &= ~(ValueType)flags;
	}
	
	void toggle(T flags) {
		this->value ^= (ValueType)flags;
	}
	
	void setByBool(T flags, bool value) {
		if(value) {
			this->set(flags);
		}
		else {
			this->clear(flags);
		}
	}
	
	bool has(T flags) const {
		return (this->value & (ValueType)flags) == static_cast<ValueType>(flags);
	}
	
	bool has(ValueType flags) const {
		return (this->value & (ValueType)flags) == static_cast<ValueType>(flags);
	}
	
	bool hasAny(T flags) const {
		return (this->value & (ValueType)flags) != 0;
	}
	
	bool hasAny(ValueType flags) const {
		return (this->value & (ValueType)flags) != 0;
	}
	
private:
	ValueType value;	
};

typedef GFlags<unsigned int> GFlags32;
typedef GFlags<unsigned long long> GFlags64;



} // namespace cpgf



#endif

