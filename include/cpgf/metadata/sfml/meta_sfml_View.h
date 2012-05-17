// Auto generated file, don't modify.

#ifndef __META_SFML_VIEW_H
#define __META_SFML_VIEW_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindapi.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_View(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (const FloatRect &)>()
        ._default(copyVariantFromCopyable(FloatRect(0, 0, 1000, 1000)))
    ;
    _d.CPGF_MD_TEMPLATE _constructor<void * (const sf::Vector2f &, const sf::Vector2f &)>();
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetCenter", _r), (void (D::ClassType::*) (float, float))&D::ClassType::SetCenter);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetCenter", _r), (void (D::ClassType::*) (const sf::Vector2f &))&D::ClassType::SetCenter);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetHalfSize", _r), (void (D::ClassType::*) (float, float))&D::ClassType::SetHalfSize);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetHalfSize", _r), (void (D::ClassType::*) (const sf::Vector2f &))&D::ClassType::SetHalfSize);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetFromRect", _r), &D::ClassType::SetFromRect);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetCenter", _r), &D::ClassType::GetCenter);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetHalfSize", _r), &D::ClassType::GetHalfSize);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetRect", _r), &D::ClassType::GetRect);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Move", _r), (void (D::ClassType::*) (float, float))&D::ClassType::Move);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Move", _r), (void (D::ClassType::*) (const sf::Vector2f &))&D::ClassType::Move);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Zoom", _r), &D::ClassType::Zoom);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
