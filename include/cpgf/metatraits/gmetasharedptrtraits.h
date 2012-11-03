#ifndef __GMETASHAREDPTRTRAITS_H
#define __GMETASHAREDPTRTRAITS_H

#include "cpgf/gapiutil.h"
#include "cpgf/gmetatype.h"


namespace cpgf {

struct GMetaTraitsParam;

struct IMetaSharedPointerTraits : public IExtendObject
{
	virtual void * G_API_CC getPointer(void * sharedPointer) = 0;
	virtual void G_API_CC getMetaType(GMetaTypeData * outType) = 0;
};


namespace gmetasharedptrtraits_internal {

template <typename T, typename SP>
class GMetaTraitsCreateSharedPointerTraitsGeneral : public IMetaSharedPointerTraits
{
private:
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT
	
protected:	
	virtual void * G_API_CC getPointer(void * sharedPointer) {
		return static_cast<SP *>(sharedPointer)->get();
	}
	
	virtual void G_API_CC getMetaType(GMetaTypeData * outType) {
		GMetaType type = createMetaType<T>();
		fixupMetaType(&type);
		*outType = type.refData();
	}
};


} // namespace gmetasharedptrtraits_internal


} // namespace cpgf


namespace cpgf_metatraits {

template <typename T>
struct GMetaTraitsCreateSharedPointerTraits
{
	static cpgf::IMetaSharedPointerTraits * createSharedPointerTraits(const cpgf::GMetaTraitsParam &) {
		return NULL;
	}
};

inline cpgf::IMetaSharedPointerTraits * metaTraitsCreateSharedPointerTraits(const cpgf::GMetaTraitsParam &, ...)
{
	return NULL;
}

template <typename T>
cpgf::IMetaSharedPointerTraits * createSharedPointerTraitsFromMetaTraits(const cpgf::GMetaTraitsParam & param, T * p)
{
	cpgf::IMetaSharedPointerTraits * sharedPointerTraits = metaTraitsCreateSharedPointerTraits(param, p);
	if(sharedPointerTraits == NULL) {
		sharedPointerTraits = GMetaTraitsCreateSharedPointerTraits<T>::createSharedPointerTraits(param);
	}
	return sharedPointerTraits;
}


} // namespace cpgf_metatraits




#endif
