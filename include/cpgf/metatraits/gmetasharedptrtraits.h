#ifndef __GMETASHAREDPTRTRAITS_H
#define __GMETASHAREDPTRTRAITS_H

#include "cpgf/gapiutil.h"
#include "cpgf/gmetatype.h"


namespace cpgf {

struct GMetaTraitsParam;

struct IMetaSharedPointerTraits : public IExtendObject
{
	virtual void * G_API_CC getPointer(const void * sharedPointer) = 0;
	virtual void G_API_CC getMetaType(GMetaTypeData * outType) = 0;
};


template <typename T>
struct GMetaTraitsCreateSharedPointerTraits
{
	static IMetaSharedPointerTraits * createTraits(const GMetaTraitsParam &) {
		return NULL;
	}
};

template <typename T>
inline IMetaSharedPointerTraits * metaTraitsCreateSharedPointerTraits(const T &, const GMetaTraitsParam & param)
{
	return GMetaTraitsCreateSharedPointerTraits<T>::createTraits(param);
}



} // namespace cpgf




#endif
