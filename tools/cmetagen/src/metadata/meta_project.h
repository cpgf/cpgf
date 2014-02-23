/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_Project
createMetaClass_metagen_ProjectVisitor
@@cpgf@creations@*/

#ifndef METADATA_META_PROJECT_H
#define METADATA_META_PROJECT_H

#include "project.h"
#include "cpgf/metatraits/gmetaconverter_string.h"
#include "cpgf/metatraits/gmetaconverter_widestring.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/gscopedinterface.h"
#include "cpgf/gselectfunctionbyarity.h"
#include "cpgf/metadata/private/gmetadata_header.h"

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

    _d.CPGF_MD_TEMPLATE _field("projectID", &D_d::ClassType::projectID);
    _d.CPGF_MD_TEMPLATE _field("sourceRootPath", &D_d::ClassType::sourceRootPath);
    _d.CPGF_MD_TEMPLATE _field("files", &D_d::ClassType::files);
    _d.CPGF_MD_TEMPLATE _field("clangOptions", &D_d::ClassType::clangOptions);
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
    _d.CPGF_MD_TEMPLATE _field("stopOnCompileError", &D_d::ClassType::stopOnCompileError);
    _d.CPGF_MD_TEMPLATE _field("fileCallback", &D_d::ClassType::fileCallback);
    _d.CPGF_MD_TEMPLATE _field("mainCallback", &D_d::ClassType::mainCallback);
    _d.CPGF_MD_TEMPLATE _field("headerReplaceCallback", &D_d::ClassType::headerReplaceCallback);
    _d.CPGF_MD_TEMPLATE _field("templateInstantiations", &D_d::ClassType::templateInstantiations);

}

template <typename D_d >
void buildMetaClass_metagen_ProjectVisitor(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    

    _d.CPGF_MD_TEMPLATE _method("visitStringArray", &D_d::ClassType::visitStringArray), cpgf::MakePolicy<GMetaRuleCopyConstReference<1> >();
    _d.CPGF_MD_TEMPLATE _method("visitTemplateInstantiations", &D_d::ClassType::visitTemplateInstantiations), cpgf::MakePolicy<GMetaRuleCopyConstReference<1> >();
    _d.CPGF_MD_TEMPLATE _method("visitScriptCallback", &D_d::ClassType::visitScriptCallback), cpgf::MakePolicy<GMetaRuleCopyConstReference<1> >();
    _d.CPGF_MD_TEMPLATE _method("visitString", &D_d::ClassType::visitString), cpgf::MakePolicy<GMetaRuleCopyConstReference<1> >();
    _d.CPGF_MD_TEMPLATE _method("visitInteger", &D_d::ClassType::visitInteger), cpgf::MakePolicy<GMetaRuleCopyConstReference<1> >();
    _d.CPGF_MD_TEMPLATE _method("getProject", &D_d::ClassType::getProject);
    _d.CPGF_MD_TEMPLATE _method("getScriptObject", &D_d::ClassType::getScriptObject);
    _d.CPGF_MD_TEMPLATE _method("setScriptObject", &D_d::ClassType::setScriptObject);

    _d.CPGF_MD_TEMPLATE _operator<metagen::ProjectVisitor &(*)(cpgf::GMetaSelf, const metagen::ProjectVisitor &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_ProjectVisitor_opAssign_0<typename D_d::ClassType >);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_Project();
cpgf::GDefineMetaInfo createMetaClass_metagen_ProjectVisitor();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
