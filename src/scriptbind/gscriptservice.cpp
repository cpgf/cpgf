#include "cpgf/scriptbind/gscriptservice.h"
#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/util/gmetadata_bytearray.h"
#include "cpgf/gbytearray.h"
#include "cpgf/gbytearrayapi.h"

#include "cpgf/metadata/private/gmetadata_header.h"


namespace cpgf {


namespace {

IByteArray * createByteArray()
{
	return byteArrayToInterface(new GByteArray, true);
}

IByteArray * createByteArrayWithLength(size_t length)
{
	return byteArrayToInterface(new GByteArray(length), true);
}

} // unnamed namespace

template <typename D>
void buildMetaClass_GScriptCoreService(D _d)
{
    _d.CPGF_MD_TEMPLATE _method("cloneClass", &D::ClassType::cloneClass);

	_d.CPGF_MD_TEMPLATE _method("createByteArray", &createByteArray);
	_d.CPGF_MD_TEMPLATE _method("createByteArray", &createByteArrayWithLength);

	GDefineMetaClass<IByteArray> byteArrayDefine = GDefineMetaClass<IByteArray>::declare("IByteArray");
	buildMetaData_byteArray(byteArrayDefine);
	_d.CPGF_MD_TEMPLATE _class(byteArrayDefine);
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
	this->previousClonedMetaClass.reset(this->scriptObject->cloneMetaClass(metaClass));
	return this->previousClonedMetaClass.get();
}



} // namespace cpgf


#include "cpgf/metadata/private/gmetadata_footer.h"
