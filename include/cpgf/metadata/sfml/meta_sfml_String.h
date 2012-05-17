// Auto generated file, don't modify.

#ifndef __META_SFML_STRING_H
#define __META_SFML_STRING_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindapi.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_String(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const Unicode::Text &, const Font &, float)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(30.f))
        ._default(copyVariantFromCopyable(Font::GetDefaultFont()))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetText", _r), &D::ClassType::SetText, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetFont", _r), &D::ClassType::SetFont);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetSize", _r), &D::ClassType::SetSize);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetStyle", _r), &D::ClassType::SetStyle);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetText", _r), &D::ClassType::GetText, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetFont", _r), &D::ClassType::GetFont);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSize", _r), &D::ClassType::GetSize);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetStyle", _r), &D::ClassType::GetStyle);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetCharacterPos", _r), &D::ClassType::GetCharacterPos);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetRect", _r), &D::ClassType::GetRect);
    _d.CPGF_MD_TEMPLATE _enum<typename D::ClassType::Style>(replaceName("Style", _r))
        ._element(replaceName("Regular", _r), D::ClassType::Regular)
        ._element(replaceName("Bold", _r), D::ClassType::Bold)
        ._element(replaceName("Italic", _r), D::ClassType::Italic)
        ._element(replaceName("Underlined", _r), D::ClassType::Underlined)
    ;
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
