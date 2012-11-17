// Auto generated file, don't modify.

#ifndef __META_SFML_COLOR_H
#define __META_SFML_COLOR_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Global_color(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _operator< Color (*)(const Color &, const Color &)>(mopHolder + mopHolder);
    _d.CPGF_MD_TEMPLATE _operator< Color (*)(const Color &, const Color &)>(mopHolder * mopHolder);
}


inline Color & opErAToRWrapper_Color__opAddAssign(Color * self, const Color & Other) {
    return (*self) += Other;
}
inline Color & opErAToRWrapper_Color__opMulAssign(Color * self, const Color & Other) {
    return (*self) *= Other;
}
inline bool opErAToRWrapper_Color__opEqual(const Color * self, const Color & Other) {
    return (*self) == Other;
}
inline bool opErAToRWrapper_Color__opNotEqual(const Color * self, const Color & Other) {
    return (*self) != Other;
}


template <typename D>
void buildMetaClass_Color(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (Uint8, Uint8, Uint8, Uint8)>()
        ._default(copyVariantFromCopyable(255))
    ;
    //_d.CPGF_MD_TEMPLATE _field("Black", &D::ClassType::Black);
    //_d.CPGF_MD_TEMPLATE _field("White", &D::ClassType::White);
    //_d.CPGF_MD_TEMPLATE _field("Red", &D::ClassType::Red);
    //_d.CPGF_MD_TEMPLATE _field("Green", &D::ClassType::Green);
    //_d.CPGF_MD_TEMPLATE _field("Blue", &D::ClassType::Blue);
    //_d.CPGF_MD_TEMPLATE _field("Yellow", &D::ClassType::Yellow);
    //_d.CPGF_MD_TEMPLATE _field("Magenta", &D::ClassType::Magenta);
    //_d.CPGF_MD_TEMPLATE _field("Cyan", &D::ClassType::Cyan);
    _d.CPGF_MD_TEMPLATE _field("r", &D::ClassType::r);
    _d.CPGF_MD_TEMPLATE _field("g", &D::ClassType::g);
    _d.CPGF_MD_TEMPLATE _field("b", &D::ClassType::b);
    _d.CPGF_MD_TEMPLATE _field("a", &D::ClassType::a);
    _d.CPGF_MD_TEMPLATE _operator<Color & (*)(cpgf::GMetaSelf, const Color &)>(mopHolder += mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAddAssign", (Color & (*) (Color *, const Color &))&opErAToRWrapper_Color__opAddAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<Color & (*)(cpgf::GMetaSelf, const Color &)>(mopHolder *= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opMulAssign", (Color & (*) (Color *, const Color &))&opErAToRWrapper_Color__opMulAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const Color &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const Color *, const Color &))&opErAToRWrapper_Color__opEqual, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const Color &)>(mopHolder != mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const Color *, const Color &))&opErAToRWrapper_Color__opNotEqual, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
