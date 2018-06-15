#ifndef CPGF_GMETASCRIPTWRAPPER_H
#define CPGF_GMETASCRIPTWRAPPER_H

#include "cpgf/gapiutil.h"

namespace cpgf {

struct IScriptDataStorage;
struct IScriptContext;
struct GMetaTraitsParam;

struct IMetaScriptWrapper : public IObject
{
	virtual void G_API_CC initializeScriptWrapper(void * instance, IScriptDataStorage * scriptDataStorage, IScriptContext * scriptContext) = 0;
};

template <typename T>
struct GMetaTraitsCreateScriptWrapper
{
	static IMetaScriptWrapper * createScriptWrapper(const GMetaTraitsParam &) {
		return nullptr;
	}
};

template <typename T>
inline IMetaScriptWrapper * metaTraitsCreateScriptWrapper(const GMetaTraitsParam & param, ...)
{
	return GMetaTraitsCreateScriptWrapper<T>::createScriptWrapper(param);
}


} // namespace cpgf


#endif
