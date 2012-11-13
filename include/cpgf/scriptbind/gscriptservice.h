#ifndef __GSCRIPTSERVICE_H
#define __GSCRIPTSERVICE_H

#include "cpgf/gapi.h"
#include "cpgf/gsharedinterface.h"

#include <string>


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
	GScriptCoreService(GScriptObject * scriptObject, const char * bindName, IScriptLibraryLoader * libraryLoader);

	bool loadLibrary(const char * namespaces, const GMetaVariadicParam * libraryNames);

private:
	GScriptObject * scriptObject;
	std::string bindName;
	GSharedInterface<IScriptLibraryLoader> libraryLoader;

private:
	friend GScriptCoreService * doBindScriptCoreService(GScriptObject * scriptObject, const char * bindName, IScriptLibraryLoader * libraryLoader);

	template <typename D>
	friend void buildMetaClass_GScriptCoreService(D _d);
};


class GScriptLibraryNamedLoaderHandler;

void initializeBuiltinLibraries(GScriptLibraryNamedLoaderHandler * libraryHandler);


} // namespace cpgf



#endif
