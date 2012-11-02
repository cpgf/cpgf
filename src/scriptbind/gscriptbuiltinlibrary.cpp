#include "cpgf/scriptbind/gscriptbindutil.h"

#include "cpgf/metadata/metautility/gmetadata_metabytearray.h"
#include "cpgf/metadata/metautility/gmetadata_metaobjectarray.h"
#include "cpgf/metadata/metautility/gmetadata_metadebug.h"
#include "cpgf/metautility/gmetabytearray.h"
#include "cpgf/metautility/gmetaobjectarray.h"
#include "cpgf/metautility/gmetadebug.h"

#include "cpgf/scriptbind/gscriptlibraryapi.h"

#include "cpgf/metadata/private/gmetadata_header.h"

#include "cpgf/metatraits/gmetasharedptrtraits_gsharedpointer.h"


namespace cpgf {


IScriptLibraryLoader * createBuiltinLibraries();

namespace {

GSharedPointer<GMetaByteArray> createByteArray()
{
	return GSharedPointer<GMetaByteArray>(new GMetaByteArray);
}

GSharedPointer<GMetaByteArray> createByteArrayWithLength(size_t length)
{
	return GSharedPointer<GMetaByteArray>(new GMetaByteArray(length));
}

GSharedPointer<GMetaObjectArray> createObjectArray(IMetaClass * metaClass)
{
	return GSharedPointer<GMetaObjectArray>(new GMetaObjectArray(metaClass));
}

} // unnamed namespace


bool loadByteArray(IScriptObject * owner, const char * namespaces, const char * /*libraryName*/)
{
	GDefineMetaNamespace ns = GDefineMetaNamespace::declare("");
	
	ns._method("createByteArray", &createByteArray);
	ns._method("createByteArray", &createByteArrayWithLength);

	GDefineMetaClass<GMetaByteArray> gbyteArrayDefine = GDefineMetaClass<GMetaByteArray>::Policy<GMetaPolicyNoCopyConstructor>::declare("GMetaByteArray");
	buildMetaData_metaByteArray(gbyteArrayDefine);
	ns._class(gbyteArrayDefine);
	
	GScopedInterface<IMetaClass> metaClass(static_cast<IMetaClass *>(metaItemToInterface(ns.takeMetaClass(), true)));
	owner->holdObject(metaClass.get());
	injectObjectToScript(owner, metaClass.get(), NULL, namespaces);
	
	return true;
}

bool loadObjectArray(IScriptObject * owner, const char * namespaces, const char * /*libraryName*/)
{
	GDefineMetaNamespace ns = GDefineMetaNamespace::declare("");
	
	ns._method("createObjectArray", &createObjectArray);

	GDefineMetaClass<GMetaObjectArray> gobjectArrayDefine = GDefineMetaClass<GMetaObjectArray>::Policy<GMetaPolicyNoDefaultAndCopyConstructor>::declare("GMetaObjectArray");
	buildMetaData_metaObjectArray(gobjectArrayDefine);
	ns._class(gobjectArrayDefine);
	
	GScopedInterface<IMetaClass> metaClass(static_cast<IMetaClass *>(metaItemToInterface(ns.takeMetaClass(), true)));
	owner->holdObject(metaClass.get());
	injectObjectToScript(owner, metaClass.get(), NULL, namespaces);
	
	return true;
}

bool loadDebug(IScriptObject * owner, const char * namespaces, const char * /*libraryName*/)
{
	GDefineMetaClass<GMetaDebug> debugDefine = GDefineMetaClass<GMetaDebug>::declare("GMetaDebug");
	buildMetaData_metaDebug(debugDefine);
	
	GScopedInterface<IMetaClass> metaClass(static_cast<IMetaClass *>(metaItemToInterface(debugDefine.takeMetaClass(), true)));
	owner->holdObject(metaClass.get());
	injectObjectToScript(owner, metaClass.get(), NULL, namespaces);
	
	return true;
}

void initializeBuiltinLibraries(GScriptLibraryNamedLoaderHandler * libraryHandler)
{
	libraryHandler->addHandler("builtin.arrays.bytearray", GScriptLibraryLoaderCallback(&loadByteArray));
	libraryHandler->addHandler("builtin.arrays.objectarray", GScriptLibraryLoaderCallback(&loadObjectArray));
	libraryHandler->addHandler("builtin.debug", GScriptLibraryLoaderCallback(&loadDebug));
}

IScriptLibraryLoader * createBuiltinLibraries()
{
	GScriptLibraryNamedLoaderHandler * namedLoader = new GScriptLibraryNamedLoaderHandler();
	GScopedInterface<IScriptLibraryLoaderHandler> namedLoaderInterface(namedLoader);
	initializeBuiltinLibraries(namedLoader);
	GScriptLibraryLoader * loader = new GScriptLibraryLoader();
	loader->addHandler(namedLoaderInterface.get());
	return loader;
}


} // namespace cpgf

