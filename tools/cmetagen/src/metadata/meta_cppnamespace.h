/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_CppNamespace
@@cpgf@creations@*/

#ifndef METADATA_META_CPPNAMESPACE_H
#define METADATA_META_CPPNAMESPACE_H

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif
#include "clang/AST/Decl.h"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include "model/cppcontainer.h"
#include "model/cppnamespace.h"
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
metagen::CppNamespace &oPeRat0rWrapPer_metagen_CppNamespace_opAssign_0(TsE1f * sE1F, const metagen::CppNamespace &pAr9_Am0)
{
    return (*sE1F) = pAr9_Am0;
}

template <typename D_d >
void buildMetaClass_metagen_CppNamespace(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _base<metagen::CppContainer >();

    _d.CPGF_MD_TEMPLATE _constructor<void * (clang::Decl *)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const metagen::CppNamespace &)>(cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());

    _d.CPGF_MD_TEMPLATE _method("isAnonymous", &D_d::ClassType::isAnonymous);

    _d.CPGF_MD_TEMPLATE _operator<metagen::CppNamespace &(*)(cpgf::GMetaSelf, const metagen::CppNamespace &)>(mopHolder = mopHolder, cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_CppNamespace_opAssign_0<typename D_d::ClassType >);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_CppNamespace();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
