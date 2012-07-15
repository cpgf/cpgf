// Auto generated file, don't modify.

#ifndef __META_SFML_VIEW_H
#define __META_SFML_VIEW_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_View(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (const FloatRect &)>()
        ._default(copyVariantFromCopyable(FloatRect(0, 0, 1000, 1000)))
    ;
    _d.CPGF_MD_TEMPLATE _constructor<void * (const sf::Vector2f &, const sf::Vector2f &)>();
    _d.CPGF_MD_TEMPLATE _method("SetCenter", (void (D::ClassType::*) (float, float))&D::ClassType::SetCenter);
    _d.CPGF_MD_TEMPLATE _method("SetCenter", (void (D::ClassType::*) (const sf::Vector2f &))&D::ClassType::SetCenter);
    _d.CPGF_MD_TEMPLATE _method("SetHalfSize", (void (D::ClassType::*) (float, float))&D::ClassType::SetHalfSize);
    _d.CPGF_MD_TEMPLATE _method("SetHalfSize", (void (D::ClassType::*) (const sf::Vector2f &))&D::ClassType::SetHalfSize);
    _d.CPGF_MD_TEMPLATE _method("SetFromRect", &D::ClassType::SetFromRect);
    _d.CPGF_MD_TEMPLATE _method("GetCenter", &D::ClassType::GetCenter);
    _d.CPGF_MD_TEMPLATE _method("GetHalfSize", &D::ClassType::GetHalfSize);
    _d.CPGF_MD_TEMPLATE _method("GetRect", &D::ClassType::GetRect);
    _d.CPGF_MD_TEMPLATE _method("Move", (void (D::ClassType::*) (float, float))&D::ClassType::Move);
    _d.CPGF_MD_TEMPLATE _method("Move", (void (D::ClassType::*) (const sf::Vector2f &))&D::ClassType::Move);
    _d.CPGF_MD_TEMPLATE _method("Zoom", &D::ClassType::Zoom);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
