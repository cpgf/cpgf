/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_CppMethod
@@cpgf@creations@*/

#ifndef METADATA_META_CPPMETHOD_H
#define METADATA_META_CPPMETHOD_H

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif
#include "clang/AST/Decl.h"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include "model/cppcontainer.h"
#include "model/cppmethod.h"
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
metagen::CppMethod &oPeRat0rWrapPer_metagen_CppMethod_opAssign_0(TsE1f * sE1F, const metagen::CppMethod &pAr9_Am0)
{
    return (*sE1F) = pAr9_Am0;
}

template <typename D_d >
void buildMetaClass_metagen_CppMethod(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _base<metagen::CppInvokable >();

    _d.CPGF_MD_TEMPLATE _constructor<void * (const clang::Decl *)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const metagen::CppMethod &)>(cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());

    _d.CPGF_MD_TEMPLATE _method("isArityUnique", &D_d::ClassType::isArityUnique);

    _d.CPGF_MD_TEMPLATE _operator<metagen::CppMethod &(*)(cpgf::GMetaSelf, const metagen::CppMethod &)>(mopHolder = mopHolder, cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_CppMethod_opAssign_0<typename D_d::ClassType >);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_CppMethod();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
