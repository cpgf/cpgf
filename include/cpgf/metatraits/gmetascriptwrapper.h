#ifndef __GMETASCRIPTWRAPPER_H
#define __GMETASCRIPTWRAPPER_H


#include "cpgf/gapiutil.h"


namespace cpgf {


struct IScriptDataStorage;
struct GMetaTraitsParam;

struct IMetaScriptWrapper : public IObject
{
	virtual void G_API_CC setScriptDataStorage(void * instance, IScriptDataStorage * scriptDataStorage) = 0;
};

} // namespace cpgf


namespace cpgf_metatraits {

template <typename T>
struct GMetaTraitsCreateScriptWrapper
{
	static cpgf::IMetaScriptWrapper * createScriptWrapper(const cpgf::GMetaTraitsParam &) {
		return NULL;
	}
};

template <typename T>
inline cpgf::IMetaScriptWrapper * metaTraitsCreateScriptWrapper(const cpgf::GMetaTraitsParam & param, ...)
{
	return GMetaTraitsCreateScriptWrapper<T>::createScriptWrapper(param);
}



} // namespace cpgf_metatraits



#endif
