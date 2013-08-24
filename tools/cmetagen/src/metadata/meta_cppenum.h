/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_CppEnumValue
createMetaClass_metagen_CppEnum
@@cpgf@creations@*/

#ifndef METADATA_META_CPPENUM_H
#define METADATA_META_CPPENUM_H

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif
#include "clang/AST/Decl.h"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include "model/cppenum.h"
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
metagen::CppEnum &oPeRat0rWrapPer_metagen_CppEnum_opAssign_0(TsE1f * sE1F, const metagen::CppEnum &pAr9_Am0)
{
    return (*sE1F) = pAr9_Am0;
}

template <typename D_d >
void buildMetaClass_metagen_CppEnumValue(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    

    _d.CPGF_MD_TEMPLATE _constructor<void * (const std::basic_string<char, std::char_traits<char>, std::allocator<char> > &, const std::basic_string<char, std::char_traits<char>, std::allocator<char> > &)>(cpgf::MakePolicy<GMetaRuleCopyConstReference<0>, GMetaRuleCopyConstReference<1> >());

    _d.CPGF_MD_TEMPLATE _method("getName", &D_d::ClassType::getName), cpgf::MakePolicy<GMetaRuleCopyConstReference<-1> >();
    _d.CPGF_MD_TEMPLATE _method("getQualifiedName", &D_d::ClassType::getQualifiedName), cpgf::MakePolicy<GMetaRuleCopyConstReference<-1> >();

}

template <typename D_d >
void buildMetaClass_metagen_CppEnum(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _base<metagen::CppNamedItem >();

    _d.CPGF_MD_TEMPLATE _constructor<void * (const clang::Decl *)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const metagen::CppEnum &)>(cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());

    _d.CPGF_MD_TEMPLATE _method("getValueList", &D_d::ClassType::getValueList);
    _d.CPGF_MD_TEMPLATE _method("isAnonymous", &D_d::ClassType::isAnonymous);

    _d.CPGF_MD_TEMPLATE _operator<metagen::CppEnum &(*)(cpgf::GMetaSelf, const metagen::CppEnum &)>(mopHolder = mopHolder, cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_CppEnum_opAssign_0<typename D_d::ClassType >);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_CppEnumValue();
cpgf::GDefineMetaInfo createMetaClass_metagen_CppEnum();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
