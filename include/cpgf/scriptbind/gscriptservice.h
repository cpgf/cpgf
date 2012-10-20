#ifndef __GSCRIPTSERVICE_H
#define __GSCRIPTSERVICE_H


#include "cpgf/gapi.h"


namespace cpgf {

class GScriptObject;
struct IMetaClass;
struct IScriptLibraryLoader;
struct GMetaVariadicParam;

class GScriptCoreService
{
public:
	~GScriptCoreService();
	
private:
	GScriptCoreService(GScriptObject * scriptObject, IScriptLibraryLoader * libraryLoader);

	IMetaClass * cloneClass(IMetaClass * metaClass);
	bool loadLibrary(const char * namespaces, const GMetaVariadicParam * libraryNames);

private:
	GScriptObject * scriptObject;
	GSharedInterface<IScriptLibraryLoader> libraryLoader;

private:
	friend GScriptCoreService * doCreateScriptCoreService(GScriptObject * scriptObject, IScriptLibraryLoader * libraryLoader);

	template <typename D>
	friend void buildMetaClass_GScriptCoreService(D _d);
};


class GScriptLibraryNamedLoaderHandler;

void initializeBuiltinLibraries(GScriptLibraryNamedLoaderHandler * libraryHandler);


} // namespace cpgf



#endif
