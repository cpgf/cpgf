#ifndef __GMETASHAREDPTRTRAITS_CPP11_SHARED_PTR_H
#define __GMETASHAREDPTRTRAITS_CPP11_SHARED_PTR_H

#include "cpgf/metatraits/gmetasharedptrtraits.h"

#include <memory>


namespace cpgf_metatraits {

template <typename T>
struct GMetaTraitsCreateSharedPointerTraits <std::shared_ptr<T> >
{
	static cpgf::IMetaSharedPointerTraits * createSharedPointerTraits(const cpgf::GMetaTraitsParam &) {
		return new cpgf::gmetasharedptrtraits_internal::GMetaTraitsCreateSharedPointerTraitsGeneral<T, std::shared_ptr<T> >();
	}
};

} // namespace cpgf_metatraits


#endif
