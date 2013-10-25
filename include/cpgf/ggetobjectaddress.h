#ifndef CPGF_GGETOBJECTADDRESS_H
#define CPGF_GGETOBJECTADDRESS_H

// suppress all warnings
#ifdef __GNUC__
#pragma GCC system_header
#endif

// Get an object address without triggering operator & overloading

namespace cpgf {

template <typename T>
inline T * getObjectAddress(T & obj)
{
	return reinterpret_cast<T *>(
		&const_cast<char &>(
		reinterpret_cast<const volatile char &>(obj)));
}


} // namespace cpgf


#endif
