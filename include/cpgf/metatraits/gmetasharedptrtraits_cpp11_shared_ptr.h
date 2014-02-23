#ifndef CPGF_GMETASHAREDPTRTRAITS_CPP11_SHARED_PTR_H
#define CPGF_GMETASHAREDPTRTRAITS_CPP11_SHARED_PTR_H

#include "cpgf/metatraits/gmetasharedptrtraits.h"

#include <memory>


namespace cpgf {

template <typename T>
struct GMetaTraitsCreateSharedPointerTraits <std::shared_ptr<T>, void >
{
	static IMetaSharedPointerTraits * createSharedPointerTraits(const GMetaTraitsParam &) {
		return new gmetasharedptrtraits_internal::GMetaTraitsCreateSharedPointerTraitsGeneral<T, std::shared_ptr<T> >();
	}
};

} // namespace cpgf


#endif
