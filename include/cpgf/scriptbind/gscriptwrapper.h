#ifndef __GSCRIPTWRAPPER_H
#define __GSCRIPTWRAPPER_H


#include "cpgf/metatraits/gmetascriptwrapper.h"


namespace cpgf {

struct IScriptFunction;

class GScriptWrapper
{
public:
	IScriptFunction * getScriptFunction(const char * name);

	void setScriptObject(IScriptObject * scriptObject);

private:
	GSharedInterface<IScriptObject> scriptObject;
};


IMetaScriptWrapper * metaTraitsCreateScriptWrapper(const GScriptWrapper & wrapper, const GMetaTraitsParam & param);



} // namespace cpgf



#endif
