#ifndef __GMETACORE_H
#define __GMETACORE_H

#include "cpgf/gvariant.h"


namespace cpgf {

class GScriptObject;
struct IMetaClass;

class GMetaCore
{
public:
	explicit GMetaCore(GScriptObject * scriptObject);
	IMetaClass * cloneClass(IMetaClass * metaClass);
	GVariant cast(const GVariant & instance, IMetaClass * targetMetaClass = NULL);

private:
	GScriptObject * scriptObject;
};


} // namespace cpgf



#endif
