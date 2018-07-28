#ifndef CPGF_GSCOPEDINTERFACE_H
#define CPGF_GSCOPEDINTERFACE_H

#include <cstddef>
#include <algorithm>
#include <memory>

namespace cpgf {


template <typename T>
struct GScopedInterfaceDeleter
{
	inline void operator() (T * p) const {
		if(p) {
			p->releaseReference();
		}
	}
};

template <typename T>
using GScopedInterface = std::unique_ptr<T, GScopedInterfaceDeleter<T> >;


} // namespace cpgf


#endif
