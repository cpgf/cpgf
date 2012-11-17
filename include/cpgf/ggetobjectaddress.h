#ifndef __GGETOBJECTADDRESS_H
#define __GGETOBJECTADDRESS_H

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
