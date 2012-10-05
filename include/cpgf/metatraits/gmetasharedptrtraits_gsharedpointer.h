#ifndef __GMETASHAREDPTRTRAITS_GSHAREDPOINTER_H
#define __GMETASHAREDPTRTRAITS_GSHAREDPOINTER_H

#include "cpgf/gsharedptr.h"
#include "cpgf/metatraits/gmetasharedptrtraits.h"
#include "cpgf/metatraits/gmetatraitsparam.h"


namespace cpgf {

namespace gmetasharedptrtraits_gsharedpointer_internal {

template <typename T>
class GMetaTraitsCreateSharedPointerTraitsGSharedPointer : public IMetaSharedPointerTraits
{
private:
	typedef GSharedPointer<T> SharedPointerType;

	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT
	
protected:	
	virtual void * G_API_CC getPointer(void * sharedPointer) {
		return static_cast<SharedPointerType *>(sharedPointer)->get();
	}
	
	virtual void G_API_CC getMetaType(GMetaTypeData * outType) {
		GMetaType type = createMetaType<T>();
		fixupMetaType(&type);
		*outType = type.getData();
	}
};


} // namespace gmetasharedptrtraits_gsharedpointer_internal


template <typename T>
struct GMetaTraitsCreateSharedPointerTraits <GSharedPointer<T> >
{
	static IMetaSharedPointerTraits * createSharedPointerTraits(const GMetaTraitsParam &) {
		return new gmetasharedptrtraits_gsharedpointer_internal::GMetaTraitsCreateSharedPointerTraitsGSharedPointer<T>();
	}
};



} // namespace cpgf


#endif
