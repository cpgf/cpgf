#include "cpgf/metautility/gmetacore.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/scriptbind/gscriptbind.h"


namespace cpgf {

GMetaCore::GMetaCore(GScriptObject * scriptObject)
	: scriptObject(scriptObject)
{
}

IMetaClass * GMetaCore::cloneClass(IMetaClass * metaClass)
{
	return this->scriptObject->cloneMetaClass(metaClass);
}



} // namespace cpgf
