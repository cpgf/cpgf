#include "cpgf/scriptbind/gscriptservice.h"
#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetadefine.h"


namespace cpgf {


template <typename D>
void buildMetaClass_GScriptCoreService(D _d)
{
    _d.CPGF_MD_TEMPLATE _method("cloneClass", &D::ClassType::cloneClass);
}

GMetaClass * doBindScriptCoreService(GScriptObject * scriptObject, const char * bindName, GScriptCoreService * scriptCoreService)
{
	GDefineMetaClass<GScriptCoreService> define = GDefineMetaClass<GScriptCoreService>::Policy<GMetaPolicyNoDefaultAndCopyConstructor>::declare("GScriptCoreService");
	buildMetaClass_GScriptCoreService(define);

	GScopedInterface<IScriptObject> scriptObjectInterface( scriptObjectToInterface(scriptObject, false));
	GScopedInterface<IMetaItem> metaItem(metaItemToInterface(define.getMetaClass()));
	IMetaClass * metaClassInterface = static_cast<IMetaClass *>(metaItem.get());
	
	if(bindName == NULL) {
		injectObjectToScript(scriptObjectInterface.get(), metaClassInterface, scriptCoreService);
	}
	else {
		scriptObject->bindObject(bindName, scriptCoreService, metaClassInterface, false);
	}

	return define.takeMetaClass();
}

GScriptCoreService * doCreateScriptCoreService(GScriptObject * scriptObject)
{
	return new GScriptCoreService(scriptObject);
}


GScriptCoreService::GScriptCoreService(GScriptObject * scriptObject)
	: scriptObject(scriptObject)
{
}

GScriptCoreService::~GScriptCoreService()
{
}

IMetaClass * GScriptCoreService::cloneClass(IMetaClass * metaClass)
{
	this->previousClonedMetaClass.reset(this->scriptObject->cloneMetaClass(metaClass));
	return this->previousClonedMetaClass.get();
}



} // namespace cpgf


#include "cpgf/metadata/private/gmetadata_footer.h"
