#ifndef __GSCRIPTWRAPPER_H
#define __GSCRIPTWRAPPER_H


#include "cpgf/metatraits/gmetascriptwrapper.h"
#include "cpgf/gapi.h"


namespace cpgf {

struct IScriptFunction;

struct IScriptDataStorage : public IObject
{
	virtual IScriptFunction * G_API_CC getScriptFunction(const char * name) = 0;
};

class GScriptWrapper
{
public:
	IScriptFunction * getScriptFunction(const char * name);

	void setScriptDataStorage(IScriptDataStorage * scriptDataStorage);

private:
	GSharedInterface<IScriptDataStorage> scriptDataStorage;
};


IMetaScriptWrapper * metaTraitsCreateScriptWrapper(const GScriptWrapper & wrapper, const GMetaTraitsParam & param);



} // namespace cpgf



#endif
