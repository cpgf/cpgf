/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_CppField
@@cpgf@creations@*/

#ifndef METADATA_META_CPPFIELD_H
#define METADATA_META_CPPFIELD_H

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif
#include "clang/AST/Decl.h"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include "model/cppcontainer.h"
#include "model/cppfield.h"
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
metagen::CppField &oPeRat0rWrapPer_metagen_CppField_opAssign_0(TsE1f * sE1F, const metagen::CppField &pAr9_Am0)
{
    return (*sE1F) = pAr9_Am0;
}

template <typename D_d >
void buildMetaClass_metagen_CppField(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _base<metagen::CppNamedItem >();

    _d.CPGF_MD_TEMPLATE _constructor<void * (const clang::Decl *)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const metagen::CppField &)>(cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());

    _d.CPGF_MD_TEMPLATE _method("isStatic", &D_d::ClassType::isStatic);
    _d.CPGF_MD_TEMPLATE _method("isBitField", &D_d::ClassType::isBitField);
    _d.CPGF_MD_TEMPLATE _method("isAnonymousStructOrUnion", &D_d::ClassType::isAnonymousStructOrUnion);
    _d.CPGF_MD_TEMPLATE _method("getType", &D_d::ClassType::getType);

    _d.CPGF_MD_TEMPLATE _operator<metagen::CppField &(*)(cpgf::GMetaSelf, const metagen::CppField &)>(mopHolder = mopHolder, cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_CppField_opAssign_0<typename D_d::ClassType >);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_CppField();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
