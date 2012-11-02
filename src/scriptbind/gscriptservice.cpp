#include "cpgf/scriptbind/gscriptservice.h"
#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptbindutil.h"

#include "cpgf/metadata/metautility/gmetadata_metabytearray.h"
#include "cpgf/metadata/metautility/gmetadata_metaobjectarray.h"
#include "cpgf/metautility/gmetabytearray.h"
#include "cpgf/metautility/gmetaobjectarray.h"

#include "cpgf/scriptbind/gscriptlibraryapi.h"

#include "cpgf/metadata/private/gmetadata_header.h"


namespace cpgf {


IScriptLibraryLoader * createBuiltinLibraries();

template <typename D>
void buildMetaClass_GScriptCoreService(D _d)
{
    _d.CPGF_MD_TEMPLATE _method("cloneClass", &D::ClassType::cloneClass);
    _d.CPGF_MD_TEMPLATE _method("loadLibrary", &D::ClassType::loadLibrary);
}

GScriptCoreService * doBindScriptCoreService(GScriptObject * scriptObject, const char * bindName, IScriptLibraryLoader * libraryLoader)
{
	GScopedPointer<GScriptCoreService> coreService(new GScriptCoreService(scriptObject, bindName, libraryLoader));

	GDefineMetaClass<GScriptCoreService> define = GDefineMetaClass<GScriptCoreService>::Policy<GMetaPolicyNoDefaultAndCopyConstructor>::declare("GScriptCoreService");
	buildMetaClass_GScriptCoreService(define);

	injectObjectToScript(scriptObject, define.getMetaClass(), coreService.get(), bindName);

	GScopedInterface<IMetaItem> metaItem(metaItemToInterface(define.takeMetaClass(), true));
	scriptObject->holdObject(metaItem.get());

	return coreService.take();
}


GScriptCoreService::GScriptCoreService(GScriptObject * scriptObject, const char * bindName, IScriptLibraryLoader * libraryLoader)
	: scriptObject(scriptObject), bindName(bindName), libraryLoader(libraryLoader)
{
}

GScriptCoreService::~GScriptCoreService()
{
}

IMetaClass * GScriptCoreService::cloneClass(IMetaClass * metaClass)
{
	return this->scriptObject->cloneMetaClass(metaClass);
}

bool GScriptCoreService::loadLibrary(const char * namespaces, const GMetaVariadicParam * libraryNames)
{
	if(! this->libraryLoader) {
		this->libraryLoader.reset(createBuiltinLibraries());
		this->libraryLoader->releaseReference();
	}

	if(namespaces == NULL) {
		namespaces = this->bindName.c_str();
	}

	GScopedInterface<IScriptObject> owner(scriptObjectToInterface(this->scriptObject, false));
	for(size_t i = 0; i < libraryNames->paramCount; ++i) {
		char * name = fromVariant<char *>(*(libraryNames->params[i]));
		if(! this->libraryLoader->loadScriptLibrary(owner.get(), namespaces, name)) {
			return false;
		}
	}
	
	return true;
}


} // namespace cpgf


#include "cpgf/metadata/private/gmetadata_footer.h"
