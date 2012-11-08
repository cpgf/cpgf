// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IGUITABCONTROL_H
#define __META_IRRLICHT_IGUITABCONTROL_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IGUITab(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getBackgroundColor", &D::ClassType::getBackgroundColor);
    _d.CPGF_MD_TEMPLATE _method("getNumber", &D::ClassType::getNumber);
    _d.CPGF_MD_TEMPLATE _method("getTextColor", &D::ClassType::getTextColor);
    _d.CPGF_MD_TEMPLATE _method("isDrawingBackground", &D::ClassType::isDrawingBackground);
    _d.CPGF_MD_TEMPLATE _method("setBackgroundColor", &D::ClassType::setBackgroundColor);
    _d.CPGF_MD_TEMPLATE _method("setDrawBackground", &D::ClassType::setDrawBackground)
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("setTextColor", &D::ClassType::setTextColor);
}


template <typename D>
void buildMetaClass_IGUITabControl(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("addTab", &D::ClassType::addTab)
        ._default(copyVariantFromCopyable(-1))
    ;
    _d.CPGF_MD_TEMPLATE _method("getActiveTab", &D::ClassType::getActiveTab);
    _d.CPGF_MD_TEMPLATE _method("getTab", &D::ClassType::getTab);
    _d.CPGF_MD_TEMPLATE _method("getTabCount", &D::ClassType::getTabCount);
    _d.CPGF_MD_TEMPLATE _method("getTabExtraWidth", &D::ClassType::getTabExtraWidth);
    _d.CPGF_MD_TEMPLATE _method("getTabHeight", &D::ClassType::getTabHeight);
    _d.CPGF_MD_TEMPLATE _method("getTabMaxWidth", &D::ClassType::getTabMaxWidth);
    _d.CPGF_MD_TEMPLATE _method("getTabVerticalAlignment", &D::ClassType::getTabVerticalAlignment);
    _d.CPGF_MD_TEMPLATE _method("setActiveTab", (bool (D::ClassType::*) (s32))&D::ClassType::setActiveTab);
    _d.CPGF_MD_TEMPLATE _method("setActiveTab", (bool (D::ClassType::*) (IGUIElement *))&D::ClassType::setActiveTab);
    _d.CPGF_MD_TEMPLATE _method("setTabExtraWidth", &D::ClassType::setTabExtraWidth);
    _d.CPGF_MD_TEMPLATE _method("setTabHeight", &D::ClassType::setTabHeight);
    _d.CPGF_MD_TEMPLATE _method("setTabMaxWidth", &D::ClassType::setTabMaxWidth);
    _d.CPGF_MD_TEMPLATE _method("setTabVerticalAlignment", &D::ClassType::setTabVerticalAlignment);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
