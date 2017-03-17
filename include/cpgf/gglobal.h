#ifndef CPGF_GGLOBAL_H
#define CPGF_GGLOBAL_H

#include "cpgf/gassert.h"


namespace cpgf {

template <typename T, typename P>
T gdynamic_cast(P p)
{
	GASSERT(p == nullptr || dynamic_cast<T>(p) != nullptr);

	return static_cast<T>(p);
}


} // namespace cpgf



#endif

