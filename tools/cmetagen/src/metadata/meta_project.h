/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_Project
createMetaClass_metagen_ProjectWrapper
@@cpgf@creations@*/

#ifndef METADATA_META_PROJECT_H
#define METADATA_META_PROJECT_H

#include "cpgf/gmetadefine.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/gscopedinterface.h"
#include "cpgf/gselectFunctionByArity.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/scriptbind/gscriptwrapper.h"
#include "project.h"

namespace metadata {

template <typename D_d >
void buildMetaClass_metagen_Project(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    

    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();

    _d.CPGF_MD_TEMPLATE _field("projectID", &D_d::ClassType::projectID);
    _d.CPGF_MD_TEMPLATE _field("sourceRootPath", &D_d::ClassType::sourceRootPath);
    _d.CPGF_MD_TEMPLATE _field("files", &D_d::ClassType::files);
    _d.CPGF_MD_TEMPLATE _field("cppNamespace", &D_d::ClassType::cppNamespace);
    _d.CPGF_MD_TEMPLATE _field("maxItemCountPerFile", &D_d::ClassType::maxItemCountPerFile);
    _d.CPGF_MD_TEMPLATE _field("headerIncludePrefix", &D_d::ClassType::headerIncludePrefix);
    _d.CPGF_MD_TEMPLATE _field("headerFileExtension", &D_d::ClassType::headerFileExtension);
    _d.CPGF_MD_TEMPLATE _field("sourceFileExtension", &D_d::ClassType::sourceFileExtension);
    _d.CPGF_MD_TEMPLATE _field("headerOutputPath", &D_d::ClassType::headerOutputPath);
    _d.CPGF_MD_TEMPLATE _field("sourceOutputPath", &D_d::ClassType::sourceOutputPath);
    _d.CPGF_MD_TEMPLATE _field("targetFilePrefix", &D_d::ClassType::targetFilePrefix);
    _d.CPGF_MD_TEMPLATE _field("includeExtensionInFileName", &D_d::ClassType::includeExtensionInFileName);
    _d.CPGF_MD_TEMPLATE _field("reflectionFunctionPrefix", &D_d::ClassType::reflectionFunctionPrefix);
    _d.CPGF_MD_TEMPLATE _field("creationFunctionPrefix", &D_d::ClassType::creationFunctionPrefix);
    _d.CPGF_MD_TEMPLATE _field("metaDefineParamName", &D_d::ClassType::metaDefineParamName);
    _d.CPGF_MD_TEMPLATE _field("classWrapperPostfix", &D_d::ClassType::classWrapperPostfix);
    _d.CPGF_MD_TEMPLATE _field("classWrapperSuperPrefix", &D_d::ClassType::classWrapperSuperPrefix);
    _d.CPGF_MD_TEMPLATE _field("mainRegisterFunctionName", &D_d::ClassType::mainRegisterFunctionName);
    _d.CPGF_MD_TEMPLATE _field("mainRegisterFileName", &D_d::ClassType::mainRegisterFileName);
    _d.CPGF_MD_TEMPLATE _field("autoRegisterToGlobal", &D_d::ClassType::autoRegisterToGlobal);
    _d.CPGF_MD_TEMPLATE _field("metaNamespace", &D_d::ClassType::metaNamespace);
    _d.CPGF_MD_TEMPLATE _field("wrapOperator", &D_d::ClassType::wrapOperator);
    _d.CPGF_MD_TEMPLATE _field("wrapBitFields", &D_d::ClassType::wrapBitFields);
    _d.CPGF_MD_TEMPLATE _field("allowPublic", &D_d::ClassType::allowPublic);
    _d.CPGF_MD_TEMPLATE _field("allowProtected", &D_d::ClassType::allowProtected);
    _d.CPGF_MD_TEMPLATE _field("allowPrivate", &D_d::ClassType::allowPrivate);
    _d.CPGF_MD_TEMPLATE _field("force", &D_d::ClassType::force);
    _d.CPGF_MD_TEMPLATE _field("mainCallback", &D_d::ClassType::mainCallback);
    _d.CPGF_MD_TEMPLATE _field("templateInstantiations", &D_d::ClassType::templateInstantiations);

}

class ProjectWrapper : public metagen::Project, public cpgf::GScriptWrapper
{
private:
    typedef metagen::Project super;

public:
    ProjectWrapper()
        : super() {}
    

protected:

private:

};

template <typename D_d >
void buildMetaClass_metagen_ProjectWrapper(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();

}

cpgf::GDefineMetaInfo createMetaClass_metagen_Project();
cpgf::GDefineMetaInfo createMetaClass_metagen_ProjectWrapper();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
