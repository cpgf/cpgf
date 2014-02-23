/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_CppOperator
@@cpgf@creations@*/

#ifndef METADATA_META_CPPOPERATOR_H
#define METADATA_META_CPPOPERATOR_H

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif
#include "clang/AST/Decl.h"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include "model/cppoperator.h"
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
metagen::CppOperator &oPeRat0rWrapPer_metagen_CppOperator_opAssign_0(TsE1f * sE1F, const metagen::CppOperator &pAr9_Am0)
{
    return (*sE1F) = pAr9_Am0;
}

template <typename D_d >
void buildMetaClass_metagen_CppOperator(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _base<metagen::CppInvokable >();

    _d.CPGF_MD_TEMPLATE _constructor<void * (const clang::Decl *)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const metagen::CppOperator &)>(cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());

    _d.CPGF_MD_TEMPLATE _method("getOperatorName", &D_d::ClassType::getOperatorName), cpgf::MakePolicy<GMetaRuleCopyConstReference<-1> >();
    _d.CPGF_MD_TEMPLATE _method("isArray", &D_d::ClassType::isArray);
    _d.CPGF_MD_TEMPLATE _method("isFunctor", &D_d::ClassType::isFunctor);
    _d.CPGF_MD_TEMPLATE _method("isTypeConverter", &D_d::ClassType::isTypeConverter);

    _d.CPGF_MD_TEMPLATE _operator<metagen::CppOperator &(*)(cpgf::GMetaSelf, const metagen::CppOperator &)>(mopHolder = mopHolder, cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_CppOperator_opAssign_0<typename D_d::ClassType >);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_CppOperator();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
