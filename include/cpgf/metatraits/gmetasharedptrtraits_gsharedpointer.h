#ifndef __GMETASHAREDPTRTRAITS_GSHAREDPOINTER_H
#define __GMETASHAREDPTRTRAITS_GSHAREDPOINTER_H

#include "cpgf/gsharedptr.h"
#include "cpgf/metatraits/gmetasharedptrtraits.h"


namespace cpgf {

template <typename T>
struct GMetaTraitsCreateSharedPointerTraits <GSharedPointer<T> >
{
	static IMetaSharedPointerTraits * createSharedPointerTraits(const GMetaTraitsParam &) {
		return new gmetasharedptrtraits_internal::GMetaTraitsCreateSharedPointerTraitsGeneral<T, GSharedPointer<T> >();
	}
};



} // namespace cpgf


#endif
