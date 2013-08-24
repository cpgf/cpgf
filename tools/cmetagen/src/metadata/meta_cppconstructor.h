/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_CppConstructor
@@cpgf@creations@*/

#ifndef METADATA_META_CPPCONSTRUCTOR_H
#define METADATA_META_CPPCONSTRUCTOR_H

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif
#include "clang/AST/Decl.h"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include "model/cppconstructor.h"
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
metagen::CppConstructor &oPeRat0rWrapPer_metagen_CppConstructor_opAssign_0(TsE1f * sE1F, const metagen::CppConstructor &pAr9_Am0)
{
    return (*sE1F) = pAr9_Am0;
}

template <typename D_d >
void buildMetaClass_metagen_CppConstructor(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _base<metagen::CppInvokable >();

    _d.CPGF_MD_TEMPLATE _constructor<void * (const clang::Decl *)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const metagen::CppConstructor &)>(cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());

    _d.CPGF_MD_TEMPLATE _method("isDefaultConstructor", &D_d::ClassType::isDefaultConstructor);
    _d.CPGF_MD_TEMPLATE _method("isCopyConstructor", &D_d::ClassType::isCopyConstructor);
    _d.CPGF_MD_TEMPLATE _method("isImplicitTypeConverter", &D_d::ClassType::isImplicitTypeConverter);

    _d.CPGF_MD_TEMPLATE _operator<metagen::CppConstructor &(*)(cpgf::GMetaSelf, const metagen::CppConstructor &)>(mopHolder = mopHolder, cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_CppConstructor_opAssign_0<typename D_d::ClassType >);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_CppConstructor();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
