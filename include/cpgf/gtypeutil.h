#ifndef __GTYPEUTIL_H
#define __GTYPEUTIL_H

#include "cpgf/gcallback.h"


namespace cpgf {

typedef GCallback<void * (void *)> GTypeConverterCallback;

template <typename From, typename To>
struct GTypeConverter
{
	void * operator() (void * p) const {
		return static_cast<To>(static_cast<From>(p));
	}
};


typedef GCallback<void (void *)> GTypeDeleterCallback;

template <typename T>
struct GTypeDeleter_Delete
{
	void operator() (void * p) const {
		delete static_cast<T *>(p);
	}
};


} // namespace cpgf



#endif
