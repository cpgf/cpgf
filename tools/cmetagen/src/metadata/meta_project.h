/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_Project
createMetaClass_metagen_ProjectVisitor
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

template <typename TsE1f>
metagen::ProjectVisitor &oPeRat0rWrapPer_metagen_ProjectVisitor_opAssign_0(TsE1f * sE1F, const metagen::ProjectVisitor &pAr9_Am0)
{
    return (*sE1F) = pAr9_Am0;
}

template <typename D_d >
void buildMetaClass_metagen_Project(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    

    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();

    _d.CPGF_MD_TEMPLATE _field("projectID", &typename D_d::ClassType::projectID);
    _d.CPGF_MD_TEMPLATE _field("sourceRootPath", &typename D_d::ClassType::sourceRootPath);
    _d.CPGF_MD_TEMPLATE _field("files", &typename D_d::ClassType::files);
    _d.CPGF_MD_TEMPLATE _field("includeDirectories", &typename D_d::ClassType::includeDirectories);
    _d.CPGF_MD_TEMPLATE _field("cppNamespace", &typename D_d::ClassType::cppNamespace);
    _d.CPGF_MD_TEMPLATE _field("maxItemCountPerFile", &typename D_d::ClassType::maxItemCountPerFile);
    _d.CPGF_MD_TEMPLATE _field("headerIncludePrefix", &typename D_d::ClassType::headerIncludePrefix);
    _d.CPGF_MD_TEMPLATE _field("headerFileExtension", &typename D_d::ClassType::headerFileExtension);
    _d.CPGF_MD_TEMPLATE _field("sourceFileExtension", &typename D_d::ClassType::sourceFileExtension);
    _d.CPGF_MD_TEMPLATE _field("headerOutputPath", &typename D_d::ClassType::headerOutputPath);
    _d.CPGF_MD_TEMPLATE _field("sourceOutputPath", &typename D_d::ClassType::sourceOutputPath);
    _d.CPGF_MD_TEMPLATE _field("targetFilePrefix", &typename D_d::ClassType::targetFilePrefix);
    _d.CPGF_MD_TEMPLATE _field("includeExtensionInFileName", &typename D_d::ClassType::includeExtensionInFileName);
    _d.CPGF_MD_TEMPLATE _field("reflectionFunctionPrefix", &typename D_d::ClassType::reflectionFunctionPrefix);
    _d.CPGF_MD_TEMPLATE _field("creationFunctionPrefix", &typename D_d::ClassType::creationFunctionPrefix);
    _d.CPGF_MD_TEMPLATE _field("metaDefineParamName", &typename D_d::ClassType::metaDefineParamName);
    _d.CPGF_MD_TEMPLATE _field("classWrapperPostfix", &typename D_d::ClassType::classWrapperPostfix);
    _d.CPGF_MD_TEMPLATE _field("classWrapperSuperPrefix", &typename D_d::ClassType::classWrapperSuperPrefix);
    _d.CPGF_MD_TEMPLATE _field("mainRegisterFunctionName", &typename D_d::ClassType::mainRegisterFunctionName);
    _d.CPGF_MD_TEMPLATE _field("mainRegisterFileName", &typename D_d::ClassType::mainRegisterFileName);
    _d.CPGF_MD_TEMPLATE _field("autoRegisterToGlobal", &typename D_d::ClassType::autoRegisterToGlobal);
    _d.CPGF_MD_TEMPLATE _field("metaNamespace", &typename D_d::ClassType::metaNamespace);
    _d.CPGF_MD_TEMPLATE _field("wrapOperator", &typename D_d::ClassType::wrapOperator);
    _d.CPGF_MD_TEMPLATE _field("wrapBitFields", &typename D_d::ClassType::wrapBitFields);
    _d.CPGF_MD_TEMPLATE _field("allowPublic", &typename D_d::ClassType::allowPublic);
    _d.CPGF_MD_TEMPLATE _field("allowProtected", &typename D_d::ClassType::allowProtected);
    _d.CPGF_MD_TEMPLATE _field("allowPrivate", &typename D_d::ClassType::allowPrivate);
    _d.CPGF_MD_TEMPLATE _field("force", &typename D_d::ClassType::force);
    _d.CPGF_MD_TEMPLATE _field("stopOnCompileError", &typename D_d::ClassType::stopOnCompileError);
    _d.CPGF_MD_TEMPLATE _field("fileCallback", &typename D_d::ClassType::fileCallback);
    _d.CPGF_MD_TEMPLATE _field("mainCallback", &typename D_d::ClassType::mainCallback);
    _d.CPGF_MD_TEMPLATE _field("headerReplaceCallback", &typename D_d::ClassType::headerReplaceCallback);
    _d.CPGF_MD_TEMPLATE _field("templateInstantiations", &typename D_d::ClassType::templateInstantiations);

}

template <typename D_d >
void buildMetaClass_metagen_ProjectVisitor(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    

    _d.CPGF_MD_TEMPLATE _method("visitStringArray", &typename D_d::ClassType::visitStringArray), cpgf::MakePolicy<GMetaRuleCopyConstReference<1> >();
    _d.CPGF_MD_TEMPLATE _method("visitTemplateInstantiations", &typename D_d::ClassType::visitTemplateInstantiations), cpgf::MakePolicy<GMetaRuleCopyConstReference<1> >();
    _d.CPGF_MD_TEMPLATE _method("visitScriptCallback", &typename D_d::ClassType::visitScriptCallback), cpgf::MakePolicy<GMetaRuleCopyConstReference<1> >();
    _d.CPGF_MD_TEMPLATE _method("visitString", &typename D_d::ClassType::visitString), cpgf::MakePolicy<GMetaRuleCopyConstReference<1> >();
    _d.CPGF_MD_TEMPLATE _method("visitInteger", &typename D_d::ClassType::visitInteger), cpgf::MakePolicy<GMetaRuleCopyConstReference<1> >();
    _d.CPGF_MD_TEMPLATE _method("getProject", &typename D_d::ClassType::getProject);
    _d.CPGF_MD_TEMPLATE _method("getScriptObject", &typename D_d::ClassType::getScriptObject);
    _d.CPGF_MD_TEMPLATE _method("setScriptObject", &typename D_d::ClassType::setScriptObject);

    _d.CPGF_MD_TEMPLATE _operator<metagen::ProjectVisitor &(*)(cpgf::GMetaSelf, const metagen::ProjectVisitor &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_ProjectVisitor_opAssign_0<typename D_d::ClassType >);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_Project();
cpgf::GDefineMetaInfo createMetaClass_metagen_ProjectVisitor();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
