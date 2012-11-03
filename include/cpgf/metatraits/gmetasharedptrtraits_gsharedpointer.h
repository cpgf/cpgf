#ifndef __GMETASHAREDPTRTRAITS_GSHAREDPOINTER_H
#define __GMETASHAREDPTRTRAITS_GSHAREDPOINTER_H

#include "cpgf/gsharedptr.h"
#include "cpgf/metatraits/gmetasharedptrtraits.h"


namespace cpgf_metatraits {

template <typename T>
struct GMetaTraitsCreateSharedPointerTraits <cpgf::GSharedPointer<T> >
{
	static cpgf::IMetaSharedPointerTraits * createSharedPointerTraits(const cpgf::GMetaTraitsParam &) {
		return new cpgf::gmetasharedptrtraits_internal::GMetaTraitsCreateSharedPointerTraitsGeneral<T, cpgf::GSharedPointer<T> >();
	}
};

} // namespace cpgf_metatraits


#endif
