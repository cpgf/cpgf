// Auto generated file, don't modify.

#ifndef __META_SFML_COLOR_H
#define __META_SFML_COLOR_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindapi.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Global_color(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _operator< Color (*)(const Color &, const Color &)>(mopHolder + mopHolder);
    _d.CPGF_MD_TEMPLATE _operator< Color (*)(const Color &, const Color &)>(mopHolder * mopHolder);
}


template <typename D>
void buildMetaClass_Color(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (Uint8, Uint8, Uint8, Uint8)>()
        ._default(copyVariantFromCopyable(255))
    ;
    _d.CPGF_MD_TEMPLATE _field(replaceName("Black", _r), &D::ClassType::Black);
    _d.CPGF_MD_TEMPLATE _field(replaceName("White", _r), &D::ClassType::White);
    _d.CPGF_MD_TEMPLATE _field(replaceName("Red", _r), &D::ClassType::Red);
    _d.CPGF_MD_TEMPLATE _field(replaceName("Green", _r), &D::ClassType::Green);
    _d.CPGF_MD_TEMPLATE _field(replaceName("Blue", _r), &D::ClassType::Blue);
    _d.CPGF_MD_TEMPLATE _field(replaceName("Yellow", _r), &D::ClassType::Yellow);
    _d.CPGF_MD_TEMPLATE _field(replaceName("Magenta", _r), &D::ClassType::Magenta);
    _d.CPGF_MD_TEMPLATE _field(replaceName("Cyan", _r), &D::ClassType::Cyan);
    _d.CPGF_MD_TEMPLATE _field(replaceName("r", _r), &D::ClassType::r);
    _d.CPGF_MD_TEMPLATE _field(replaceName("g", _r), &D::ClassType::g);
    _d.CPGF_MD_TEMPLATE _field(replaceName("b", _r), &D::ClassType::b);
    _d.CPGF_MD_TEMPLATE _field(replaceName("a", _r), &D::ClassType::a);
    _d.CPGF_MD_TEMPLATE _operator<Color & (*)(cpgf::GMetaSelf, const Color &)>(mopHolder += mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<Color & (*)(cpgf::GMetaSelf, const Color &)>(mopHolder *= mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const Color &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const Color &)>(mopHolder != mopHolder);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
