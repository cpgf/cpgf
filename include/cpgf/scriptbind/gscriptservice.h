#ifndef __GSCRIPTSERVICE_H
#define __GSCRIPTSERVICE_H


namespace cpgf {

class GScriptObject;
struct IMetaClass;

class GScriptCoreService
{
private:
	explicit GScriptCoreService(GScriptObject * scriptObject);
	
	IMetaClass * cloneClass(IMetaClass * metaClass);

private:
	GScriptObject * scriptObject;

private:
	friend GScriptCoreService * doCreateScriptCoreService(GScriptObject * scriptObject);

	template <typename D>
	friend void buildMetaClass_GScriptCoreService(D _d);
};



} // namespace cpgf



#endif
