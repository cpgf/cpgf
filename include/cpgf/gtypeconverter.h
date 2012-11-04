#ifndef __GTYPECONVERTER_H
#define __GTYPECONVERTER_H

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


} // namespace cpgf



#endif
