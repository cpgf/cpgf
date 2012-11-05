#ifndef __GMETACORE_H
#define __GMETACORE_H


namespace cpgf {

class GScriptObject;
struct IMetaClass;

class GMetaCore
{
public:
	explicit GMetaCore(GScriptObject * scriptObject);
	IMetaClass * cloneClass(IMetaClass * metaClass);

private:
	GScriptObject * scriptObject;
};


} // namespace cpgf



#endif
