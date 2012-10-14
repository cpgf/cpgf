#include "cpgf/scriptbind/gscriptservice.h"
#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/util/gmetadata_bytearray.h"
#include "cpgf/gbytearray.h"
#include "cpgf/metadata/util/gmetadata_metaobjectarray.h"
#include "cpgf/metautility/gmetaobjectarray.h"

#include "cpgf/metadata/private/gmetadata_header.h"

#include "cpgf/metatraits/gmetasharedptrtraits_gsharedpointer.h"


namespace cpgf {


namespace {

GSharedPointer<GByteArray> createByteArray()
{
	return GSharedPointer<GByteArray>(new GByteArray);
}

GSharedPointer<GByteArray> createByteArrayWithLength(size_t length)
{
	return GSharedPointer<GByteArray>(new GByteArray(length));
}

GSharedPointer<GMetaObjectArray> createObjectArray(IMetaClass * metaClass)
{
	return GSharedPointer<GMetaObjectArray>(new GMetaObjectArray(metaClass));
}

} // unnamed namespace

template <typename D>
void buildMetaClass_GScriptCoreService(D _d)
{
    _d.CPGF_MD_TEMPLATE _method("cloneClass", &D::ClassType::cloneClass);

	_d.CPGF_MD_TEMPLATE _method("createByteArray", &createByteArray);
	_d.CPGF_MD_TEMPLATE _method("createByteArray", &createByteArrayWithLength);
	_d.CPGF_MD_TEMPLATE _method("createObjectArray", &createObjectArray);

	GDefineMetaClass<GByteArray> gbyteArrayDefine = GDefineMetaClass<GByteArray>::Policy<GMetaPolicyNoCopyConstructor>::declare("GByteArray");
	buildMetaData_byteArray(gbyteArrayDefine);
	_d.CPGF_MD_TEMPLATE _class(gbyteArrayDefine);

	GDefineMetaClass<GMetaObjectArray> gobjectArrayDefine = GDefineMetaClass<GMetaObjectArray>::Policy<GMetaPolicyNoDefaultAndCopyConstructor>::declare("GMetaObjectArray");
	buildMetaData_metaObjectArray(gobjectArrayDefine);
	_d.CPGF_MD_TEMPLATE _class(gobjectArrayDefine);
}

GMetaClass * doBindScriptCoreService(GScriptObject * scriptObject, const char * bindName, GScriptCoreService * scriptCoreService)
{
	GDefineMetaClass<GScriptCoreService> define = GDefineMetaClass<GScriptCoreService>::Policy<GMetaPolicyNoDefaultAndCopyConstructor>::declare("GScriptCoreService");
	buildMetaClass_GScriptCoreService(define);

	injectObjectToScript(scriptObject, define.getMetaClass(), scriptCoreService, bindName);

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
	return this->scriptObject->cloneMetaClass(metaClass);
}



} // namespace cpgf


#include "cpgf/metadata/private/gmetadata_footer.h"
