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


template <typename T>
struct GMetaTraitsCreateScriptWrapper
{
	static IMetaScriptWrapper * createScriptWrapper(const GMetaTraitsParam &) {
		return NULL;
	}
};

template <typename T>
inline IMetaScriptWrapper * metaTraitsCreateScriptWrapper(const GMetaTraitsParam & param, ...)
{
	return GMetaTraitsCreateScriptWrapper<T>::createScriptWrapper(param);
}



} // namespace cpgf



#endif
