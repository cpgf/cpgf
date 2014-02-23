/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_CppInvokable
@@cpgf@creations@*/

#ifndef METADATA_META_CPPINVOKABLE_H
#define METADATA_META_CPPINVOKABLE_H

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif
#include "clang/AST/Decl.h"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include "model/cpppolicy.h"
#include "model/cppinvokable.h"
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
metagen::CppInvokable &oPeRat0rWrapPer_metagen_CppInvokable_opAssign_0(TsE1f * sE1F, const metagen::CppInvokable &pAr9_Am0)
{
    return (*sE1F) = pAr9_Am0;
}

template <typename D_d >
void buildMetaClass_metagen_CppInvokable(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _base<metagen::CppNamedItem >();

    _d.CPGF_MD_TEMPLATE _method("isInvokable", &D_d::ClassType::isInvokable);
    _d.CPGF_MD_TEMPLATE _method("isStatic", &D_d::ClassType::isStatic);
    _d.CPGF_MD_TEMPLATE _method("isConst", &D_d::ClassType::isConst);
    _d.CPGF_MD_TEMPLATE _method("isVirtual", &D_d::ClassType::isVirtual);
    _d.CPGF_MD_TEMPLATE _method("isPureVirtual", &D_d::ClassType::isPureVirtual);
    _d.CPGF_MD_TEMPLATE _method("isVariadic", &D_d::ClassType::isVariadic);
    _d.CPGF_MD_TEMPLATE _method("isTemplate", &D_d::ClassType::isTemplate);
    _d.CPGF_MD_TEMPLATE _method("isOverloaded", &D_d::ClassType::isOverloaded);
    _d.CPGF_MD_TEMPLATE _method("hasResult", &D_d::ClassType::hasResult);
    _d.CPGF_MD_TEMPLATE _method("hasTemplateDependentParam", &D_d::ClassType::hasTemplateDependentParam);
    _d.CPGF_MD_TEMPLATE _method("getArity", &D_d::ClassType::getArity);
    _d.CPGF_MD_TEMPLATE _method("getNonDefaultParamCount", &D_d::ClassType::getNonDefaultParamCount);
    _d.CPGF_MD_TEMPLATE _method("getParamType", &D_d::ClassType::getParamType);
    _d.CPGF_MD_TEMPLATE _method("paramHasDefaultValue", &D_d::ClassType::paramHasDefaultValue);
    _d.CPGF_MD_TEMPLATE _method("getTextOfParamDeafultValue", &D_d::ClassType::getTextOfParamDeafultValue);
    _d.CPGF_MD_TEMPLATE _method("getParamName", &D_d::ClassType::getParamName);
    _d.CPGF_MD_TEMPLATE _method("getTextOfUnusedParamsPlaceholder", &D_d::ClassType::getTextOfUnusedParamsPlaceholder);
    _d.CPGF_MD_TEMPLATE _method("getTextOfPointeredType", &D_d::ClassType::getTextOfPointeredType);
    _d.CPGF_MD_TEMPLATE _method("getTextOfParamList", &D_d::ClassType::getTextOfParamList), cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >();
    _d.CPGF_MD_TEMPLATE _method("getText", &D_d::ClassType::getText), cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >();
    _d.CPGF_MD_TEMPLATE _method("getTextWithReplacedName", &D_d::ClassType::getTextWithReplacedName), cpgf::MakePolicy<GMetaRuleCopyConstReference<0>, GMetaRuleCopyConstReference<1> >();
    _d.CPGF_MD_TEMPLATE _method("getFunctionType", &D_d::ClassType::getFunctionType);
    _d.CPGF_MD_TEMPLATE _method("getResultType", &D_d::ClassType::getResultType);
    _d.CPGF_MD_TEMPLATE _method("getPolicy", &D_d::ClassType::getPolicy);

    _d.CPGF_MD_TEMPLATE _operator<metagen::CppInvokable &(*)(cpgf::GMetaSelf, const metagen::CppInvokable &)>(mopHolder = mopHolder, cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_CppInvokable_opAssign_0<typename D_d::ClassType >);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_CppInvokable();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
