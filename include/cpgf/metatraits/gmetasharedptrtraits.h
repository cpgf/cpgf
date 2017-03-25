#ifndef CPGF_GMETASHAREDPTRTRAITS_H
#define CPGF_GMETASHAREDPTRTRAITS_H

#include "cpgf/gapiutil.h"
#include "cpgf/gmetatype.h"

#include <type_traits>

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
	
public:
	virtual ~GMetaTraitsCreateSharedPointerTraitsGeneral() {}
	
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


namespace cpgf {

template <typename T, typename Enabled = void>
struct GMetaTraitsCreateSharedPointerTraits
{
	static IMetaSharedPointerTraits * createSharedPointerTraits(const GMetaTraitsParam &) {
		return nullptr;
	}
};

template <typename T>
IMetaSharedPointerTraits * createSharedPointerTraitsFromMetaTraits(const GMetaTraitsParam & param, T *)
{
	return GMetaTraitsCreateSharedPointerTraits<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::createSharedPointerTraits(param);
}

} // namespace cpgf


#endif
