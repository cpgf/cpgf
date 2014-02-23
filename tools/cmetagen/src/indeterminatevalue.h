#ifndef CPGF_INDETERMINATEVALUE_H
#define CPGF_INDETERMINATEVALUE_H

namespace metagen {

template <typename T>
class IndeterminateValue
{
public:
	IndeterminateValue() : value(), determined(false) {
	}

	IndeterminateValue(const T & value)
		: value(value), determined(true) {
	}

	IndeterminateValue(const IndeterminateValue & other)
		: value(other.value), determined(other.determined) {
	}

	const T & get(const T & valueIfItsIndeterminate) const {
		return this->isIndeterminate() ? valueIfItsIndeterminate : this->value;
	}

	const T & get() const {
		return this->get(T());
	}

	void set(const T & newValue) {
		this->determined = true;
		this->value = newValue;
	}

	bool isIndeterminate() const {
		return !this->determined;
	}

	bool isDeterminate() const {
		return !this->determined;
	}

	operator const T & () const {
		return this->value;
	}

	operator T () {
		return this->value;
	}

	IndeterminateValue & operator = (const IndeterminateValue & other) {
		this->value = other.value;
		this->determined = other.determined;
		return *this;
	}

	IndeterminateValue & operator = (const T & value) {
		this->value = value;
		this->determined = true;
		return *this;
	}

private:
	T value;
	bool determined;
};



} // namespace metagen


#endif
