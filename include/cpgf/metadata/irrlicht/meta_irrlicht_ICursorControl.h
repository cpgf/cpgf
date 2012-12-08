// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ICURSORCONTROL_H
#define __META_IRRLICHT_ICURSORCONTROL_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_icursorcontrol(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("GUICursorIconNames", &GUICursorIconNames);
    _d.CPGF_MD_TEMPLATE _enum<ECURSOR_ICON>("ECURSOR_ICON")
        ._element("ECI_NORMAL", irr::gui::ECI_NORMAL)
        ._element("ECI_CROSS", irr::gui::ECI_CROSS)
        ._element("ECI_HAND", irr::gui::ECI_HAND)
        ._element("ECI_HELP", irr::gui::ECI_HELP)
        ._element("ECI_IBEAM", irr::gui::ECI_IBEAM)
        ._element("ECI_NO", irr::gui::ECI_NO)
        ._element("ECI_WAIT", irr::gui::ECI_WAIT)
        ._element("ECI_SIZEALL", irr::gui::ECI_SIZEALL)
        ._element("ECI_SIZENESW", irr::gui::ECI_SIZENESW)
        ._element("ECI_SIZENWSE", irr::gui::ECI_SIZENWSE)
        ._element("ECI_SIZENS", irr::gui::ECI_SIZENS)
        ._element("ECI_SIZEWE", irr::gui::ECI_SIZEWE)
        ._element("ECI_UP", irr::gui::ECI_UP)
        ._element("ECI_COUNT", irr::gui::ECI_COUNT)
    ;
    _d.CPGF_MD_TEMPLATE _enum<ECURSOR_PLATFORM_BEHAVIOR>("ECURSOR_PLATFORM_BEHAVIOR")
        ._element("ECPB_NONE", irr::gui::ECPB_NONE)
        ._element("ECPB_X11_CACHE_UPDATES", irr::gui::ECPB_X11_CACHE_UPDATES)
    ;
}


template <typename D>
void buildMetaClass_ICursorControl(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("setVisible", &D::ClassType::setVisible);
    _d.CPGF_MD_TEMPLATE _method("isVisible", &D::ClassType::isVisible);
    _d.CPGF_MD_TEMPLATE _method("setPosition", (void (D::ClassType::*) (const core::position2d< f32 > &))&D::ClassType::setPosition);
    _d.CPGF_MD_TEMPLATE _method("setPosition", (void (D::ClassType::*) (f32, f32))&D::ClassType::setPosition);
    _d.CPGF_MD_TEMPLATE _method("setPosition", (void (D::ClassType::*) (const core::position2d< s32 > &))&D::ClassType::setPosition);
    _d.CPGF_MD_TEMPLATE _method("setPosition", (void (D::ClassType::*) (s32, s32))&D::ClassType::setPosition);
    _d.CPGF_MD_TEMPLATE _method("getPosition", &D::ClassType::getPosition);
    _d.CPGF_MD_TEMPLATE _method("getRelativePosition", &D::ClassType::getRelativePosition);
    _d.CPGF_MD_TEMPLATE _method("setReferenceRect", &D::ClassType::setReferenceRect)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("setActiveIcon", &D::ClassType::setActiveIcon);
    _d.CPGF_MD_TEMPLATE _method("getActiveIcon", &D::ClassType::getActiveIcon);
    _d.CPGF_MD_TEMPLATE _method("addIcon", &D::ClassType::addIcon);
    _d.CPGF_MD_TEMPLATE _method("changeIcon", &D::ClassType::changeIcon);
    _d.CPGF_MD_TEMPLATE _method("getSupportedIconSize", &D::ClassType::getSupportedIconSize);
    _d.CPGF_MD_TEMPLATE _method("setPlatformBehavior", &D::ClassType::setPlatformBehavior);
    _d.CPGF_MD_TEMPLATE _method("getPlatformBehavior", &D::ClassType::getPlatformBehavior);
}


template <typename D>
void buildMetaClass_SCursorSprite(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (gui::IGUISpriteBank *, s32, const core::position2d< s32 > &)>()
        ._default(copyVariantFromCopyable(core::position2d< s32 >(0, 0)))
    ;
    _d.CPGF_MD_TEMPLATE _field("SpriteBank", &D::ClassType::SpriteBank);
    _d.CPGF_MD_TEMPLATE _field("SpriteId", &D::ClassType::SpriteId);
    _d.CPGF_MD_TEMPLATE _field("HotSpot", &D::ClassType::HotSpot);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
