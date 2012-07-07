#ifndef __GMETASCRIPTWRAPPER_H
#define __GMETASCRIPTWRAPPER_H


#include "cpgf/gapiutil.h"


namespace cpgf {


struct IScriptObject;
struct GMetaTraitsParam;

struct IMetaScriptWrapper : public IObject
{
	virtual void G_API_CC setScriptObject(void * instance, IScriptObject * scriptObject) = 0;
};


template <typename T>
struct GMetaTraitsCreateScriptWrapper
{
	static IMetaScriptWrapper * createScriptWrapper(const GMetaTraitsParam &) {
		return NULL;
	}
};

template <typename T>
inline IMetaScriptWrapper * metaTraitsCreateScriptWrapper(const T &, const GMetaTraitsParam & param)
{
	return GMetaTraitsCreateScriptWrapper<T>::createScriptWrapper(param);
}



} // namespace cpgf



#endif
