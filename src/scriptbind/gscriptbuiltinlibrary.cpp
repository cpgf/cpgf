#include "cpgf/scriptbind/gscriptbindutil.h"

#include "cpgf/metadata/metautility/gmetadata_metabytearray.h"
#include "cpgf/metadata/metautility/gmetadata_metaobjectarray.h"
#include "cpgf/metadata/metautility/gmetadata_metadebug.h"
#include "cpgf/metadata/metautility/gmetadata_metacore.h"
#include "cpgf/metautility/gmetabytearray.h"
#include "cpgf/metautility/gmetaobjectarray.h"
#include "cpgf/metautility/gmetadebug.h"
#include "cpgf/metautility/gmetacore.h"

#include "cpgf/scriptbind/gscriptlibraryapi.h"
#include "cpgf/gobjectdeleterinterface.h"

#include "cpgf/metadata/private/gmetadata_header.h"

#include "cpgf/metatraits/gmetasharedptrtraits_gsharedpointer.h"

#include "cpgf/gscopedinterface.h"


namespace cpgf {


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


bool loadCore(GScriptObject * scriptObject, const char * namespaces, const char * /*libraryName*/)
{
	GScopedPointer<GMetaCore> core(new GMetaCore(scriptObject));

	GDefineMetaClass<GMetaCore> define = GDefineMetaClass<GMetaCore>::Policy<GMetaPolicyNoDefaultAndCopyConstructor>::declare("GMetaCore");
	buildMetaData_metaCore(define);

	injectObjectToScript(scriptObject, define.getMetaClass(), core.get(), namespaces);

	GScopedInterface<IMetaItem> metaItem(metaItemToInterface(define.takeMetaClass(), true));
	scriptObject->holdObject(metaItem.get());
	GScopedInterface<IObject> metaObject(createObjectDeleterInterface(core.take()));
	scriptObject->holdObject(metaObject.get());
	
	return true;
}

bool loadByteArray(GScriptObject * scriptObject, const char * namespaces, const char * /*libraryName*/)
{
	GDefineMetaNamespace ns = GDefineMetaNamespace::declare("");
	
	ns._method("createByteArray", &createByteArray);
	ns._method("createByteArray", &createByteArrayWithLength);

	GDefineMetaClass<GMetaByteArray> gbyteArrayDefine = GDefineMetaClass<GMetaByteArray>::Policy<GMetaPolicyNoCopyConstructor>::declare("GMetaByteArray");
	buildMetaData_metaByteArray(gbyteArrayDefine);
	ns._class(gbyteArrayDefine);
	
	GScopedInterface<IMetaClass> metaClass(static_cast<IMetaClass *>(metaItemToInterface(ns.takeMetaClass(), true)));
	scriptObject->holdObject(metaClass.get());
	injectObjectToScript(scriptObject, metaClass.get(), nullptr, namespaces);
	
	return true;
}

bool loadObjectArray(GScriptObject * scriptObject, const char * namespaces, const char * /*libraryName*/)
{
	GDefineMetaNamespace ns = GDefineMetaNamespace::declare("");
	
	ns._method("createObjectArray", &createObjectArray);

	GDefineMetaClass<GMetaObjectArray> gobjectArrayDefine = GDefineMetaClass<GMetaObjectArray>::Policy<GMetaPolicyNoDefaultAndCopyConstructor>::declare("GMetaObjectArray");
	buildMetaData_metaObjectArray(gobjectArrayDefine);
	ns._class(gobjectArrayDefine);
	
	GScopedInterface<IMetaClass> metaClass(static_cast<IMetaClass *>(metaItemToInterface(ns.takeMetaClass(), true)));
	scriptObject->holdObject(metaClass.get());
	injectObjectToScript(scriptObject, metaClass.get(), nullptr, namespaces);
	
	return true;
}

bool loadDebug(GScriptObject * scriptObject, const char * namespaces, const char * /*libraryName*/)
{
	GDefineMetaClass<GMetaDebug> debugDefine = GDefineMetaClass<GMetaDebug>::declare("GMetaDebug");
	buildMetaData_metaDebug(debugDefine);
	
	GScopedInterface<IMetaClass> metaClass(static_cast<IMetaClass *>(metaItemToInterface(debugDefine.takeMetaClass(), true)));
	scriptObject->holdObject(metaClass.get());
	injectObjectToScript(scriptObject, metaClass.get(), nullptr, namespaces);
	
	return true;
}

void initializeBuiltinLibraries(GScriptLibraryNamedLoaderHandler * libraryHandler)
{
	libraryHandler->addHandler("builtin.core", GScriptLibraryLoaderCallback(&loadCore));
	libraryHandler->addHandler("builtin.collections.bytearray", GScriptLibraryLoaderCallback(&loadByteArray));
	libraryHandler->addHandler("builtin.collections.objectarray", GScriptLibraryLoaderCallback(&loadObjectArray));
	libraryHandler->addHandler("builtin.debug", GScriptLibraryLoaderCallback(&loadDebug));
}

IScriptLibraryLoader * createBuiltinLibraries(GScriptObject * scriptObject)
{
	GScriptLibraryNamedLoaderHandler * namedLoader = new GScriptLibraryNamedLoaderHandler(scriptObject);
	GScopedInterface<IScriptLibraryLoaderHandler> namedLoaderInterface(namedLoader);
	initializeBuiltinLibraries(namedLoader);
	GScriptLibraryLoader * loader = new GScriptLibraryLoader();
	loader->addHandler(namedLoaderInterface.get());
	return loader;
}


} // namespace cpgf

