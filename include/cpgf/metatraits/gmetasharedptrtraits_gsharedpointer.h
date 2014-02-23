#ifndef CPGF_GMETASHAREDPTRTRAITS_GSHAREDPOINTER_H
#define CPGF_GMETASHAREDPTRTRAITS_GSHAREDPOINTER_H

#include "cpgf/gsharedptr.h"
#include "cpgf/metatraits/gmetasharedptrtraits.h"


namespace cpgf {

template <typename T>
struct GMetaTraitsCreateSharedPointerTraits <GSharedPointer<T>, void >
{
	static IMetaSharedPointerTraits * createSharedPointerTraits(const cpgf::GMetaTraitsParam &) {
		return new gmetasharedptrtraits_internal::GMetaTraitsCreateSharedPointerTraitsGeneral<T, GSharedPointer<T> >();
	}
};

} // namespace cpgf


#endif
