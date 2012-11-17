#ifndef __GREFERENCE_h
#define __GREFERENCE_h

#include "cpgf/ggetobjectaddress.h"


namespace cpgf {

template <typename T>
class GReference
{
public:
	typedef T Type;

public:
	explicit GReference(T & data) : dataAddress(getObjectAddress(data)) {
	}

	GReference(const GReference & other) : dataAddress(other.dataAddress) {
	}

	GReference & operator = (const GReference & other) {
		this->dataAddress = other.dataAddress;
	}

	T * operator & () const {
		return this->dataAddress;
	}

	operator T& () const {
		return *this->dataAddress;
	}

private:
	bool operator == (const GReference & other);

private:
	T * dataAddress;
};


template <typename T>
inline GReference<T> makeReference(T & data)
{
	return GReference<T>(data);
}

template <typename T>
inline GReference<T> makeReference(const GReference<T> & data)
{
	return data;
}

template <typename T>
inline GReference<const T> makeReference(const GReference<const T> & data)
{
	return data;
}

template <typename T>
inline GReference<const volatile T> makeReference(const GReference<const volatile T> & data)
{
	return data;
}

template <typename T>
inline GReference<const T> makeConstReference(const T & data)
{
	return GReference<const T>(data);
}

template <typename T>
inline GReference<T> makeConstReference(const GReference<T> & data)
{
	return data;
}

template <typename T>
inline GReference<const T> makeConstReference(const GReference<const T> & data)
{
	return data;
}

template <typename T>
inline GReference<const volatile T> makeConstReference(const GReference<const volatile T> & data)
{
	return data;
}


} //namespace cpgf


#endif
