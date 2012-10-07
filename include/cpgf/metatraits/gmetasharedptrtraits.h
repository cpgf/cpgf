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
		*outType = type.getData();
	}
};


} // namespace gmetasharedptrtraits_internal


template <typename T>
struct GMetaTraitsCreateSharedPointerTraits
{
	static IMetaSharedPointerTraits * createSharedPointerTraits(const GMetaTraitsParam &) {
		return NULL;
	}
};

template <typename T>
inline IMetaSharedPointerTraits * metaTraitsCreateSharedPointerTraits(const T &, const GMetaTraitsParam & param)
{
	return GMetaTraitsCreateSharedPointerTraits<T>::createSharedPointerTraits(param);
}



} // namespace cpgf




#endif
