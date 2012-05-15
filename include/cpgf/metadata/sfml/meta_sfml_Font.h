// Auto generated file, don't modify.

#ifndef __META_SFML_FONT_H
#define __META_SFML_FONT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;
using namespace sf::priv;


namespace meta_sfml { 


extern Uint32 cpgf_meta_ourDefaultCharset[];


template <typename D>
void buildMetaClass_Font(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method(replaceName("LoadFromFile", _r), &D::ClassType::LoadFromFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<2> >())
        ._default(copyVariantFromCopyable(cpgf_meta_ourDefaultCharset))
        ._default(copyVariantFromCopyable(30))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("LoadFromMemory", _r), &D::ClassType::LoadFromMemory, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<3> >())
        ._default(copyVariantFromCopyable(cpgf_meta_ourDefaultCharset))
        ._default(copyVariantFromCopyable(30))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetCharacterSize", _r), &D::ClassType::GetCharacterSize);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetGlyph", _r), &D::ClassType::GetGlyph);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetImage", _r), &D::ClassType::GetImage);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetDefaultFont", _r), &D::ClassType::GetDefaultFont);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
